/*   Ported from: src.3rdparty.webkit.JavaScriptCore.kjs.object.h
<snip>
//! [0]

     *   class BarImp : public JSObject {
     *     virtual const ClassInfo *classInfo() const { return &info; }
     *     static const ClassInfo info;
     *     // ...
     *   };
     *
     *   class FooImp : public JSObject {
     *     virtual const ClassInfo *classInfo() const { return &info; }
     *     static const ClassInfo info;
     *     // ...
     *   };
     * //! [0]


//! [1]

     *   const ClassInfo BarImp::info = {"Bar", 0, 0, 0}; // no parent class
     *   const ClassInfo FooImp::info = {"Foo", &BarImp::info, 0, 0};
     * //! [1]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_3rdparty_webkit_JavaScriptCore_kjs_object {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]

     *   class BarImp : public JSObject {
     *     virtual ClassInfo lassInfo() { return nfo; }
     *     static ClassInfo info;
     *     // ...
     *   };
     *
     *   class FooImp : public JSObject {
     *     virtual ClassInfo lassInfo() { return nfo; }
     *     static ClassInfo info;
     *     // ...
     *   };
     * //! [0]


//! [1]

     *   ClassInfo BarImp.info = {"Bar", 0, 0, 0}; // no parent class
     *   ClassInfo FooImp.info = {"Foo", arImp.info, 0, 0};
     * //! [1]


    }
}
