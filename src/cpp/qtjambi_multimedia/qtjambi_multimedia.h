/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBI_MULTIMEDIA_H
#define QTJAMBI_MULTIMEDIA_H

#include <qtjambi/qtjambi_global.h>
#include <qtjambi_gui/qtjambi_gui_qhashes.h>
#include <QtCore/QtCore>
#include <QtMultimedia/QtMultimedia>
#include <qtjambi_core/qtjambi_core_qhashes.h>
#include <qtjambi_network/qtjambi_network.h>

hash_type qHash(const QAudioFormat& value);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QVideoFrame::PaintOptions& value){
    return qHashMulti(0, value.backgroundColor, int(value.aspectRatioMode));
}

inline bool operator==(const QVideoFrame::PaintOptions& value1, const QVideoFrame::PaintOptions& value2){
    return value1.backgroundColor==value2.backgroundColor && value1.aspectRatioMode==value2.aspectRatioMode;
}

inline hash_type qHash(const QVideoFrameFormat& value){
    if(!value.isValid())
        return 0;
    hash_type hashCode = qHash(int(value.pixelFormat()));
    hashCode = hashCode * 31 + qHash(value.frameWidth());
    hashCode = hashCode * 31 + qHash(value.frameHeight());
    hashCode = hashCode * 31 + qHash(value.planeCount());
    hashCode = hashCode * 31 + qHash(value.viewport());
    hashCode = hashCode * 31 + qHash(int(value.scanLineDirection()));
    hashCode = hashCode * 31 + qHash(int(value.frameRate()));
    hashCode = hashCode * 31 + qHash(int(value.yCbCrColorSpace()));
    hashCode = hashCode * 31 + qHash(value.isMirrored());
    hashCode = hashCode * 31 + qHash(value.vertexShaderFileName());
    hashCode = hashCode * 31 + qHash(value.fragmentShaderFileName());
    return hashCode;
}

inline hash_type qHash(const QAudioDevice& value){
    if(!value.isNull())
        return 0;
    hash_type hashCode = qHash(value.id());
    hashCode = hashCode * 31 + qHash(value.description());
    hashCode = hashCode * 31 + qHash(value.isDefault());
    hashCode = hashCode * 31 + qHash(int(value.mode()));
    hashCode = hashCode * 31 + qHash(value.preferredFormat());
    hashCode = hashCode * 31 + qHash(value.minimumSampleRate());
    hashCode = hashCode * 31 + qHash(value.maximumSampleRate());
    hashCode = hashCode * 31 + qHash(value.minimumChannelCount());
    hashCode = hashCode * 31 + qHash(value.maximumChannelCount());
    hashCode = hashCode * 31 + qHash(value.supportedSampleFormats());
    return hashCode;
}

inline hash_type qHash(const QCameraFormat& value){
    if(!value.isNull())
        return 0;
    hash_type hashCode = qHash(int(value.pixelFormat()));
    hashCode = hashCode * 31 + qHash(value.resolution());
    hashCode = hashCode * 31 + qHash(value.minFrameRate());
    hashCode = hashCode * 31 + qHash(value.maxFrameRate());
    return hashCode;
}

inline hash_type qHash(const QCameraDevice& value){
    if(!value.isNull())
        return 0;
    hash_type hashCode = qHash(value.id());
    hashCode = hashCode * 31 + qHash(value.description());
    hashCode = hashCode * 31 + qHash(value.isDefault());
    hashCode = hashCode * 31 + qHash(int(value.position()));
    hashCode = hashCode * 31 + qHash(value.photoResolutions());
    hashCode = hashCode * 31 + qHash(value.videoFormats());
    return hashCode;
}

inline hash_type qHash(const QMediaTimeRange::Interval& value){
    return qHashMulti(0, value.start(), value.end(), value.isNormal());
}

inline hash_type qHash(const QMediaFormat& value){
    return qHashMulti(0, value.fileFormat(), int(value.audioCodec()), int(value.videoCodec()));
}
#endif


inline hash_type qHash(const QVideoFrame& value){
    if(!value.isValid())
        return 0;
    hash_type hashCode = qHash(value.size());
    hashCode = hashCode * 31 + qHash(value.width());
    hashCode = hashCode * 31 + qHash(value.height());
    hashCode = hashCode * 31 + qHash(value.endTime());
    hashCode = hashCode * 31 + qHash(int(value.mapMode()));
    hashCode = hashCode * 31 + qHash(value.isMapped());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    hashCode = hashCode * 31 + qHash(value.fieldType());
#else
    hashCode = hashCode * 31 + qHash(value.surfaceFormat());
    hashCode = hashCode * 31 + qHash(value.isReadable());
#endif
    hashCode = hashCode * 31 + qHash(value.startTime());
    hashCode = hashCode * 31 + qHash(int(value.handleType()));
    hashCode = hashCode * 31 + qHash(value.isReadable());
    hashCode = hashCode * 31 + qHash(value.isWritable());
    hashCode = hashCode * 31 + qHash(int(value.pixelFormat()));
    hashCode = hashCode * 31 + qHash(value.planeCount());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    hashCode = hashCode * 31 + qHash(value.mappedBytes());
    hashCode = hashCode * 31 + qHash(value.bytesPerLine());
    hashCode = hashCode * 31 + qHash(QByteArray(reinterpret_cast<const char*>(value.bits()), value.mappedBytes()));
#else
    for(int i=0; i< value.planeCount(); ++i){
        hashCode = hashCode * 31 + qHash(value.mappedBytes(i));
        hashCode = hashCode * 31 + qHash(value.bytesPerLine(i));
        hashCode = hashCode * 31 + qHash(QByteArray(reinterpret_cast<const char*>(value.bits(i)), value.mappedBytes(i)));
    }
#endif
    return hashCode;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QVideoEncoderSettings& value){
    if(value.isNull())
        return 0;
    hash_type hashCode = qHash(value.encodingMode());
    hashCode = hashCode * 31 + qHash(value.codec());
    hashCode = hashCode * 31 + qHash(value.bitRate());
    hashCode = hashCode * 31 + qHash(value.quality());
    hashCode = hashCode * 31 + qHash(value.frameRate());
    hashCode = hashCode * 31 + qHash(value.resolution());
    return hashCode;
}

inline hash_type qHash(const QImageEncoderSettings& value){
    if(value.isNull())
        return 0;
    hash_type hashCode = qHash(value.quality());
    hashCode = hashCode * 31 + qHash(value.codec());
    hashCode = hashCode * 31 + qHash(value.resolution());
    return hashCode;
}
#endif

inline hash_type qHash(const QMediaTimeRange& value){
    hash_type hashCode = qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.intervals());
    hashCode = hashCode * 31 + qHash(value.latestTime());
    hashCode = hashCode * 31 + qHash(value.earliestTime());
    hashCode = hashCode * 31 + qHash(value.isContinuous());
    return hashCode;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QMediaTimeInterval& value){
    hash_type hashCode = qHash(value.start());
    hashCode = hashCode * 31 + qHash(value.end());
    hashCode = hashCode * 31 + qHash(value.isNormal());
    return hashCode;
}

inline hash_type qHash(const QMediaServiceProviderHint& value){
    if(value.isNull())
        return 0;
    hash_type hashCode = qHash(value.type());
    hashCode = hashCode * 31 + qHash(value.codecs());
    hashCode = hashCode * 31 + qHash(value.device());
    hashCode = hashCode * 31 + qHash(int(value.features()));
    hashCode = hashCode * 31 + qHash(value.mimeType());
    hashCode = hashCode * 31 + qHash(value.cameraPosition());
    return hashCode;
}

inline hash_type qHash(const QMediaResource& value){
    if(value.isNull())
        return 0;
    hash_type hashCode = qHash(value.url());
    hashCode = hashCode * 31 + qHash(value.request());
    hashCode = hashCode * 31 + qHash(value.dataSize());
    hashCode = hashCode * 31 + qHash(value.language());
    hashCode = hashCode * 31 + qHash(value.mimeType());
    hashCode = hashCode * 31 + qHash(value.audioCodec());
    hashCode = hashCode * 31 + qHash(value.resolution());
    hashCode = hashCode * 31 + qHash(value.sampleRate());
    hashCode = hashCode * 31 + qHash(value.videoCodec());
    hashCode = hashCode * 31 + qHash(value.audioBitRate());
    hashCode = hashCode * 31 + qHash(value.channelCount());
    hashCode = hashCode * 31 + qHash(value.videoBitRate());
    return hashCode;
}

inline hash_type qHash(const QMediaContent& value){
    if(value.isNull())
        return 0;
    hash_type hashCode = qHash(value.canonicalRequest());
    hashCode = hashCode * 31 + qHash(value.canonicalUrl());
    return hashCode;
}

inline hash_type qHash(const QCameraInfo& value){
    if(value.isNull())
        return 0;
    hash_type hashCode = qHash(value.position());
    hashCode = hashCode * 31 + qHash(value.deviceName());
    hashCode = hashCode * 31 + qHash(value.description());
    hashCode = hashCode * 31 + qHash(value.orientation());
    return hashCode;
}

inline hash_type qHash(const QCameraFocusZone& value){
    hash_type hashCode = qHash(value.area());
    hashCode = hashCode * 31 + qHash(value.status());
    hashCode = hashCode * 31 + qHash(value.isValid());
    return hashCode;
}

inline hash_type qHash(const QCamera::FrameRateRange& value){
    hash_type hashCode = qHash(value.minimumFrameRate);
    hashCode = hashCode * 31 + qHash(value.maximumFrameRate);
    return hashCode;
}

inline hash_type qHash(const QAudioEncoderSettings& value){
    if(value.isNull())
        return 0;
    hash_type hashCode = qHash(value.codec());
    hashCode = hashCode * 31 + qHash(value.bitRate());
    hashCode = hashCode * 31 + qHash(value.quality());
    hashCode = hashCode * 31 + qHash(value.sampleRate());
    hashCode = hashCode * 31 + qHash(value.channelCount());
    hashCode = hashCode * 31 + qHash(value.encodingMode());
    return hashCode;
}

inline hash_type qHash(const QAudioDeviceInfo& value){
    if(value.isNull())
        return 0;
    hash_type hashCode = qHash(value.deviceName());
    hashCode = hashCode * 31 + qHash(value.preferredFormat());
    hashCode = hashCode * 31 + qHash(value.supportedCodecs());
    hashCode = hashCode * 31 + qHash(value.supportedByteOrders());
    hashCode = hashCode * 31 + qHash(value.supportedSampleRates());
    hashCode = hashCode * 31 + qHash(value.supportedChannelCounts());
    hashCode = hashCode * 31 + qHash(value.supportedSampleTypes());
    hashCode = hashCode * 31 + qHash(value.supportedSampleSizes());
    return hashCode;
}
#endif

inline hash_type qHash(const QAudioFormat& value){
    hash_type hashCode = qHash(value.sampleRate());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    hashCode = hashCode * 31 + qHash(value.byteOrder());
    hashCode = hashCode * 31 + qHash(value.sampleSize());
    hashCode = hashCode * 31 + qHash(value.sampleType());
    hashCode = hashCode * 31 + qHash(value.codec());
#endif
    hashCode = hashCode * 31 + qHash(value.channelCount());
    hashCode = hashCode * 31 + qHash(value.bytesPerFrame());
    return hashCode;
}

#endif // QTJAMBI_MULTIMEDIA_H
