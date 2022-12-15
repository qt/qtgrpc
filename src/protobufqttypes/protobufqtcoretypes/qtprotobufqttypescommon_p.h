// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef QTPROTOBUFQTTYPESCOMMON_P_H
#define QTPROTOBUFQTTYPESCOMMON_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtProtobuf/qtprotobuftypes.h>
#include <QtProtobuf/qprotobufserializer.h>
#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtCore/QtGlobal>

#include <optional>

QT_BEGIN_NAMESPACE

namespace QtProtobufPrivate {

template <typename T>
constexpr inline bool is_optional_v = false;
template <typename T>
constexpr inline bool is_optional_v<std::optional<T>> = true;

template<typename QType, typename PType>
void registerQtTypeHandler()
{
    registerHandler(
            QMetaType::fromType<QType>(),
            { [](const QProtobufSerializer *serializer, const QVariant &value,
                 const QProtobufPropertyOrderingInfo &info, QByteArray &buffer) {
                 auto do_convert = [](const QType &qtype) {
                     auto res = convert(qtype);
                     if constexpr (is_optional_v<decltype(res)>) {
                         if (!res) {
                             qWarning() << "Qt Proto Type conversion error.";
                             return PType();
                         }
                         return PType(std::move(*res));
                     } else {
                         return PType(std::move(res));
                     }
                 };
                 PType object = do_convert(value.value<QType>());
                 buffer.append(serializer->serializeObject(&object,
                                                           PType::propertyOrdering, info));
              },
              [](const QProtobufSerializer *serializer, QProtobufSelfcheckIterator &it,
                 QVariant &value) {
                  PType object;
                  serializer->deserializeObject(&object, PType::propertyOrdering, it);
                  auto res = convert(object);
                  if constexpr (is_optional_v<decltype(res)>) {
                      if (!res)
                          qWarning() << "Qt Proto Type conversion error.";
                      else
                          value = QVariant::fromValue<QType>(*res);
                  } else {
                      value = QVariant::fromValue<QType>(res);
                  }
              }
            });
}
} // namespace QtProtobufPrivate

QT_END_NAMESPACE

#endif // QTPROTOBUFQTTYPESCOMMON_P_H
