
import org.qtjambi.qt.*;
import org.qtjambi.qt.gui.*;
import java.util.*;

public class doc_src_qpair {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QPair<String, Double> pair = new QPair<String, Double>("", 0.0);
//! [0]

//! [1]
        pair.first = "pi";
        pair.second = 3.14159265358979323846;
//! [1]


//! [2]
        List<QPair<Integer, Double> > list = new ArrayList<QPair<Integer, Double>>();
        list.add(new QPair<Integer, Double>(66, 3.14159));
//! [2]


    }
}
