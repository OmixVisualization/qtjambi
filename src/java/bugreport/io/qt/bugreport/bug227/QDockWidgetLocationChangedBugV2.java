// mkdir build/bugreport
// javac -g -d build/bugreport  -cp qtjambi-4.7.4*.jar -source 1.5 -target 1.5  src/java/bugreport/org/qtjambi/bugreport/bug227/QDockWidgetLocationChangedBugV2.java
// jar -cvf $PWD/bug227.jar -C src/java/bugreport .
// jar -uvf $PWD/bug227.jar -C build/bugreport .
// java -Dio.qt.debug=true -cp qtjambi-4.7.4*.jar:build/qtjambi-linux64-gcc-4.7.4.jar:bug227.jar org/qtjambi/bugreport/bug227/QDockWidgetLocationChangedBugV2

// When a signal is emitted the slot method is not being invoked.

package io.qt.bugreport.bug227;

import io.qt.core.Qt.AlignmentFlag;
import io.qt.core.Qt.DockWidgetArea;
import io.qt.gui.QApplication;
import io.qt.gui.QDockWidget;
import io.qt.gui.QFrame.Shadow;
import io.qt.gui.QFrame.Shape;
import io.qt.gui.QLabel;
import io.qt.gui.QMainWindow;

abstract class MyAbstractDockWidget extends QDockWidget
{
	protected final QLabel label;

	protected MyAbstractDockWidget(String title) {

		super(title);

		setFeatures(DockWidgetFeature.DockWidgetMovable);

		label = new QLabel("Move me!");
		label.setContentsMargins(50, 50, 50, 50);
		label.setAlignment(AlignmentFlag.AlignCenter);

		setWidget(label);

	}

}

class MyBadDockWidget extends MyAbstractDockWidget
{
	private int count;

	public MyBadDockWidget() {

		super("Bad widget");
		dockLocationChanged.connect(this, "dockLocationChanged(io.qt.core.Qt$DockWidgetArea)");

	}

	/**
	 * This won't ever get called.
	 */
	protected void dockLocationChanged(io.qt.core.Qt.DockWidgetArea area)
	{
		count++;
		label.setText("Bad[" + count + "]: You just moved me to the " + area.name());
	}
}

class MyGoodDockWidget extends MyAbstractDockWidget
{
	private int count;

	public MyGoodDockWidget() {

		super("Good widget");
		dockLocationChanged.connect(this, "dockLocationChanged(Object)");

	}

	protected void dockLocationChanged(Object area)
	{
		count++;
		label.setText("Good[ " + count + "]: You just moved me to the " + ((io.qt.core.Qt.DockWidgetArea) area).name());
	}
}

class MyBetterDockWidget extends MyAbstractDockWidget
{
	private int count;

	public MyBetterDockWidget() {

		super("Better widget");
		dockLocationChanged.connect(this, "dockLocationChanged2(io.qt.core.Qt.DockWidgetArea)");

	}

	protected void dockLocationChanged2(io.qt.core.Qt.DockWidgetArea area)
	{
		count++;
		label.setText("Better[" + count + "]: You just moved me to the " + area.name());
	}
}

public class QDockWidgetLocationChangedBugV2 extends QMainWindow {

	public QDockWidgetLocationChangedBugV2() {

		resize(640, 400);

		final QLabel label = new QLabel("Central widget");
		label.setAlignment(AlignmentFlag.AlignCenter);
		label.setFrameShadow(Shadow.Plain);
		label.setFrameShape(Shape.Panel);
		setCentralWidget(label);

		addDockWidget(DockWidgetArea.LeftDockWidgetArea, new MyBadDockWidget());
		addDockWidget(DockWidgetArea.RightDockWidgetArea, new MyGoodDockWidget());
		addDockWidget(DockWidgetArea.TopDockWidgetArea, new MyBetterDockWidget());

		show();

	}

	public static void doit() {

		QDockWidgetLocationChangedBugV2 w = new QDockWidgetLocationChangedBugV2();

		QApplication.execStatic();
	}

	public static void main(String[] args) {

		QApplication.initialize(args);

		doit();

	}

}
