package io.qt.designer.customwidgets;

import io.qt.core.QList;
import io.qt.core.QObject;
import io.qt.designer.QDesignerCustomWidgetCollectionInterface;
import io.qt.designer.QDesignerCustomWidgetInterface;
import io.qt.designer.util.*;

public class QCustomWidgetsPlugin extends QObject implements QDesignerCustomWidgetCollectionInterface{
	
	public QCustomWidgetsPlugin() {
		this(null);
	}

	public QCustomWidgetsPlugin(QObject parent) {
		super(parent);
		list.add(new CustomWidgetInterface("CustomWidgets", CustomWidget::new));
	}

	private final QList<QDesignerCustomWidgetInterface> list = new QList<>(QDesignerCustomWidgetInterface.class);

	@Override
	public QList<QDesignerCustomWidgetInterface> customWidgets() {
		return list.clone();
	}

}
