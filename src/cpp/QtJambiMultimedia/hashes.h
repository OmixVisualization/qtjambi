/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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

#ifndef QTJAMBIMULTIMEDIA_HASHES_H
#define QTJAMBIMULTIMEDIA_HASHES_H

#include <QtCore/QtCore>
#include <QtMultimedia/QtMultimedia>
#include <QtJambi/Global>
#include <QtJambiGui/hashes.h>
#include <QtJambiNetwork/hashes.h>
#if !defined(QTJAMBI_GENERATOR_RUNNING)
#include <QtJambi/CoreAPI>
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

hash_type qHash(const QAudioFormat& value, hash_type seed = 0);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QVideoFrame::PaintOptions& value, hash_type seed = 0){
    return qHashMulti(seed, value.backgroundColor, int(value.aspectRatioMode));
}

inline bool operator==(const QVideoFrame::PaintOptions& value1, const QVideoFrame::PaintOptions& value2){
    return value1.backgroundColor==value2.backgroundColor && value1.aspectRatioMode==value2.aspectRatioMode;
}

inline hash_type qHash(const QVideoFrameFormat& value, hash_type seed = 0){
    if(!value.isValid())
        return seed;
    return qHashMulti(seed, int(value.pixelFormat()), value.frameWidth(),
                        value.frameHeight(), value.planeCount(), value.viewport(),
                        int(value.scanLineDirection()),
#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
                        int(value.frameRate()),
#else
                      int(value.streamFrameRate()),
#endif
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
                        int(value.yCbCrColorSpace()),
#else
                      int(value.colorSpace()),
#endif
                        value.isMirrored(),
                        value.vertexShaderFileName(),
                        value.fragmentShaderFileName());
}

inline hash_type qHash(const QAudioDevice& value, hash_type seed = 0){
    if(!value.isNull())
        return seed;
    return qHashMulti(seed, value.id(),
                            value.description(),
                            value.isDefault(),
                            int(value.mode()),
                            value.preferredFormat(),
                            value.minimumSampleRate(),
                            value.maximumSampleRate(),
                            value.minimumChannelCount(),
                            value.maximumChannelCount(),
                            value.supportedSampleFormats());
}

inline hash_type qHash(const QCameraFormat& value, hash_type seed = 0){
    if(!value.isNull())
        return seed;
    return qHashMulti(seed, int(value.pixelFormat()),
                            value.resolution(),
                            value.minFrameRate(),
                            value.maxFrameRate());
}

inline hash_type qHash(const QCameraDevice& value, hash_type seed = 0){
    if(!value.isNull())
        return seed;
    return qHashMulti(seed, value.id(),
                            value.description(),
                            value.isDefault(),
                            int(value.position()),
                            value.photoResolutions(),
                            value.videoFormats());
}

inline hash_type qHash(const QMediaTimeRange::Interval& value, hash_type seed = 0){
    return qHashMulti(seed, value.start(), value.end(), value.isNormal());
}

inline hash_type qHash(const QMediaFormat& value, hash_type seed = 0){
    return qHashMulti(seed, value.fileFormat(), int(value.audioCodec()), int(value.videoCodec()));
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
inline hash_type qHash(const QPlaybackOptions& value, hash_type seed = 0){
    return qHashMulti(seed, value.networkTimeout().count(), int(value.playbackIntent()), value.probeSize());
}
#endif
#endif


inline hash_type qHash(const QVideoFrame& value, hash_type seed = 0){
    if(!value.isValid())
        return seed;
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombineCommutative hash;
#else
    QtPrivate::QHashCombineCommutative hash(seed);
#endif
    seed = hash(seed, value.size());
    seed = hash(seed, value.width());
    seed = hash(seed, value.height());
    seed = hash(seed, value.endTime());
    seed = hash(seed, int(value.mapMode()));
    seed = hash(seed, value.isMapped());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    seed = hash(seed, value.fieldType());
#else
    seed = hash(seed, value.surfaceFormat());
    seed = hash(seed, value.isReadable());
#endif
    seed = hash(seed, value.startTime());
    seed = hash(seed, int(value.handleType()));
    seed = hash(seed, value.isReadable());
    seed = hash(seed, value.isWritable());
    seed = hash(seed, int(value.pixelFormat()));
    seed = hash(seed, value.planeCount());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    seed = hash(seed, value.mappedBytes());
    seed = hash(seed, value.bytesPerLine());
    seed = hash(seed, QByteArray(reinterpret_cast<const char*>(value.bits()), value.mappedBytes()));
#else
    for(int i=0; i< value.planeCount(); ++i){
        seed = hash(seed, value.mappedBytes(i));
        seed = hash(seed, value.bytesPerLine(i));
        seed = hash(seed, QByteArray(reinterpret_cast<const char*>(value.bits(i)), value.mappedBytes(i)));
    }
#endif
    return seed;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QVideoEncoderSettings& value, hash_type seed = 0){
    if(value.isNull())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.encodingMode());
    seed = hash(seed, value.codec());
    seed = hash(seed, value.bitRate());
    seed = hash(seed, value.quality());
    seed = hash(seed, value.frameRate());
    seed = hash(seed, value.resolution());
    return seed;
}

inline hash_type qHash(const QImageEncoderSettings& value, hash_type seed = 0){
    if(value.isNull())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.quality());
    seed = hash(seed, value.codec());
    seed = hash(seed, value.resolution());
    return seed;
}

inline hash_type qHash(const QVideoSurfaceFormat& value, hash_type seed = 0){
    return qHash(quintptr(reinterpret_cast<const QSharedDataPointer<QSharedData>&>(value).data()), seed);
}

inline hash_type qHash(const QCameraViewfinderSettings& value, hash_type seed = 0){
    if(value.isNull())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.resolution());
    seed = hash(seed, value.minimumFrameRate());
    seed = hash(seed, value.maximumFrameRate());
    seed = hash(seed, value.pixelFormat());
    seed = hash(seed, value.pixelAspectRatio());
    return seed;
}
#endif

inline hash_type qHash(const QMediaTimeRange& value, hash_type seed = 0){
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.isEmpty());
    seed = hash(seed, value.intervals());
    seed = hash(seed, value.latestTime());
    seed = hash(seed, value.earliestTime());
    seed = hash(seed, value.isContinuous());
    return seed;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QMediaTimeInterval& value, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.start());
    seed = hash(seed, value.end());
    seed = hash(seed, value.isNormal());
    return seed;
}

inline hash_type qHash(const QMediaServiceProviderHint& value, hash_type seed = 0){
    if(value.isNull())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.type());
    seed = hash(seed, value.codecs());
    seed = hash(seed, value.device());
    seed = hash(seed, int(value.features()));
    seed = hash(seed, value.mimeType());
    seed = hash(seed, value.cameraPosition());
    return seed;
}

inline hash_type qHash(const QMediaResource& value, hash_type seed = 0){
    if(value.isNull())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.url());
    seed = hash(seed, value.request());
    seed = hash(seed, value.dataSize());
    seed = hash(seed, value.language());
    seed = hash(seed, value.mimeType());
    seed = hash(seed, value.audioCodec());
    seed = hash(seed, value.resolution());
    seed = hash(seed, value.sampleRate());
    seed = hash(seed, value.videoCodec());
    seed = hash(seed, value.audioBitRate());
    seed = hash(seed, value.channelCount());
    seed = hash(seed, value.videoBitRate());
    return seed;
}

inline hash_type qHash(const QMediaContent& value, hash_type seed = 0){
    if(value.isNull())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.canonicalRequest());
    seed = hash(seed, value.canonicalUrl());
    return seed;
}

inline hash_type qHash(const QCameraInfo& value, hash_type seed = 0){
    if(value.isNull())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.position());
    seed = hash(seed, value.deviceName());
    seed = hash(seed, value.description());
    seed = hash(seed, value.orientation());
    return seed;
}

inline hash_type qHash(const QCameraFocusZone& value, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.area());
    seed = hash(seed, value.status());
    seed = hash(seed, value.isValid());
    return seed;
}

inline hash_type qHash(const QCamera::FrameRateRange& value, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.minimumFrameRate);
    seed = hash(seed, value.maximumFrameRate);
    return seed;
}

inline hash_type qHash(const QAudioEncoderSettings& value, hash_type seed = 0){
    if(value.isNull())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.codec());
    seed = hash(seed, value.bitRate());
    seed = hash(seed, value.quality());
    seed = hash(seed, value.sampleRate());
    seed = hash(seed, value.channelCount());
    seed = hash(seed, value.encodingMode());
    return seed;
}

inline hash_type qHash(const QAudioDeviceInfo& value, hash_type seed = 0){
    if(value.isNull())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.deviceName());
    seed = hash(seed, value.preferredFormat());
    seed = hash(seed, value.supportedCodecs());
    seed = hash(seed, value.supportedByteOrders());
    seed = hash(seed, value.supportedSampleRates());
    seed = hash(seed, value.supportedChannelCounts());
    seed = hash(seed, value.supportedSampleTypes());
    seed = hash(seed, value.supportedSampleSizes());
    return seed;
}
#endif

inline hash_type qHash(const QAudioFormat& value, hash_type seed){
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.sampleRate());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    seed = hash(seed, value.byteOrder());
    seed = hash(seed, value.sampleSize());
    seed = hash(seed, value.sampleType());
    seed = hash(seed, value.codec());
#endif
    seed = hash(seed, value.channelCount());
    seed = hash(seed, value.bytesPerFrame());
    return seed;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
inline hash_type qHash(const QCapturableWindow &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.description());
    return seed;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 1) && defined(QTJAMBI_GENERATOR_RUNNING)
using QtAudio::Error = QAudio::Error;
using QtAudio::State = QAudio::State;
#endif

#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
inline hash_type qHash(const QMediaMetaData &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    for(QMediaMetaData::Key key : value.keys()){
        QVariant v = value.value(key);
        seed = hash(seed, CoreAPI::computeHash(v.metaType(), v.constData(), 0));
    }
    return seed;
}
#endif //QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)

#endif // QTJAMBIMULTIMEDIA_HASHES_H
