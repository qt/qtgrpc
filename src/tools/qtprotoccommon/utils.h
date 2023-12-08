// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QTPROTOBUFGEN_UTILS_H
#define QTPROTOBUFGEN_UTILS_H

#include <string>
#include <vector>
#include <algorithm>

#ifdef QT_PROTOBUF_DEBUG_GENERATOR
#    include <iostream>
#    define QT_PROTOBUF_DEBUG(X) std::clog << X << std::endl
#else
#    define QT_PROTOBUF_DEBUG(X)
#endif

namespace google::protobuf {
class FileDescriptor;
}

namespace qtprotoccommon {
namespace utils {
std::vector<std::string> split(std::string_view s, std::string_view c, bool keepEmpty = false);
std::string replace(std::string_view where, std::string_view from, std::string_view to);
void asciiToLower(std::string &str);
std::string removeFileSuffix(std::string fileName);
std::string extractFileBasename(std::string fileName);
std::string capitalizeAsciiName(std::string name);
std::string deCapitalizeAsciiName(std::string name);
std::string &rtrim(std::string &s);
// trim from beginning of string (left)
std::string &ltrim(std::string &s);
// trim from both ends of string (right then left)
std::string &trim(std::string &s);

bool contains(std::string_view s, char c);
bool contains(std::string_view s, std::string_view c);
template<typename Container>
bool contains(const Container &c, typename Container::value_type v)
{ return std::find(std::begin(c), std::end(c), v) != std::end(c); }

bool startsWith(std::string_view s, char c);
bool startsWith(std::string_view s, std::string_view c);

bool endsWith(std::string_view s, char c);
bool endsWith(std::string_view s, std::string_view c);

template<typename T>
std::string join(const T &container, std::string_view separator, std::string_view prefix = "",
                 std::string_view suffix = "")
{
    std::string output = std::string(prefix);
    bool first = true;
    for (const std::string &value : container) {
        if (!first)
            output += separator;
        first = false;
        output += value;
    }
    output += suffix;
    return output;
}

size_t count(std::string_view s, char c);

} // namespace utils
} // namespace qtprotoccommon
#endif // QTPROTOBUFGEN_UTILS_H
