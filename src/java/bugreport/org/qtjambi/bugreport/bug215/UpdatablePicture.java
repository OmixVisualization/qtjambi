package org.qtjambi.bugreport.bug215;

import java.io.File;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt.AlignmentFlag;
import org.qtjambi.qt.core.Qt.AspectRatioMode;
import org.qtjambi.qt.core.Qt.TransformationMode;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QImage.Format;
import org.qtjambi.qt.gui.QLabel;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QResizeEvent;
import org.qtjambi.qt.gui.QSizePolicy.Policy;

class UpdatablePicture extends QLabel {
	private static final int imageBorderPixels = 2;
	private String image = null;
	private final Lock lock = new ReentrantLock();
	private Integer maxHeight = null;
	private Integer maxWidth = null;
	private boolean needUpdate = true;
	private QPixmap pixmapOriginal = null;
	private int rawHeight = 0;
	private int[] rawPixelData;
	private int rawWidth = 0;
	private QSize size = null;

//private QPixmap pixmapSaved = null;
//private QImage imageSaved = null;

	UpdatablePicture(final File initialImage) {
		this(initialImage, null, null);
	}

	UpdatablePicture(final File initialImage, final Integer maxWidth, final Integer maxHeight) {
		image = initialImage.getAbsolutePath();
		this.maxWidth = maxWidth;
		this.maxHeight = maxHeight;
		setAlignment(AlignmentFlag.AlignCenter);
		setSizePolicy(Policy.MinimumExpanding, Policy.MinimumExpanding);
		setMinimumSize(1, 1);
		updatePicture();
	}

	private QSize getTargetSize() {
		if (pixmapOriginal != null) {
			size = pixmapOriginal.size();
			final QSize targetSize = new QSize(Math.min(size.width(), width() - imageBorderPixels), Math.min(size.height(),
				height() - imageBorderPixels));
			size.scale(targetSize, AspectRatioMode.KeepAspectRatio);
		}
		return size;
	}

	void push(final File newImage) {
		final String path = newImage.getAbsolutePath();
		lock.lock();
		needUpdate = !path.equals(image);
		image = path;
		rawPixelData = null;
		lock.unlock();
	}

	void push(final int[] pixelData, final int width, final int height) {
		lock.lock();
		needUpdate = true; // Comparing each pixel would be expensive; assume it did change
		image = null;
		rawPixelData = pixelData;
		rawWidth = width;
		rawHeight = height;
		lock.unlock();
	}

	@Override
	protected void resizeEvent(final QResizeEvent event) {
		lock.lock();
		final QSize targetSize = getTargetSize();
		if (targetSize != null && !targetSize.equals(pixmap().size())) {
			needUpdate = true;
			updatePicture();
		}
		lock.unlock();
	}

	void updatePicture() {
		boolean doGc = false;
		lock.lock();
		if (needUpdate) {
			if (image != null) {
//pixmapSaved = pixmapOriginal;
				pixmapOriginal = new QPixmap(image);
			} else if (rawPixelData != null) {
				final int length = rawPixelData.length;
				int width = rawWidth;
				int height = rawHeight;
				byte[] rgb;
				if ((maxWidth != null && rawWidth > maxWidth) || (maxHeight != null && rawHeight > maxHeight)) {
					final double inverseScale = Math.max((double) width / (double) maxWidth, (double) height
						/ (double) maxHeight);
					final double scale = 1 / inverseScale;
					width = (int) Math.floor(width * scale);
					height = (int) Math.floor(height * scale);
					int index;
					rgb = new byte[width * height * 3];
					for (int i = 0; i < width * height; i++) {
						index = Math.min((int) (inverseScale * (i / width)), rawHeight) * rawWidth
							+ Math.min((int) (inverseScale * (i % width)), rawWidth);
						rgb[i * 3] = (byte) ((rawPixelData[index] & 0xff0000) >> 16);
						rgb[i * 3 + 1] = (byte) ((rawPixelData[index] & 0xff00) >> 8);
						rgb[i * 3 + 2] = (byte) (rawPixelData[index] & 0xff);
					}
				} else {
					rgb = new byte[length * 3];
					for (int i = 0; i < length; i++) {
						rgb[i * 3] = (byte) ((rawPixelData[i] & 0xff0000) >> 16);
						rgb[i * 3 + 1] = (byte) ((rawPixelData[i] & 0xff00) >> 8);
						rgb[i * 3 + 2] = (byte) (rawPixelData[i] & 0xff);
					}
				}
				final QImage finalImage = new QImage(rgb, width, height, Format.Format_RGB888);
				if (!finalImage.isNull()) {
//pixmapSaved = pixmapOriginal;
					pixmapOriginal = QPixmap.fromImage(finalImage);
				}
//imageSaved = finalImage;
				finalImage.dispose();
				rawPixelData = null;
			}
			if (!pixmapOriginal.isNull()) {
				setPixmap(pixmapOriginal.scaled(getTargetSize(), AspectRatioMode.KeepAspectRatio,
					TransformationMode.SmoothTransformation));
			}
			needUpdate = false;
			doGc = true;
		}
		lock.unlock();
		if (doGc) {
			System.gc();
		}
	}
}
