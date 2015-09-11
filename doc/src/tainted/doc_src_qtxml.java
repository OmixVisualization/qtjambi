/*   Ported from: doc.src.qtxml.qdoc
<snip>
//! [0]
        #include <QtXml>
//! [0]


//! [1]
        QT += xml
//! [1]


//! [2]
        QT += xml
//! [2]


//! [3]
    <quote>A quotation.</quote>
//! [3]


//! [4]
    <document xmlns:book = 'http://trolltech.com/fnord/book/'
              xmlns      = 'http://trolltech.com/fnord/' >
//! [4]


//! [5]
    <author xmlns:fnord = 'http://trolltech.com/fnord/'
                 title="Ms"
                 fnord:title="Goddess"
                 name="Eris Kallisti"/>
//! [5]


//! [6]
    <document>
    <book>
      <title>Practical XML</title>
      <author title="Ms" name="Eris Kallisti"/>
      <chapter>
        <title>A Namespace Called fnord</title>
      </chapter>
    </book>
    </document>
//! [6]


//! [7]
    <book:title>Practical XML</book:title>
//! [7]


//! [8]
    xmlns="http://trolltech.com/fnord/"
//! [8]


//! [9]
    xmlns:book="http://trolltech.com/fnord/book/"
//! [9]


//! [10]
    <document xmlns:book = 'http://trolltech.com/fnord/book/'
              xmlns      = 'http://trolltech.com/fnord/' >
    <book>
      <book:title>Practical XML</book:title>
      <book:author xmlns:fnord = 'http://trolltech.com/fnord/'
                   title="Ms"
                   fnord:title="Goddess"
                   name="Eris Kallisti"/>
      <chapter>
        <title>A Namespace Called fnord</title>
      </chapter>
    </book>
    </document>
//! [10]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_qtxml {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        #include <QtXml>
//! [0]


//! [1]
        QT += xml
//! [1]


//! [2]
        QT += xml
//! [2]


//! [3]
    <quote>A quotation.</quote>
//! [3]


//! [4]
    <document xmlns:book = 'http://trolltech.com/fnord/book/'
              xmlns      = 'http://trolltech.com/fnord/' >
//! [4]


//! [5]
    <author xmlns:fnord = 'http://trolltech.com/fnord/'
                 title="Ms"
                 fnord:title="Goddess"
                 name="Eris Kallisti"/>
//! [5]


//! [6]
    <document>
    <book>
      <title>Practical XML</title>
      <author title="Ms" name="Eris Kallisti"/>
      <chapter>
        <title>A Namespace Called fnord</title>
      </chapter>
    </book>
    </document>
//! [6]


//! [7]
    <book:title>Practical XML</book:title>
//! [7]


//! [8]
    xmlns="http://trolltech.com/fnord/"
//! [8]


//! [9]
    xmlns:book="http://trolltech.com/fnord/book/"
//! [9]


//! [10]
    <document xmlns:book = 'http://trolltech.com/fnord/book/'
              xmlns      = 'http://trolltech.com/fnord/' >
    <book>
      <book:title>Practical XML</book:title>
      <book:author xmlns:fnord = 'http://trolltech.com/fnord/'
                   title="Ms"
                   fnord:title="Goddess"
                   name="Eris Kallisti"/>
      <chapter>
        <title>A Namespace Called fnord</title>
      </chapter>
    </book>
    </document>
//! [10]


    }
}
