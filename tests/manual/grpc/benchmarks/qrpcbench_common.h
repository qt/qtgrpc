// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QRPCBENCH_COMMON_H
#define QRPCBENCH_COMMON_H

#include <QtCore/qcommandlineoption.h>
#include <QtCore/qcommandlineparser.h>
#include <QtCore/qelapsedtimer.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qsysinfo.h>

#include <chrono>
#include <concepts>
#include <cstdlib>
#include <format>
#include <iostream>
#include <string_view>

inline std::string getTransportAddress(QAnyStringView transport)
{
    if (transport == "http") {
        return "localhost:65002";
    } else if (transport == "https") {
        return "localhost:65003";
    }
#ifndef Q_OS_WINDOWS
    else if (transport == "unix") {
        return "unix-abstract:bench";
    }
#endif
    else {
        std::cerr << "Invalid transport specified: " << transport.toString().toStdString()
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

// Valid for the next 100 years.
static constexpr std::string_view SslCert = R"(
-----BEGIN CERTIFICATE-----
MIID+jCCAuKgAwIBAgIUVJNsgxX2GVzFdU8yC0xwETtnHWwwDQYJKoZIhvcNAQEL
BQAwgZMxCzAJBgNVBAYTAkRFMQ8wDQYDVQQIDAZCZXJsaW4xDzANBgNVBAcMBkJl
cmxpbjEcMBoGA1UECgwTVGhlIFF0IENvbXBhbnkgR21iSDEMMAoGA1UECwwDUiZE
MRIwEAYDVQQDDAlsb2NhbGhvc3QxIjAgBgkqhkiG9w0BCQEWE2Rlbm5pcy5vYmVy
c3RAcXQuaW8wIBcNMjUwMTE2MTU1ODIzWhgPMjEyNDEyMjMxNTU4MjNaMIGTMQsw
CQYDVQQGEwJERTEPMA0GA1UECAwGQmVybGluMQ8wDQYDVQQHDAZCZXJsaW4xHDAa
BgNVBAoME1RoZSBRdCBDb21wYW55IEdtYkgxDDAKBgNVBAsMA1ImRDESMBAGA1UE
AwwJbG9jYWxob3N0MSIwIAYJKoZIhvcNAQkBFhNkZW5uaXMub2JlcnN0QHF0Lmlv
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAszG9ps195LVF9kUDbQXj
1/5JAemlRvLMAJcr28virbqcCqphnode/vYGYiy2pbnPMJKxN9MsH5NMZH1/sn1A
PHGQ3sD6vcp24BZkgitDXLQab3QCF00K3Kka7GO6wQ9eloobBTtAWZDU/Px2LkLx
bmlIsr9Lbic2ZbKVWL3oRUECIAppmjEn1La+QSsV5lI2vROy6/b84OrBcYRDHifp
q70ZgIGO7AcucIeRAJGpfJJHxx8cuguDf0bgXJjsnIfXiAXCQkwtTWCrvES9akP4
dIJUz8onKAe7TRS1jaNGnXpikezhRa++GJNJhFAY3m6/0+qVHa0wByk5lxRoWV40
xwIDAQABo0IwQDAdBgNVHQ4EFgQUd2BWbXCVfYO0kHX+Ea5DcIzUlWIwHwYDVR0j
BBgwFoAU8jBTKqrkQqH2H7Kgyt6jtJtlZgcwDQYJKoZIhvcNAQELBQADggEBAG2f
xwaTuwBQ7ldk/8u4vfu1H8cq/eMLnmrbUm+QaSMdK1LXlhPQPpFcGN+6no/0tUA/
LhhzVzK6SRUgrm+IoGwL8ojyxdO760LgUOHN/VpvxQDY6DmBLct2L1sIRrjMNLym
ZgkXSmmeoswNgeonOwN8hIJz9Kx6xil1mPASGHvUaBFBdWDFxB/RuEYyv7X1wPeX
G2WqJZ+hoJESgtJFoybcbvqRGYq+ftXY8phacC1IuoaBinEBFxPrOkC2pzSHrvx2
Awm0fIbfSml23kefPh19Kquhf4NPdyUJo+RHuNn86tadANOEVVQ5slzaSuVu86Ym
u2A5Ea7yqvthhAzHrbg=
-----END CERTIFICATE-----
)";

static constexpr std::string_view SslKey = R"(
-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQCzMb2mzX3ktUX2
RQNtBePX/kkB6aVG8swAlyvby+KtupwKqmGeh17+9gZiLLaluc8wkrE30ywfk0xk
fX+yfUA8cZDewPq9ynbgFmSCK0NctBpvdAIXTQrcqRrsY7rBD16WihsFO0BZkNT8
/HYuQvFuaUiyv0tuJzZlspVYvehFQQIgCmmaMSfUtr5BKxXmUja9E7Lr9vzg6sFx
hEMeJ+mrvRmAgY7sBy5wh5EAkal8kkfHHxy6C4N/RuBcmOych9eIBcJCTC1NYKu8
RL1qQ/h0glTPyicoB7tNFLWNo0ademKR7OFFr74Yk0mEUBjebr/T6pUdrTAHKTmX
FGhZXjTHAgMBAAECggEAN5khTNXJT+LmmCiFjZgcP3IIWO2TeFXw8eX1l7bE2D5k
F/MRYsyBrv3KsT9KVFU4ccux7K46rHlZZHyD2G+ANMDPwC2EHsro41JPUQv3VJYU
9au60lv3GMvnLJ0s3qXUJUUoaREfQCrtyqjSSjw/CJDmG3+6+ax09kzYhbY2kPW7
RfIWZFTq9n5x3e7Zd2wPDD7PUiqZy3TIHFOwxtpSD3TapVismIFiRtLA6nV84gKf
zKCOLNqzU0usgfHiLBWGXxKcFVjXiv6Nss0o+TFbKhVPgh9r22tEkSFbLp19x2Ii
jF1DU1M5oR1ew9uF22IAQDl13xgwQ/qAHqtQssAR0QKBgQDoelgfUKeFndypiDct
zMtd9AxXAmDl4HSA01eUH9m6yXw5PS3mtK14eInJtywq+PwIvCeJz8IkaioG+JVi
x7xrACEcxjud6Yq8MkPpi8omFSlGBzL0VnZfJpEXUdwlYMiyy5viYLiFPBm8Z94x
Putiob227E1WEooaPMo77Hd4rQKBgQDFUz3bHU5R0tI4bBiM5WlBRsB1gfk/Sc10
WQi9D9d5uFfdFn2bXBzeBtWR8PYB9RO+SMc+i81Vva1IAk/7XbMLYXtoH2rrC6Fz
8OarQARhTGG5gfnAV48YD2AJGH4uCt1snVoJ/bxDz15vNSR0ywzfuURVgvSLXkPe
51PGXz6NwwKBgQDZZf+eWSgvVW6SwyUGmWrcU2puu3Stw3ZvOjO9+wL7H4whYsrX
4cIO1HnVvot5LBlUec9nmnds4jKnDjN0il/yl85fQClkBI+OalsDvYuujT9pkzXd
NDXBySkJa6247ocAXFNMITKstYVDoMYxuysXszTcKKIxiWjIHGzqGLmoiQKBgQC8
kQO3dJX3k2PZD1OWsVSYUKhyorYxSLHR0ZOMOKtNYmB0op197dSYSCenw4ET9cPc
P2hH2QlsOkpxWeRc7fm/knR/2CYwX3j2duu4EwEcigWJZS/qIsJX17mKd6F9Flzr
AqOckKFsm6o+06X3BmNTGJS4suBGntp1FNL16ua4SQKBgC0Y+822m1CjxLMtU6rr
5OH7MSHC7szyo+yiuch3APOOg1jxqjx5Ru1dohLvzU2vLmbFIWezxKmLwRYFjnoH
xhA6h48p0zvZRJlFjOyQCRAT4w3Hm938g9o81F+QNFWI44USsX/htdaPOxqR0SkP
UKzk7CPqzQYR2xHUi+VcBI93
-----END PRIVATE KEY-----
)";

static constexpr std::string_view SslRootKey = R"(
-----BEGIN CERTIFICATE-----
MIIECzCCAvOgAwIBAgIUBraeFSR5B//R/M22CKLtFNwf6XswDQYJKoZIhvcNAQEL
BQAwgZMxCzAJBgNVBAYTAkRFMQ8wDQYDVQQIDAZCZXJsaW4xDzANBgNVBAcMBkJl
cmxpbjEcMBoGA1UECgwTVGhlIFF0IENvbXBhbnkgR21iSDEMMAoGA1UECwwDUiZE
MRIwEAYDVQQDDAlsb2NhbGhvc3QxIjAgBgkqhkiG9w0BCQEWE2Rlbm5pcy5vYmVy
c3RAcXQuaW8wIBcNMjUwMTE2MTU1ODIzWhgPMjEyNDEyMjMxNTU4MjNaMIGTMQsw
CQYDVQQGEwJERTEPMA0GA1UECAwGQmVybGluMQ8wDQYDVQQHDAZCZXJsaW4xHDAa
BgNVBAoME1RoZSBRdCBDb21wYW55IEdtYkgxDDAKBgNVBAsMA1ImRDESMBAGA1UE
AwwJbG9jYWxob3N0MSIwIAYJKoZIhvcNAQkBFhNkZW5uaXMub2JlcnN0QHF0Lmlv
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAq2n9SNDLpd59+wV2AgOy
KYLMCcZE719w67cnYDAdFyBsVS5/Us2AiQtPYbESNDRX8SxOLM0b9FfTrFH3EU70
EIuClNqHq9OhtPoRoPo7bDULlEqOQeCFSMOrY2OW1m5ngvSqrY7LyVViH3QHk02g
afnkuCdTbAkNNfNyHWCkyVrad7RMk28t2dptSaH4SFripiaO2f59l8//lZ/6bdze
m2HhP5wcx88X02JdskmqcVuOOSlE03UoWGKG2O5TuRY0M48piypSuLz5Ajz72Erg
g3yFh98DKojW60EepM6Tgcq5l3UNlY4rzXBzCV2X+EO/VzvVKSc9u2k12cJmHZxh
lwIDAQABo1MwUTAdBgNVHQ4EFgQU8jBTKqrkQqH2H7Kgyt6jtJtlZgcwHwYDVR0j
BBgwFoAU8jBTKqrkQqH2H7Kgyt6jtJtlZgcwDwYDVR0TAQH/BAUwAwEB/zANBgkq
hkiG9w0BAQsFAAOCAQEARZWMphTnpwhUKS5/Z52MZOBTeqQgZ6vJ8SfRi1LIoDw/
tVrJQm1yLEvwZUY3NKGSOy6Rseuz68L6VOj9RHVc9xD5e8Si3gh/3Yyhjdv2g+1V
fJNaMEqvVA4sGnkXk21kbnloYaVpojyyBU5nBEivvFoSyYhlT3V+6gWqfFI6+F/o
FW9KC07WXcLcM41sQN7DsOx5q+6Qfc3VYl4z+k6X8rDEe3s1zFXZu2nPgYHD/New
9/h7mAQbil06MwuPOYbousc1uY95EVRLN4XrKHDkm3TxM8wHDipoanwyPnOqcpTy
Esh/Nj3INbL1yljh4Fi447mtfgg+ERh+BgAv61ZwsA==
-----END CERTIFICATE-----
)";

namespace Client {

template <typename T>
concept ClientConcept = requires(T t) {
    { T(std::string(), uint64_t()) };
    { t.unaryCall() } -> std::same_as<void>;
    { t.serverStreaming() } -> std::same_as<void>;
    { t.clientStreaming() } -> std::same_as<void>;
    { t.bidiStreaming() } -> std::same_as<void>;
};

template <ClientConcept C>
inline void benchmarkMain(std::string_view name, int argc, char *argv[])
{
    QStringList args;
    for (int i = 0; i < argc; ++i)
        args.push_back(argv[i]);

    QCommandLineParser parser;
    parser.setApplicationDescription(name.data());
    parser.addHelpOption();

    QCommandLineOption transport({ "t", "transport" }, "Use Transport", "http|https", "http");
#ifndef Q_OS_WINDOWS
    transport.setValueName("http|https|unix");
#endif

    QCommandLineOption calls({ "c", "calls" }, "Amount of calls made.", "size", "1000");
    QCommandLineOption payload({ "p", "payload" }, "Payload size in bytes", "size", "0");
    QCommandLineOption uniqueRpc({ "u", "unique" }, "Make each RPC on a fresh client");

    QCommandLineOption enableUnary("U", "Enable UnaryCalls");
    QCommandLineOption enableSStream("S", "Enable ServerStream");
    QCommandLineOption enableCStream("C", "Enable ClientStream");
    QCommandLineOption enableBStream("B", "Enable BiDiStream");

    parser.addOptions({
        transport,

        calls,
        payload,
        uniqueRpc,

        enableUnary,
        enableSStream,
        enableCStream,
        enableBStream,
    });

    parser.process(args);

    bool defaultRun = !parser.isSet(enableUnary) && !parser.isSet(enableSStream)
        && !parser.isSet(enableCStream) && !parser.isSet(enableBStream);
    uint64_t amountCalls = parser.value(calls).toULong();
    qsizetype payloadSize = parser.value(payload).toLong();
    const auto transportValue = parser.value(transport).toStdString();

    std::cout << std::format("#### Start of {} benchmark ####\n", name);
    std::cout << std::format("  cpu-arch: {}\n", QSysInfo::buildCpuArchitecture().toStdString());
    std::cout << std::format("  kernel: {}, {}\n", QSysInfo::kernelType().toStdString(),
                             QSysInfo::kernelVersion().toStdString());
    std::cout << std::format("  host URI: {}, {}\n\n", transportValue,
                             getTransportAddress(transportValue));

    if (parser.isSet(payload))
        std::cout << std::format("  Option: payload per message {} bytes\n", payloadSize);
    if (parser.isSet(uniqueRpc))
        std::cout << std::format("  Option: unique client per RPC {}\n", parser.isSet(uniqueRpc));
    if (parser.isSet(payload) || parser.isSet(uniqueRpc))
        std::cout << "\n";

    if (parser.isSet(uniqueRpc)) {
        {
            C client(transportValue, amountCalls, payloadSize);
            if (defaultRun || parser.isSet(enableUnary))
                client.unaryCall();
        }
        {
            C client(transportValue, amountCalls, payloadSize);
            if (defaultRun || parser.isSet(enableSStream))
                client.serverStreaming();
        }
        {
            C client(transportValue, amountCalls, payloadSize);
            if (defaultRun || parser.isSet(enableCStream))
                client.clientStreaming();
        }
        {
            C client(transportValue, amountCalls, payloadSize);
            if (defaultRun || parser.isSet(enableBStream))
                client.bidiStreaming();
        }
    } else {
        C client(transportValue, amountCalls, payloadSize);
        if (defaultRun || parser.isSet(enableUnary))
            client.unaryCall();
        if (defaultRun || parser.isSet(enableSStream))
            client.serverStreaming();
        if (defaultRun || parser.isSet(enableCStream))
            client.clientStreaming();
        if (defaultRun || parser.isSet(enableBStream))
            client.bidiStreaming();
    }

    std::cout << std::format("\n#### End of {} benchmark ####\n", name);
}

inline void printRpcResult(std::string benched, int64_t elapsedNs, uint64_t amountCalls,
                           uint64_t recvBytes = 0, uint64_t sendBytes = 0)
{
    using namespace std::chrono;

    std::cout << std::format("Finished benchmark: {}\n", benched);
    const auto ns = nanoseconds(elapsedNs);
    const auto us = duration_cast<microseconds>(ns);
    std::cout << std::format("  Completed calls: {}\n", amountCalls);
    std::cout << std::format("  Total time: {}, {}\n", us, ns);
    const auto avgNs = nanoseconds(ns / amountCalls);
    const auto avgUs = duration_cast<microseconds>(avgNs);
    std::cout << std::format("  Average time: {}, {}\n", avgUs, avgNs);
    if (recvBytes > 0 && sendBytes > 0) {
        std::cout << std::format("  Send bytes: {}, Recv bytes: {}\n", sendBytes, recvBytes);
        const auto sec = duration_cast<duration<double>>(ns);
        double totalTimeSec = sec.count();
        double totalBytes = static_cast<double>(recvBytes + sendBytes);
        double throughputKBPerSec = (totalBytes / 1024) / totalTimeSec;
        std::cout << std::format("  Throughput (KB/sec): {:.2f}\n", throughputKBPerSec);
    }
}

} // namespace Client

#endif // QRPCBENCH_COMMON_H
