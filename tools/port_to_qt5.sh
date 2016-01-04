#!/bin/sh

QT_WIDGETS="MacOSXFunctions \
QAbstractButton \
QAbstractGraphicsShapeItem \
QAbstractItemDelegate \
QAbstractItemView \
QAbstractScrollArea \
QAbstractSlider \
QAbstractSpinBox \
QAccessibleWidget \
QAction \
QActionGroup \
QApplication \
QBoxLayout \
QButtonGroup \
QCalendarWidget \
QCheckBox \
QColorDialog \
QColormap \
QColumnView \
QComboBox \
QCommandLinkButton \
QCommonStyle \
QCompleter \
QDataWidgetMapper \
QDateEdit \
QDateTimeEdit \
QDesktopWidget \
QDial \
QDialog \
QDialogButtonBox \
QDockWidget \
QDoubleSpinBox \
QErrorMessage \
QFileDialog \
QFileIconProvider \
QFileSystemModel \
QFocusFrame \
QFontComboBox \
QFontDialog \
QFormLayout \
QFrame \
QGesture \
QGestureEvent \
QGestureRecognizer \
QGraphicsAnchor \
QGraphicsAnchorLayout \
QGraphicsBlurEffect \
QGraphicsColorizeEffect \
QGraphicsDropShadowEffect \
QGraphicsEffect \
QGraphicsEllipseItem \
QGraphicsGridLayout \
QGraphicsItem \
QGraphicsItemGroup \
QGraphicsLayout \
QGraphicsLayoutItem \
QGraphicsLineItem \
QGraphicsLinearLayout \
QGraphicsObject \
QGraphicsOpacityEffect \
QGraphicsPathItem \
QGraphicsPixmapItem \
QGraphicsPolygonItem \
QGraphicsProxyWidget \
QGraphicsRectItem \
QGraphicsRotation \
QGraphicsScale \
QGraphicsScene \
QGraphicsSceneContextMenuEvent \
QGraphicsSceneDragDropEvent \
QGraphicsSceneEvent \
QGraphicsSceneHelpEvent \
QGraphicsSceneHoverEvent \
QGraphicsSceneMouseEvent \
QGraphicsSceneMoveEvent \
QGraphicsSceneResizeEvent \
QGraphicsSceneWheelEvent \
QGraphicsSimpleTextItem \
QGraphicsTextItem \
QGraphicsTransform \
QGraphicsView \
QGraphicsWidget \
QGridLayout \
QGroupBox \
QHBoxLayout \
QHeaderView \
QInputDialog \
QItemDelegate \
QItemEditorCreator \
QItemEditorCreatorBase \
QItemEditorFactory \
QKeyEventTransition \
QKeySequenceEdit \
QLCDNumber \
QLabel \
QLayout \
QLayoutItem \
QLineEdit \
QListView \
QListWidget \
QListWidgetItem \
QMacCocoaViewContainer \
QMacNativeWidget \
QMainWindow \
QMdiArea \
QMdiSubWindow \
QMenu \
QMenuBar \
QMessageBox \
QMouseEventTransition \
QOpenGLWidget \
QPanGesture \
QPinchGesture \
QPlainTextDocumentLayout \
QPlainTextEdit \
QProgressBar \
QProgressDialog \
QProxyStyle \
QPushButton \
QRadioButton \
QRubberBand \
QScrollArea \
QScrollBar \
QScroller \
QScrollerProperties \
QShortcut \
QSizeGrip \
QSizePolicy \
QSlider \
QSpacerItem \
QSpinBox \
QSplashScreen \
QSplitter \
QSplitterHandle \
QStackedLayout \
QStackedWidget \
QStandardItemEditorCreator \
QStatusBar \
QStyle \
QStyleFactory \
QStyleHintReturn \
QStyleHintReturnMask \
QStyleHintReturnVariant \
QStyleOption \
QStyleOptionButton \
QStyleOptionComboBox \
QStyleOptionComplex \
QStyleOptionDockWidget \
QStyleOptionFocusRect \
QStyleOptionFrame \
QStyleOptionGraphicsItem \
QStyleOptionGroupBox \
QStyleOptionHeader \
QStyleOptionMenuItem \
QStyleOptionProgressBar \
QStyleOptionRubberBand \
QStyleOptionSizeGrip \
QStyleOptionSlider \
QStyleOptionSpinBox \
QStyleOptionTab \
QStyleOptionTabBarBase \
QStyleOptionTabWidgetFrame \
QStyleOptionTitleBar \
QStyleOptionToolBar \
QStyleOptionToolBox \
QStyleOptionToolButton \
QStyleOptionViewItem \
QStylePainter \
QStylePlugin \
QStyledItemDelegate \
QSwipeGesture \
QSystemTrayIcon \
QTabBar \
QTabWidget \
QTableView \
QTableWidget \
QTableWidgetItem \
QTableWidgetSelectionRange \
QTapAndHoldGesture \
QTapGesture \
QTextBrowser \
QTextEdit \
QTileRules \
QTimeEdit \
QToolBar \
QToolBox \
QToolButton \
QToolTip \
QTreeView \
QTreeWidget \
QTreeWidgetItem \
QTreeWidgetItemIterator \
QUndoCommand \
QUndoGroup \
QUndoStack \
QUndoView \
QVBoxLayout \
QWhatsThis \
QWidget \
QWidgetAction \
QWidgetItem \
QWizard \
QWizardPage"

QT_PRINTSUPPORT="QAbstractPrintDialog \
QPageSetupDialog \
QPrintDialog \
QPrintEngine \
QPrintPreviewDialog \
QPrintPreviewWidget \
QPrinter \
QPrinterInfo"

for CLS in $QT_WIDGETS
do
	if [[ -z "$QT_WIDGETS_CLASSES" ]];
	then
		QT_WIDGETS_CLASSES="$CLS"
	else
		QT_WIDGETS_CLASSES="$QT_WIDGETS_CLASSES\|$CLS"
	fi
done

for CLS in $QT_PRINTSUPPORT
do
	if [[ -z "$QT_PRINTSUPPORT_CLASSES" ]];
	then
		QT_PRINTSUPPORT_CLASSES="$CLS"
	else
		QT_PRINTSUPPORT_CLASSES="$QT_PRINTSUPPORT_CLASSES\|$CLS"
	fi
done

for PATH in $@
	do
	for file in $PATH/*.java
	do
		if [[ -f $file ]]; then
			echo "replace on $file"
			/usr/bin/sed -i 's/com\.trolltech\.qt/org.qtjambi.qt/g' $file
			/usr/bin/sed -i 's/com_trolltech_qt/org_qtjambi_qt/g' $file
			/usr/bin/sed -i 's_com/trolltech/qt_org/qtjambi/qt_g' $file
			/usr/bin/sed -i "s/org\.qtjambi\.qt\.gui\.\($QT_WIDGETS_CLASSES\)/org.qtjambi.qt.widgets.\1/g" $file
			/usr/bin/sed -i "s/org\.qtjambi\.qt\.gui\.\($QT_PRINTSUPPORT_CLASSES\)/org.qtjambi.qt.printsupport.\1/g" $file
			/usr/bin/sed -i 's/org\.qtjambi\.qt\.gui\.\(QItemSelection\|QItemSelectionModel\|QSortFilterProxyModel\|QMarginsF\|QAbstractProxyModel\)/org.qtjambi.qt.core.\1/g' $file
			/usr/bin/sed -i 's/QStyleOptionFrameV2\|QStyleOptionFrameV3/QStyleOptionFrame/g' $file
			/usr/bin/sed -i 's/QInputDialog\.getInteger/QInputDialog.getInt/g' $file
			/usr/bin/sed -i 's/QDesktopServices\.storageLocation/QStandardPaths.writableLocation/g' $file
			/usr/bin/sed -i 's/QDesktopServices\.displayName/QStandardPaths.displayName/g' $file
			/usr/bin/sed -i 's/QDesktopServices\.StandardLocation/QStandardPaths.StandardLocation/g' $file
			/usr/bin/sed -i 's/QPrinter\.PageSize/QPagedPaintDevice.PageSize/g' $file
			/usr/bin/sed -i 's/import\sorg\.qtjambi\.qt\.gui\.QDesktopServices;/import org.qtjambi.qt.gui.QDesktopServices;\nimport org.qtjambi.qt.core.QStandardPaths;/g' $file
		fi
	done
	for file in $PATH/*
	do
		if [[ -d $file ]]; then
			$0 $file
		fi
	done
done
