package io.qt.designer.help;

import io.qt.core.*;
import io.qt.designer.*;
import io.qt.designer.util.*;
import io.qt.help.*;

public class QHelpWidgetsPlugin extends QObject implements QDesignerCustomWidgetCollectionInterface{

	public QHelpWidgetsPlugin() {
		this(null);
	}

	public QHelpWidgetsPlugin(QObject parent) {
		super(parent);
		list.add(new CustomWidgetInterface("HelpWidgets", QHelpSearchQueryWidget::new, "QHelpSearchQueryWidget"));
		list.add(new CustomWidgetInterface("HelpWidgets", QHelpFilterSettingsWidget::new, "QHelpFilterSettingsWidget"));
	}

	private final QList<QDesignerCustomWidgetInterface> list = new QList<>(QDesignerCustomWidgetInterface.class);

	@Override
	public QList<QDesignerCustomWidgetInterface> customWidgets() {
		return list.clone();
	}

}
