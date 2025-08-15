// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "mockserver.h"

#include <grpcpp/alarm.h>
#include <grpcpp/server_builder.h>

MockServer::MockServer() = default;
MockServer::~MockServer()
{
    stop();
};

bool MockServer::start(std::vector<ListeningPort> ports, std::vector<grpc::Service *> services)
{
    if (!transitionState(State::Stopped, State::Starting))
        return false;

    grpc::ServerBuilder builder;
    for (auto &p : ports)
        builder.AddListeningPort(p.addressUri, p.creds, &p.selectedPort);
    for (auto *s : services)
        builder.RegisterService(s);
    mCQ = builder.AddCompletionQueue();
    mServer = builder.BuildAndStart();
    if (!mServer || !mCQ) {
        mState = State::Stopped;
        return false;
    }
    mState = State::Started;
    return true;
}

bool MockServer::stop()
{
    State currentState = mState.load();
    if (currentState == State::Processing)
        stopAsyncProcessing();

    if (currentState != State::Started && currentState != State::Processing)
        return currentState == State::Stopped;

    mState = State::ShuttingDown;

    mServer->Shutdown(std::chrono::system_clock::now() + std::chrono::milliseconds(500));
    mCQ->Shutdown();

    void *drainTag;
    bool drainOk;
    while (mCQ->Next(&drainTag, &drainOk)) {
        if (drainTag)
            static_cast<AbstractTag*>(drainTag)->process(drainOk);
    }

    mServer.reset();
    mCQ.reset();
    mState = State::Stopped;

    return true;
}

bool MockServer::processTag(int timeoutMs)
{
    State currentState = mState.load();
    if (currentState != State::Processing && currentState != State::Started) {
        return false;
    }

    void *rawTag = nullptr;
    bool ok = false;
    const auto status = timeoutMs < 0
        ? mCQ->AsyncNext(&rawTag, &ok, gpr_inf_future(GPR_CLOCK_REALTIME))
        : mCQ->AsyncNext(&rawTag, &ok,
                         std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMs));

    if (rawTag && status == grpc::CompletionQueue::NextStatus::GOT_EVENT) {
        static_cast<AbstractTag *>(rawTag)->process(ok);
        return true;
    }

    return false;
}

bool MockServer::startAsyncProcessing(int timeoutMs)
{
    if (!transitionState(State::Started, State::Processing))
        return false;

    mProcessingThread = std::thread([this, timeoutMs] {
        while (mState.load(std::memory_order_acquire) == State::Processing) {
            processTag(timeoutMs);
        }
    });
    return true;
}

bool MockServer::stopAsyncProcessing()
{
    if (!transitionState(State::Processing, State::Started))
        return false;

    if (mProcessingThread.joinable()) {
        grpc::Alarm alarm;
        // Trigger an event so that the processing loop detects the change.
        alarm.Set(mCQ.get(), gpr_now(gpr_clock_type::GPR_CLOCK_REALTIME), new VoidTag());
        mProcessingThread.join();
    }
    return true;
}

MockServer &MockServer::step(int timeoutMs)
{
    mFutures.emplace_back(std::async(std::launch::async,
                                     [this, timeoutMs] { return processTag(timeoutMs); }));
    return *this;
}

bool MockServer::waitForAllSteps()
{
    for (auto &f : mFutures) {
        if (!f.get())
            return false;
    }
    mFutures.clear();
    return true;
}

bool MockServer::transitionState(State from, State to)
{
    State expected = from;
    return mState.compare_exchange_strong(expected, to);
}
