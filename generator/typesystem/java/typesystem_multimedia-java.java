/****************************************************************************
**
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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
    @QtBlockedSlot
    public final ContinuousList<Double> supportedFrameRates(org.qtjambi.qt.multimedia.QVideoEncoderSettings settings) {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<Double> supportedFrameRates = supportedFrameRates(settings, continuous);
    	return new ContinuousList<Double>(supportedFrameRates, continuous.booleanValue());
    }

    @QtBlockedSlot
    public final ContinuousList<org.qtjambi.qt.core.QSize> supportedResolutions(org.qtjambi.qt.multimedia.QVideoEncoderSettings settings) {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<org.qtjambi.qt.core.QSize> supportedResolutions = supportedResolutions(settings, continuous);
    	return new ContinuousList<org.qtjambi.qt.core.QSize>(supportedResolutions, continuous.booleanValue());
    }
}// class

class QImageEncoderControl__{
    @QtBlockedSlot
    public final ContinuousList<org.qtjambi.qt.core.QSize> supportedResolutions(org.qtjambi.qt.multimedia.QImageEncoderSettings settings) {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<org.qtjambi.qt.core.QSize> supportedResolutions = supportedResolutions(settings, continuous);
    	return new ContinuousList<org.qtjambi.qt.core.QSize>(supportedResolutions, continuous.booleanValue());
    }
}// class

class QMediaRecorder__{
    @QtBlockedSlot
    public final ContinuousList<Double> supportedFrameRates() {
        return supportedFrameRates(new org.qtjambi.qt.multimedia.QVideoEncoderSettings());
    }

    @QtBlockedSlot
    public final ContinuousList<Double> supportedFrameRates(org.qtjambi.qt.multimedia.QVideoEncoderSettings settings) {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<Double> supportedFrameRates = supportedFrameRates(settings, continuous);
    	return new ContinuousList<Double>(supportedFrameRates, continuous.booleanValue());
    }

    @QtBlockedSlot
    public final ContinuousList<org.qtjambi.qt.core.QSize> supportedResolutions() {
        return supportedResolutions(new org.qtjambi.qt.multimedia.QVideoEncoderSettings());
    }

    @QtBlockedSlot
    public final ContinuousList<org.qtjambi.qt.core.QSize> supportedResolutions(org.qtjambi.qt.multimedia.QVideoEncoderSettings settings) {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<org.qtjambi.qt.core.QSize> supportedResolutions = supportedResolutions(settings, continuous);
    	return new ContinuousList<org.qtjambi.qt.core.QSize>(supportedResolutions, continuous.booleanValue());
    }

    @QtBlockedSlot
    public final ContinuousList<Integer> supportedAudioSampleRates() {
        return supportedAudioSampleRates(new org.qtjambi.qt.multimedia.QAudioEncoderSettings());
    }
    
    @QtBlockedSlot
    public final ContinuousList<Integer> supportedAudioSampleRates(org.qtjambi.qt.multimedia.QAudioEncoderSettings settings)    {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<Integer> supportedAudioSampleRates = supportedAudioSampleRates(settings, continuous);
    	return new ContinuousList<Integer>(supportedAudioSampleRates, continuous.booleanValue());
    }
}// class

class QCameraImageCapture__{
    @QtBlockedSlot
    public final ContinuousList<org.qtjambi.qt.core.QSize> supportedResolutions() {
        return supportedResolutions(new org.qtjambi.qt.multimedia.QImageEncoderSettings());
    }

    @QtBlockedSlot
    public final ContinuousList<org.qtjambi.qt.core.QSize> supportedResolutions(org.qtjambi.qt.multimedia.QImageEncoderSettings settings) {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<org.qtjambi.qt.core.QSize> supportedResolutions = supportedResolutions(settings, continuous);
    	return new ContinuousList<org.qtjambi.qt.core.QSize>(supportedResolutions, continuous.booleanValue());
    }
}// class

class QCameraExposureControl__{
    @QtBlockedSlot
    public final ContinuousList<Object> supportedParameterRange(ExposureParameter settings) {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<Object> supportedParameterRange = supportedParameterRange(settings, continuous);
    	return new ContinuousList<Object>(supportedParameterRange, continuous.booleanValue());
    }
}// class

class QCameraExposure__{
    @QtBlockedSlot
    public final ContinuousList<Integer> supportedIsoSensitivities()    {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<Integer> supportedIsoSensitivities = supportedIsoSensitivities(continuous);
    	return new ContinuousList<Integer>(supportedIsoSensitivities, continuous.booleanValue());
    }
	
	@QtBlockedSlot
    public final ContinuousList<Double> supportedApertures()    {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<Double> supportedApertures = supportedApertures(continuous);
    	return new ContinuousList<Double>(supportedApertures, continuous.booleanValue());
    }
	
	@QtBlockedSlot
    public final ContinuousList<Double> supportedShutterSpeeds()    {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<Double> supportedShutterSpeeds = supportedShutterSpeeds(continuous);
    	return new ContinuousList<Double>(supportedShutterSpeeds, continuous.booleanValue());
    }
}// class

class QAudioEncoderSettingsControl__{
    @QtBlockedSlot
    public final ContinuousList<Integer> supportedSampleRates() {
        return supportedSampleRates(new org.qtjambi.qt.multimedia.QAudioEncoderSettings());
    }

    @QtBlockedSlot
    public final ContinuousList<Integer> supportedSampleRates(org.qtjambi.qt.multimedia.QAudioEncoderSettings settings) {
    	org.qtjambi.qt.QNativePointer continuous = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
    	java.util.List<Integer> supportedSampleRates = supportedSampleRates(settings, continuous);
    	return new ContinuousList<Integer>(supportedSampleRates, continuous.booleanValue());
    }
}// class

