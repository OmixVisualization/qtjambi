import qtjambi.nativewidgets.*;
import io.qt.gui.*;

public class TestJava {

    public static void main(String[] args) {

        QApplication.initialize(args);

		QNativeWidgetTest test = new QNativeWidgetTest();
		test.setLabel("foo");
		test.show();
		
        QApplication.execStatic();
        QApplication.shutdown();

    }
}
