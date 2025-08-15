// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#pragma once

#include "tags.h"
#include "certificates.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/security/server_credentials.h>

#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <future>

struct ListeningPort
{
    std::string addressUri;
    std::shared_ptr<grpc::ServerCredentials> creds;
    int selectedPort = -1;
};

class MockServer
{
public:
    enum class State {
        Stopped,
        Starting,
        Started,
        Processing,
        ShuttingDown,
    };

    MockServer();
    ~MockServer();

    grpc::ServerCompletionQueue *cq() { return mCQ.get(); }
    State state() const { return mState.load(std::memory_order_acquire); }

    bool start(std::vector<ListeningPort> ports, std::vector<grpc::Service *> services);
    bool stop();

    bool processTag(int timeoutMs = -1);
    bool startAsyncProcessing(int timeoutMs = -1);
    bool stopAsyncProcessing();

    void startRpcTag(AbstractRpcTag &tag) { tag.start(mCQ.get()); }

    MockServer &step(int timeoutMs = -1);
    bool waitForAllSteps();

private:
    bool transitionState(State from, State to);

private:
    std::unique_ptr<grpc::Server> mServer;
    std::unique_ptr<grpc::ServerCompletionQueue> mCQ;
    std::vector<std::future<bool>> mFutures;
    std::thread mProcessingThread;
    std::atomic<State> mState = State::Stopped;
};
