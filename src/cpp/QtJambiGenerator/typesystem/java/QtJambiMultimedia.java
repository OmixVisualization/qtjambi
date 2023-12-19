/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

class QVideoEncoderSettingsControl__{
    @QtUninvokable
    public final @NonNull ContinuousList<@NonNull Double> supportedFrameRates(io.qt.multimedia.@NonNull QVideoEncoderSettings settings) {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<Double> supportedFrameRates = supportedFrameRates(settings, continuous);
        return new ContinuousList<Double>(supportedFrameRates, continuous.booleanValue());
    }

    @QtUninvokable
    public final @NonNull ContinuousList<io.qt.core.@NonNull QSize> supportedResolutions(io.qt.multimedia.@NonNull QVideoEncoderSettings settings) {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<io.qt.core.QSize> supportedResolutions = supportedResolutions(settings, continuous);
        return new ContinuousList<io.qt.core.QSize>(supportedResolutions, continuous.booleanValue());
    }
}// class

class QImageEncoderControl__{
    @QtUninvokable
    public final ContinuousList<io.qt.core.QSize> supportedResolutions(io.qt.multimedia.@NonNull QImageEncoderSettings settings) {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<io.qt.core.QSize> supportedResolutions = supportedResolutions(settings, continuous);
        return new ContinuousList<io.qt.core.QSize>(supportedResolutions, continuous.booleanValue());
    }
}// class

class QMediaRecorder__{
    @QtUninvokable
    public final @NonNull ContinuousList<@NonNull Double> supportedFrameRates() {
        return supportedFrameRates(new io.qt.multimedia.QVideoEncoderSettings());
    }

    @QtUninvokable
    public final @NonNull ContinuousList<@NonNull Double> supportedFrameRates(io.qt.multimedia.@NonNull QVideoEncoderSettings settings) {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<Double> supportedFrameRates = supportedFrameRates(settings, continuous);
        return new ContinuousList<Double>(supportedFrameRates, continuous.booleanValue());
    }

    @QtUninvokable
    public final @NonNull ContinuousList<io.qt.core.@NonNull QSize> supportedResolutions() {
        return supportedResolutions(new io.qt.multimedia.QVideoEncoderSettings());
    }

    @QtUninvokable
    public final @NonNull ContinuousList<io.qt.core.@NonNull QSize> supportedResolutions(io.qt.multimedia.@NonNull QVideoEncoderSettings settings) {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<io.qt.core.QSize> supportedResolutions = supportedResolutions(settings, continuous);
        return new ContinuousList<io.qt.core.QSize>(supportedResolutions, continuous.booleanValue());
    }

    @QtUninvokable
    public final @NonNull ContinuousList<@NonNull Integer> supportedAudioSampleRates() {
        return supportedAudioSampleRates(new io.qt.multimedia.QAudioEncoderSettings());
    }
    
    @QtUninvokable
    public final @NonNull ContinuousList<@NonNull Integer> supportedAudioSampleRates(io.qt.multimedia.@NonNull QAudioEncoderSettings settings)    {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<Integer> supportedAudioSampleRates = supportedAudioSampleRates(settings, continuous);
        return new ContinuousList<Integer>(supportedAudioSampleRates, continuous.booleanValue());
    }
}// class

class QCameraImageCapture__{
    @QtUninvokable
    public final @NonNull ContinuousList<io.qt.core.@NonNull QSize> supportedResolutions() {
        return supportedResolutions(new io.qt.multimedia.QImageEncoderSettings());
    }

    @QtUninvokable
    public final @NonNull ContinuousList<io.qt.core.@NonNull QSize> supportedResolutions(io.qt.multimedia.@NonNull QImageEncoderSettings settings) {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<io.qt.core.QSize> supportedResolutions = supportedResolutions(settings, continuous);
        return new ContinuousList<io.qt.core.QSize>(supportedResolutions, continuous.booleanValue());
    }
}// class

class QCameraExposureControl__{
    @QtUninvokable
    public final @NonNull ContinuousList<Object> supportedParameterRange(ExposureParameter settings) {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<Object> supportedParameterRange = supportedParameterRange(settings, continuous);
        return new ContinuousList<Object>(supportedParameterRange, continuous.booleanValue());
    }
}// class

class QCameraExposure__{
    @QtUninvokable
    public final @NonNull ContinuousList<@NonNull Integer> supportedIsoSensitivities()    {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<Integer> supportedIsoSensitivities = supportedIsoSensitivities(continuous);
        return new ContinuousList<Integer>(supportedIsoSensitivities, continuous.booleanValue());
    }
    
    @QtUninvokable
    public final @NonNull ContinuousList<@NonNull Double> supportedApertures()    {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<Double> supportedApertures = supportedApertures(continuous);
        return new ContinuousList<Double>(supportedApertures, continuous.booleanValue());
    }
    
    @QtUninvokable
    public final @NonNull ContinuousList<@NonNull Double> supportedShutterSpeeds()    {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<Double> supportedShutterSpeeds = supportedShutterSpeeds(continuous);
        return new ContinuousList<Double>(supportedShutterSpeeds, continuous.booleanValue());
    }
}// class

class QAudioEncoderSettingsControl__{
    @QtUninvokable
    public final @NonNull ContinuousList<@NonNull Integer> supportedSampleRates() {
        return supportedSampleRates(new io.qt.multimedia.QAudioEncoderSettings());
    }

    @QtUninvokable
    public final @NonNull ContinuousList<@NonNull Integer> supportedSampleRates(io.qt.multimedia.QAudioEncoderSettings settings) {
        QNativePointer continuous = new QNativePointer(QNativePointer.Type.Boolean);
        java.util.List<Integer> supportedSampleRates = supportedSampleRates(settings, continuous);
        return new ContinuousList<Integer>(supportedSampleRates, continuous.booleanValue());
    }
}// class

class QAbstractVideoBuffer__{
    public static final class MapResult{
        public MapResult(java.nio.@NonNull ByteBuffer data, int bytesPerLine){
            if(!data.isDirect()){
                throw new IllegalArgumentException("Direct buffer expected but given buffer is indirect.");
            }
            this.data = data;
            this.bytesPerLine = bytesPerLine;
        }
        
        @NativeAccess
        private MapResult(java.nio.ByteBuffer data, int bytesPerLine, boolean unused){
            this.data = data;
            this.bytesPerLine = bytesPerLine;
        }
        
        public final java.nio.ByteBuffer data;
        public final int bytesPerLine;
    }
}// class

class QMediaServiceProviderFactoryInterface_java__{

    /**
     * Service with support for media playback
     * Required Controls: QMediaPlayerControl
     * Optional Controls: QMediaPlaylistControl, QAudioDeviceControl
     * Video Output Controls (used by QWideoWidget and QGraphicsVideoItem):
     *                     Required: QVideoOutputControl
     *                     Optional: QVideoWindowControl, QVideoRendererControl, QVideoWidgetControl
     */
    public static @NonNull String Q_MEDIASERVICE_MEDIAPLAYER = "org.qt-project.qt.mediaplayer";

    /**
     * Service with support for recording from audio sources
     * Required Controls: QAudioDeviceControl
     * Recording Controls (QMediaRecorder):
     *                      Required: QMediaRecorderControl
     *                      Recommended: QAudioEncoderSettingsControl
     *                      Optional: QMediaContainerControl
     */
    public static @NonNull String Q_MEDIASERVICE_AUDIOSOURCE = "org.qt-project.qt.audiosource";

    /**
     * Service with support for camera use.
     * Required Controls: QCameraControl
     * Optional Controls: QCameraExposureControl, QCameraFocusControl, QCameraImageProcessingControl
     * Still Capture Controls: QCameraImageCaptureControl
     * Video Capture Controls (QMediaRecorder):
     *                      Required: QMediaRecorderControl
     *                      Recommended: QAudioEncoderSettingsControl, QVideoEncoderSettingsControl, QMediaContainerControl
     * Viewfinder Video Output Controls (used by QCameraViewfinder and QGraphicsVideoItem):
     *                      Required: QVideoOutputControl
     *                      Optional: QVideoWindowControl, QVideoRendererControl, QVideoWidgetControl
     */
    public static @NonNull String Q_MEDIASERVICE_CAMERA = "org.qt-project.qt.camera";

    /**
     * Service with support for radio tuning.
     * Required Controls: QRadioTunerControl
     * Recording Controls (Optional, used by QMediaRecorder):
     *                      Required: QMediaRecorderControl
     *                      Recommended: QAudioEncoderSettingsControl
     *                      Optional: QMediaContainerControl
     */
    public static @NonNull String Q_MEDIASERVICE_RADIO = "org.qt-project.qt.radio";

    /**
     * Service with support for decoding audio.
     * Required Controls: QAudioDecoderControl
     * Optional: that streams control
     */
    public static @NonNull String Q_MEDIASERVICE_AUDIODECODER = "org.qt-project.qt.audiodecode";
    
}// class


class QMediaService___{
    public static class Result<T>{
        Result(T result, boolean continuous) {
            super();
            this.result = result;
            this.continuous = continuous;
        }
        public final T result;
        public final boolean continuous;
    }

    public static class ListResult<T> extends Result<io.qt.core.@NonNull QList<T>> implements java.lang.Iterable<T>{
        ListResult(io.qt.core.QList<T> result, boolean continuous) {
            super(result, continuous);
        }

        @Override
        public java.util.@NonNull Iterator<T> iterator() {
            return result.iterator();
        }
    }
}// class

class QAudioFormat__{
    public static @NonNull ChannelConfig channelConfig(@NonNull AudioChannelPosition @NonNull... channels) {
        int value = 0;
        for (AudioChannelPosition channel : channels) {
            value |= (0x01 << channel.value());
        }
        return ChannelConfig.resolve(value);
    }
}// class

class QAudioBuffer__6_{

    public final @NonNull QAudioFrame.AsByte dataAsU8M(){
        return QAudioFrame.dataAsAudioFrame(dataAsBuffer(), QAudioFormat.ChannelConfig.ChannelConfigMono);
    }

    public final @NonNull QAudioFrame.AsShort dataAsS16M(){
        return QAudioFrame.dataAsAudioFrame(dataAsBuffer().asShortBuffer(), QAudioFormat.ChannelConfig.ChannelConfigMono);
    }

    public final @NonNull QAudioFrame.AsInt dataAsS32M(){
        return QAudioFrame.dataAsAudioFrame(dataAsBuffer().asIntBuffer(), QAudioFormat.ChannelConfig.ChannelConfigMono);
    }

    public final @NonNull QAudioFrame.AsFloat dataAsF32M(){
        return QAudioFrame.dataAsAudioFrame(dataAsBuffer().asFloatBuffer(), QAudioFormat.ChannelConfig.ChannelConfigMono);
    }

    public final @NonNull QAudioFrame.AsByte dataAsU8S(){
        return QAudioFrame.dataAsAudioFrame(dataAsBuffer(), QAudioFormat.ChannelConfig.ChannelConfigStereo);
    }

    public final @NonNull QAudioFrame.AsShort dataAsS16S(){
        return QAudioFrame.dataAsAudioFrame(dataAsBuffer().asShortBuffer(), QAudioFormat.ChannelConfig.ChannelConfigStereo);
    }

    public final @NonNull QAudioFrame.AsInt dataAsS32S(){
        return QAudioFrame.dataAsAudioFrame(dataAsBuffer().asIntBuffer(), QAudioFormat.ChannelConfig.ChannelConfigStereo);
    }

    public final @NonNull QAudioFrame.AsFloat dataAsF32S(){
        return QAudioFrame.dataAsAudioFrame(dataAsBuffer().asFloatBuffer(), QAudioFormat.ChannelConfig.ChannelConfigStereo);
    }
}// class

class QCamera__{
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qcamera.html#setViewfinder">QCamera::setViewfinder(QVideoWidget*)</a></p>
     */
    @QtUninvokable
    public final <T extends io.qt.core.QObject & io.qt.multimedia.QMediaBindableInterface> void setViewfinder(@Nullable T viewfinder){
        setViewfinderImpl((io.qt.core.QObject)viewfinder);
    }
    
}// class

class QMediaPlayer__{
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qmediaplayer.html#setVideoOutput">QMediaPlayer::setVideoOutput(QVideoWidget*)</a></p>
     */
    @QtUninvokable
    public final <T extends io.qt.core.QObject & io.qt.multimedia.QMediaBindableInterface> void setVideoOutput(@Nullable T arg__1){
        setVideoOutputImpl(arg__1);
    }
    
}// class
