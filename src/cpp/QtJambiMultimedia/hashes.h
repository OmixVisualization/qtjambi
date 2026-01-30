/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

size_t qHash(const QAudioFormat& value, size_t seed = 0);

inline size_t qHash(const QVideoFrame::PaintOptions& value, size_t seed = 0){
    return qHashMulti(seed, value.backgroundColor, int(value.aspectRatioMode));
}

inline bool operator==(const QVideoFrame::PaintOptions& value1, const QVideoFrame::PaintOptions& value2){
    return value1.backgroundColor==value2.backgroundColor && value1.aspectRatioMode==value2.aspectRatioMode;
}

inline size_t qHash(const QVideoFrameFormat& value, size_t seed = 0){
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

inline size_t qHash(const QAudioDevice& value, size_t seed = 0){
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

inline size_t qHash(const QCameraFormat& value, size_t seed = 0){
    if(!value.isNull())
        return seed;
    return qHashMulti(seed, int(value.pixelFormat()),
                            value.resolution(),
                            value.minFrameRate(),
                            value.maxFrameRate());
}

inline size_t qHash(const QCameraDevice& value, size_t seed = 0){
    if(!value.isNull())
        return seed;
    return qHashMulti(seed, value.id(),
                            value.description(),
                            value.isDefault(),
                            int(value.position()),
                            value.photoResolutions(),
                            value.videoFormats());
}

inline size_t qHash(const QMediaTimeRange::Interval& value, size_t seed = 0){
    return qHashMulti(seed, value.start(), value.end(), value.isNormal());
}

inline size_t qHash(const QMediaFormat& value, size_t seed = 0){
    return qHashMulti(seed, value.fileFormat(), int(value.audioCodec()), int(value.videoCodec()));
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
inline size_t qHash(const QPlaybackOptions& value, size_t seed = 0){
    return qHashMulti(seed, value.networkTimeout().count(), int(value.playbackIntent()), value.probeSize());
}
#endif


inline size_t qHash(const QVideoFrame& value, size_t seed = 0){
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
    seed = hash(seed, value.surfaceFormat());
    seed = hash(seed, value.isReadable());
    seed = hash(seed, value.startTime());
    seed = hash(seed, int(value.handleType()));
    seed = hash(seed, value.isReadable());
    seed = hash(seed, value.isWritable());
    seed = hash(seed, int(value.pixelFormat()));
    seed = hash(seed, value.planeCount());
    for(int i=0; i< value.planeCount(); ++i){
        seed = hash(seed, value.mappedBytes(i));
        seed = hash(seed, value.bytesPerLine(i));
        seed = hash(seed, QByteArray(reinterpret_cast<const char*>(value.bits(i)), value.mappedBytes(i)));
    }
    return seed;
}

inline size_t qHash(const QMediaTimeRange& value, size_t seed = 0){
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

inline size_t qHash(const QAudioFormat& value, size_t seed){
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.sampleRate());
    seed = hash(seed, value.channelCount());
    seed = hash(seed, value.bytesPerFrame());
    return seed;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
inline size_t qHash(const QCapturableWindow &value, size_t seed = 0)
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
inline size_t qHash(const QMediaMetaData &value, size_t seed = 0)
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
