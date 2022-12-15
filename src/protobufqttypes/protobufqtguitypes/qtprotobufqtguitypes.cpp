// Copyright (C) 2023 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "qtprotobufqtguitypes.h"
#include "private/QtGui.qpb.h"

#include <QtProtobufQtCoreTypes/private/qtprotobufqttypescommon_p.h>

#include <QtGui/qrgba64.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qvector2d.h>
#include <QtGui/qtransform.h>
#include <QtGui/qquaternion.h>
#include <QtGui/qimage.h>
#include <QtCore/qbuffer.h>

QT_BEGIN_NAMESPACE

QRgba64 convert(const QtProtobufPrivate::QtGui::QRgba64 &from)
{
    return QRgba64::fromRgba64(from.rgba64());
}

QtProtobufPrivate::QtGui::QRgba64 convert(const QRgba64 &from)
{
    QtProtobufPrivate::QtGui::QRgba64 rgba64;
    rgba64.setRgba64((quint64)from);
    return rgba64;
}

std::optional<QMatrix4x4> convert(const QtProtobufPrivate::QtGui::QMatrix4x4 &from)
{
    QtProtobuf::floatList list = from.m();
    if (list.size() == 16) {
        return QMatrix4x4(list[0], list[1], list[2], list[3],
                          list[4], list[5], list[6], list[7],
                          list[8], list[9], list[10], list[11],
                          list[12], list[13], list[14], list[15]);
    }
    qWarning() << "Input for QMatrix4x4 should provide 16 values, but size = "
               << list.size();

    return std::nullopt;
}

QtProtobufPrivate::QtGui::QMatrix4x4 convert(const QMatrix4x4 &from)
{
    const float *matrixData = from.data();
    QtProtobufPrivate::QtGui::QMatrix4x4 matrix;
    // QMatrix4x4::data returned in column-major format
    matrix.setM({matrixData[0], matrixData[4], matrixData[8], matrixData[12],
                 matrixData[1], matrixData[5], matrixData[9], matrixData[13],
                 matrixData[2], matrixData[6], matrixData[10], matrixData[14],
                 matrixData[3], matrixData[7], matrixData[11], matrixData[15]});
    return matrix;
}

QVector2D convert(const QtProtobufPrivate::QtGui::QVector2D &from)
{
    return QVector2D(from.xPos(), from.yPos());;
}

QtProtobufPrivate::QtGui::QVector2D convert(const QVector2D &from)
{
    QtProtobufPrivate::QtGui::QVector2D vector2D;
    vector2D.setXPos(from.x());
    vector2D.setYPos(from.y());
    return vector2D;
}

QVector3D convert(const QtProtobufPrivate::QtGui::QVector3D &from)
{
    return QVector3D(from.xPos(), from.yPos(), from.zPos());
}

QtProtobufPrivate::QtGui::QVector3D convert(const QVector3D &from)
{
    QtProtobufPrivate::QtGui::QVector3D vector3D;
    vector3D.setXPos(from.x());
    vector3D.setYPos(from.y());
    vector3D.setZPos(from.z());
    return vector3D;
}

QVector4D convert(const QtProtobufPrivate::QtGui::QVector4D &from)
{
    return QVector4D(from.xPos(), from.yPos(), from.zPos(), from.wPos());
}

QtProtobufPrivate::QtGui::QVector4D convert(const QVector4D &from)
{
    QtProtobufPrivate::QtGui::QVector4D vector4D;
    vector4D.setXPos(from.x());
    vector4D.setYPos(from.y());
    vector4D.setZPos(from.z());
    vector4D.setWPos(from.w());
    return vector4D;
}

std::optional<QTransform> convert(const QtProtobufPrivate::QtGui::QTransform &from)
{
    QtProtobuf::doubleList list = from.m();
    if (list.size() == 9) {
        return QTransform(list[0], list[1], list[2],
                list[3], list[4], list[5],
                list[6], list[7], list[8]);
    }
    qWarning() << "Input list for QTransform should provide 9 members. But size = "
               << list.size();
    return std::nullopt;
}

QtProtobufPrivate::QtGui::QTransform convert(const QTransform &from)
{
    QtProtobufPrivate::QtGui::QTransform transform;
    transform.setM({from.m11(), from.m12(), from.m13(),
                    from.m21(), from.m22(), from.m23(),
                    from.m31(), from.m32(), from.m33()});
    return transform;
}

QQuaternion convert(const QtProtobufPrivate::QtGui::QQuaternion &from)
{
    return QQuaternion(from.scalar(), from.x(), from.y(), from.z());
}

QtProtobufPrivate::QtGui::QQuaternion convert(const QQuaternion &from)
{
    QtProtobufPrivate::QtGui::QQuaternion quaternion;
    quaternion.setScalar(from.scalar());
    quaternion.setX(from.x());
    quaternion.setY(from.y());
    quaternion.setZ(from.z());
    return quaternion;
}

QImage convert(const QtProtobufPrivate::QtGui::QImage &from)
{
    return QImage::fromData(from.data(), from.format().toLatin1().data());
}

std::optional<QtProtobufPrivate::QtGui::QImage> convert(const QImage &from)
{
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    if (from.save(&buffer, "PNG")) {
        QtProtobufPrivate::QtGui::QImage image;
        image.setData(data);
        image.setFormat(QLatin1StringView("PNG"));
        return image;
    }
    return std::nullopt;
}

namespace QtProtobuf {
/*!
    Registers serializers for the Qt::ProtobufQtGuiTypes library.
*/
void qRegisterProtobufQtGuiTypes() {
    QtProtobufPrivate::registerQtTypeHandler<QRgba64, QtProtobufPrivate::QtGui::QRgba64>();
    QtProtobufPrivate::registerQtTypeHandler<QMatrix4x4, QtProtobufPrivate::QtGui::QMatrix4x4>();
    QtProtobufPrivate::registerQtTypeHandler<QVector2D, QtProtobufPrivate::QtGui::QVector2D>();
    QtProtobufPrivate::registerQtTypeHandler<QVector3D, QtProtobufPrivate::QtGui::QVector3D>();
    QtProtobufPrivate::registerQtTypeHandler<QVector4D, QtProtobufPrivate::QtGui::QVector4D>();
    QtProtobufPrivate::registerQtTypeHandler<QTransform, QtProtobufPrivate::QtGui::QTransform>();
    QtProtobufPrivate::registerQtTypeHandler<QQuaternion, QtProtobufPrivate::QtGui::QQuaternion>();
    QtProtobufPrivate::registerQtTypeHandler<QImage, QtProtobufPrivate::QtGui::QImage>();
}
}

QT_END_NAMESPACE
