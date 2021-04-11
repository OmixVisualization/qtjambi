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

#include <QtCore/QtCore>
#include <QtMultimedia/QtMultimedia>
#include <qtjambi_core/qtjambi_core_qhashes.h>
#include <qtjambi_network/qtjambi_network.h>

inline hash_type qHash(const QVideoFrame& value){
    if(!value.isValid())
        return 0;
    hash_type hashCode = qHash(value.size());
    hashCode = hashCode * 31 + qHash(value.width());
    hashCode = hashCode * 31 + qHash(value.height());
    hashCode = hashCode * 31 + qHash(value.endTime());
    hashCode = hashCode * 31 + qHash(value.mapMode());
    hashCode = hashCode * 31 + qHash(value.isMapped());
    hashCode = hashCode * 31 + qHash(value.fieldType());
    hashCode = hashCode * 31 + qHash(value.startTime());
    hashCode = hashCode * 31 + qHash(value.handleType());
    hashCode = hashCode * 31 + qHash(value.isReadable());
    hashCode = hashCode * 31 + qHash(value.isWritable());
    hashCode = hashCode * 31 + qHash(value.planeCount());
    hashCode = hashCode * 31 + qHash(value.mappedBytes());
    hashCode = hashCode * 31 + qHash(value.pixelFormat());
    hashCode = hashCode * 31 + qHash(value.bytesPerLine());
    hashCode = hashCode * 31 + qHash(QByteArray(reinterpret_cast<const char*>(value.bits()), value.mappedBytes()));
    return hashCode;
}

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

inline hash_type qHash(const QMediaTimeRange& value){
    hash_type hashCode = qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.intervals());
    hashCode = hashCode * 31 + qHash(value.latestTime());
    hashCode = hashCode * 31 + qHash(value.earliestTime());
    hashCode = hashCode * 31 + qHash(value.isContinuous());
    return hashCode;
}

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

inline hash_type qHash(const QCamera& value){
    hash_type hashCode = qHash(value.error());
    hashCode = hashCode * 31 + qHash(value.focus());
    hashCode = hashCode * 31 + qHash(value.state());
    hashCode = hashCode * 31 + qHash(value.status());
    hashCode = hashCode * 31 + qHash(value.exposure());
    hashCode = hashCode * 31 + qHash(value.lockStatus());
    hashCode = hashCode * 31 + qHash(value.captureMode());
    hashCode = hashCode * 31 + qHash(value.errorString());
    hashCode = hashCode * 31 + qHash(value.availability());
    hashCode = hashCode * 31 + qHash(value.requestedLocks());
    hashCode = hashCode * 31 + qHash(value.supportedLocks());
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

inline hash_type qHash(const QAudioFormat& value){
    hash_type hashCode = qHash(value.codec());
    hashCode = hashCode * 31 + qHash(value.byteOrder());
    hashCode = hashCode * 31 + qHash(value.sampleRate());
    hashCode = hashCode * 31 + qHash(value.sampleSize());
    hashCode = hashCode * 31 + qHash(value.sampleType());
    hashCode = hashCode * 31 + qHash(value.channelCount());
    hashCode = hashCode * 31 + qHash(value.bytesPerFrame());
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

#endif // QTJAMBI_MULTIMEDIA_H
