import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_tools_qbitarray {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

    public void first() {
//! [0]
        QBitArray ba = new QBitArray(200);
//! [0]
    }

    public void second() {
//! [1]
       //The following snipplet is not applicable in Java since there are no
       //support for operator overloading. Use QBitArray.setBit(int).
       /*
        QBitArray ba;
        ba.resize(3);
        ba[0] = true;
        ba[1] = false;
        ba[2] = true;
        */
//! [1]
}
    public void third() {
//! [2]
        QBitArray ba = new QBitArray(3);
        ba.setBit(0, true);
        ba.setBit(1, false);
        ba.setBit(2, true);
//! [2]


//! [3]
        QBitArray x = new QBitArray(5);
        x.setBit(3, true);
        // x: [ 0, 0, 0, 1, 0 ]

        QBitArray y = new QBitArray(5);
        y.setBit(4, true);
        // y: [ 0, 0, 0, 0, 1 ]

        x.or(y);
        // x: [ 0, 0, 0, 1, 1 ]
//! [3]


//! [4]
        new QBitArray().isNull();           // returns true
        new QBitArray().isEmpty();          // returns true

        new QBitArray(0).isNull();          // returns false
        new QBitArray(0).isEmpty();         // returns true

        new QBitArray(3).isNull();          // returns false
        new QBitArray(3).isEmpty();         // returns false
//! [4]


//! [5]
        new QBitArray().isNull();           // returns true
        new QBitArray(0).isNull();          // returns false
        new QBitArray(3).isNull();          // returns false
//! [5]
}
    public void fourth() {

//! [6]
        QBitArray ba = new QBitArray(8);
        ba.fill(true);
        // ba: [ 1, 1, 1, 1, 1, 1, 1, 1 ]

        ba.fill(false, 2);
        // ba: [ 0, 0 ]
//! [6]
    }
    public void fith() {
//! [7]
        QBitArray a = new QBitArray(3);
        a.setBit(0, false);
        a.setBit(1,true);
        a.setBit(2, a.at(0) ^ a.at(1));
//! [7]
                }

//! [8]


       //The following snipplet is not applicable in Java since there are no
       //support for operator overloading.
        /*QBitArray a(3);
        QBitArray b(2);
        a[0] = 1; a[1] = 0; a[2] = 1;   // a: [ 1, 0, 1 ]
        b[0] = 1; b[1] = 0;             // b: [ 1, 1 ]
        a &= b;                         // a: [ 1, 0, 0 ]
        */
//! [8]


//! [9]
       //The following snipplet is not applicable in Java since there are no
       //support for operator overloading.
        /*QBitArray a(3);
        QBitArray b(2);
        a[0] = 1; a[1] = 0; a[2] = 1;   // a: [ 1, 0, 1 ]
        b[0] = 1; b[1] = 0;             // b: [ 1, 1 ]
        a |= b;                         // a: [ 1, 1, 1 ]*/
//! [9]


//! [10]
       //The following snipplet is not applicable in Java since there are no
       //support for operator overloading.
       /* QBitArray a(3);
        QBitArray b(2);
        a[0] = 1; a[1] = 0; a[2] = 1;   // a: [ 1, 0, 1 ]
        b[0] = 1; b[1] = 0;             // b: [ 1, 1 ]
        a ^= b;                         // a: [ 0, 1, 1 ]*/
//! [10]


//! [11]
       //The following snipplet is not applicable in Java since there are no
       //support for operator overloading.
       /* QBitArray a(3);
        QBitArray b;
        a[0] = 1; a[1] = 0; a[2] = 1;   // a: [ 1, 0, 1 ]
        b = ~a;                         // b: [ 0, 1, 0 ] */
//! [11]

    public void sixth() {
//! [12]
        QBitArray a = new QBitArray(3);
        QBitArray b = new QBitArray(2);
        a.setBit(0); a.setBit(1,false); a.setBit(2);  // a: [ 1, 0, 1 ]
        b.setBit(0); b.setBit(1);                  // b: [ 1, 1 ]
        a.and(b);                            // a: [ 1, 0, 0 ]
//! [12]
}
    public void seventh() {
//! [13]
        QBitArray a = new QBitArray(3);
        QBitArray b = new QBitArray(2);
        QBitArray c;
        a.setBit(0); a.setBit(1,false); a.setBit(2);  // a: [ 1, 0, 1 ]
        b.setBit(0); b.setBit(1);                  // b: [ 1, 1 ]
        a.or(b);                             // a: [ 1, 1, 1 ]
//! [13]
}
    public void eigth() {
//! [14]
        QBitArray a = new QBitArray(3);
        QBitArray b = new QBitArray(2);
        QBitArray c;
        a.setBit(0); a.setBit(1,false); a.setBit(2);  // a: [ 1, 0, 1 ]
        b.setBit(0); b.setBit(1);                  // b: [ 1, 1 ]
        a.xor(b);                            // a: [ 0, 1, 1 ]
//! [14]
    }

}

