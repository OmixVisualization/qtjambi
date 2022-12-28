package io.qt.designer.multimedia;

import io.qt.core.*;
import io.qt.designer.*;
import io.qt.multimedia.widgets.*;
import io.qt.designer.util.*;

public class QMultimediaPlugin extends QObject implements QDesignerCustomWidgetCollectionInterface{
	
	public QMultimediaPlugin() {
		this(null);
	}

	public QMultimediaPlugin(QObject parent) {
		super(parent);
		list.add(new CustomWidgetInterface("MultimediaWidgets", QVideoWidget::new, "QVideoWidget"));
	}

	private final QList<QDesignerCustomWidgetInterface> list = new QList<>(QDesignerCustomWidgetInterface.class);

	@Override
	public QList<QDesignerCustomWidgetInterface> customWidgets() {
		return list.clone();
	}

}
