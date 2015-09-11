import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class SignalsSlotsExample extends QWidget
{
    public SignalsSlotsExample()
    {
        System.err.println("Yes we are here");
//! [0]
        Counter a, b;
        a = new Counter();
        b = new Counter();

        a.valueChanged.connect(b, "setValue(int)");
        a.setValue(12);     // a.value() == 12, b.value() == 12
        b.setValue(48);     // a.value() == 12, b.value() == 48
//! [0]

        System.err.println("a: "+a.value()+" b: "+b.value());
    }

//! [1]
    class Counter extends QSignalEmitter {
        int value;

//! [2]
        public Signal1<Integer> valueChanged = new Signal1<Integer>();
//! [2]

//! [3]
        @QtBlockedSlot
        public int value()
        {
            return value;
        }
//! [3]

//! [4]
        public void setValue(int val)
        {
            if (value != val) {
                value = val;
                valueChanged.emit(value);
            }
        }
//! [4]

        public Counter()
        {
            value = 0;
        }
    }
//! [1]

    public static void main(String args[])
    {
        System.err.println("Enter main");
        QApplication.initialize(args);
        System.err.println("All OK");

        new SignalsSlotsExample();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
