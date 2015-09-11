/*   Ported from: src.3rdparty.webkit.JavaScriptCore.kjs.function.h
<snip>
//! [0]

     * function f() {
     *
     *   function b() {
     *     return prototype;
     *   }
     *
     *   var x = 4;
     *   // do some stuff
     * }
     * f.prototype = new String();
     * //! [0]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_3rdparty_webkit_JavaScriptCore_kjs_function {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]

     * function f() {
     *
     *   function b() {
     *     return prototype;
     *   }
     *
     *   var x = 4;
     *   // do some stuff
     * }
     * f.prototype = new String();
     * //! [0]


    }
}
