// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "rpcjobs.h"
#include <qrpcbench_common.h>

#include <grpcpp/completion_queue.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/security/tls_certificate_provider.h>
#include <grpcpp/security/tls_credentials_options.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

#include <atomic>
#include <cassert>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <thread>

class AsyncServer
{
public:
    enum class State { Created, Running, Finished };

    void run(const QList<QString> &transports)
    {
        constexpr int numCompletionQueues = 1;      // scale to ~numCPUs/2 for better throughput
        constexpr int threadsPerQueue = 1;          // Increase if CQ processing is the bottleneck
        constexpr int initialInstancesPerQueue = 1; // raise for high concurrency

        State expected = State::Created;
        while (!mState.compare_exchange_weak(expected, State::Running)) {
            if (expected != State::Created)
                return;
        }

        {
            grpc::ServerBuilder builder;
            std::shared_ptr<grpc::ServerCredentials> tcpCreds = grpc::InsecureServerCredentials();
            std::shared_ptr<grpc::ServerCredentials> tlsCreds;
            {
                auto provider = std::make_shared<grpc::experimental::InMemoryCertificateProvider>();
                std::string sslRootKey(Bench::SslRootKey);
                auto isOk = provider->UpdateRoot(sslRootKey).ok();
                if (!isOk) {
                    std::cerr << "failed to update SslRootKey\n";
                    std::terminate();
                }
                std::vector<grpc::experimental::IdentityKeyOrSignerCertPair> identityPairs;
                identityPairs.emplace_back(grpc::experimental::IdentityKeyOrSignerCertPair{
                    .private_key = std::string(Bench::SslKey.data(), Bench::SslKey.length()),
                    .certificate_chain = std::string(Bench::SslCert.data(),
                                                     Bench::SslCert.length()),
                });
                isOk = provider->UpdateIdentityKeyCertPair(std::move(identityPairs)).ok();
                if (!isOk) {
                    std::cerr << "Failed to update identity key\n";
                    std::terminate();
                }
                auto tlsOpts = grpc::experimental::TlsServerCredentialsOptions::Create(provider)
                                   .value();
                tlsOpts.set_root_certificate_provider(provider);
                // Needed for TLS debugging in wireshark (Edit > Preferences >
                // Protocol > TLS > Master-Secret log filename)
                tlsOpts.set_tls_session_key_log_file_path("sslkeylog.log");
                tlsOpts.set_cert_request_type(GRPC_SSL_REQUEST_CLIENT_CERTIFICATE_AND_VERIFY);
                tlsCreds = grpc::experimental::TlsServerCredentials(tlsOpts);
            }
            for (const auto &t : transports) {
                const auto address = Bench::getTransportAddress(t);
                if (t == "https")
                    builder.AddListeningPort(address, tlsCreds);
                else
                    builder.AddListeningPort(address, tcpCreds);
                std::cout << std::format("Server listening on: {}, {}\n", t.toStdString(), address);
            }
            builder.RegisterService(&mService);
            for (int i = 0; i < numCompletionQueues; ++i)
                mCompletionQueues.emplace_back(builder.AddCompletionQueue());
            mServer = builder.BuildAndStart();
        }

        for (auto &cq : mCompletionQueues) {
            for (int i = 0; i < initialInstancesPerQueue; ++i) {
                new UnaryCall(cq.get(), &mService);
                new ServerStreaming(cq.get(), &mService);
                new ClientStreaming(cq.get(), &mService);
                new BiDiStreaming(cq.get(), &mService);
            }
        }

        for (auto &cq : mCompletionQueues) {
            for (int i = 0; i < threadsPerQueue; ++i)
                mThreads.emplace_back([cq = cq.get()] { processRPCs(cq); });
        }
    }

private:
    qt::bench::BenchmarkService::AsyncService mService;
    std::unique_ptr<grpc::Server> mServer;
    std::vector<std::unique_ptr<grpc::ServerCompletionQueue>> mCompletionQueues;
    std::vector<std::jthread> mThreads;

    std::atomic<State> mState = { State::Created };
    static_assert(std::atomic<State>::is_always_lock_free);

    // A single thread works on the completion queue.
    static void processRPCs(grpc::ServerCompletionQueue *cq)
    {
        assert(cq);
        void *rawTag = nullptr;
        bool ok = false;
        while (cq->Next(&rawTag, &ok))
            static_cast<RpcJob *>(rawTag)->onProcess(ok);
    }
};

int main(int argc, char *argv[])
{
    QStringList args;
    for (int i = 0; i < argc; ++i)
        args.push_back(argv[i]);
    QCommandLineParser parser;
    parser.setApplicationDescription("Asyncbenchserver");
    parser.addHelpOption();

    QCommandLineOption transport({ "t", "transport" }, "Use Transport(s)", "http|https");
#ifdef Q_OS_WINDOWS
    transport.setDefaultValues({ "http", "https" });
#else
    transport.setValueName("http|https|unix");
    transport.setDefaultValues({ "http", "https", "unix" });
#endif
    parser.addOption(transport);
    parser.process(args);

    AsyncServer server;
    server.run(parser.values(transport));
}
