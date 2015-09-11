import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_xml_dom_qdom {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QDomDocument doc = new QDomDocument();
        QDomImplementation impl = new QDomImplementation();

        // This will create the element, but the resulting XML document will
        // be invalid, because '~' is not a valid character in a tag name.
        impl.setInvalidDataPolicy(QDomImplementation.InvalidDataPolicy.AcceptInvalidChars);
        QDomElement elt1 = doc.createElement("foo~bar");

        // This will create an element with the tag name "foobar".
        impl.setInvalidDataPolicy(QDomImplementation.InvalidDataPolicy.DropInvalidChars);
        QDomElement elt2 = doc.createElement("foo~bar");

        // This will create a null element.
        impl.setInvalidDataPolicy(QDomImplementation.InvalidDataPolicy.ReturnNullNode);
        QDomElement elt3 = doc.createElement("foo~bar");
//! [0]

    java.lang.String someXML = "blah";
//! [1]
    QDomDocument d = new QDomDocument();
    d.setContent(someXML);
    QDomNode n = d.firstChild();
    while (!n.isNull()) {
        if (n.isElement()) {
            QDomElement e = n.toElement();
            System.out.printf("Element name: %s\n", e.tagName());
            break;
        }
        n = n.nextSibling();
    }
//! [1]


//! [2]
        QDomDocument document = new QDomDocument();
        QDomElement element1 = document.documentElement();
        QDomElement element2 = element1;
//! [2]


//! [3]
        QDomElement element3 = document.createElement("MyElement");
        QDomElement element4 = document.createElement("MyElement");
//! [3]

/*
//! [4]
    <body>
    <h1>Heading</h1>
    <p>Hello <b>you</b></p>
    </body>
//! [4]


//! [5]
    <h1>Heading</h1>
    <p>The text...</p>
    <h2>Next heading</h2>
//! [5]


//! [6]
    <h1>Heading</h1>
    <p>The text...</p>
    <h2>Next heading</h2>
//! [6]


//! [7]
    <link href="http://www.trolltech.com" color="red" />
//! [7]

//! [8]
    QDomElement e = //...
    //...
    QDomAttr a = e.attributeNode("href");
    System.out.println(a.value());                // prints "http://www.trolltech.com"
    a.setValue("http://doc.trolltech.com"); // change the node's attribute
    QDomAttr a2 = e.attributeNode("href");
    System.out.println(a2.value());               // prints "http://doc.trolltech.com"
//! [8]


//! [9]
    QDomElement e = //...
    //...
    java.lang.String s = e.text()
//! [9]

*/

//! [10]
    java.lang.String text = new java.lang.String();
    QDomElement element = doc.documentElement();
    for(QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling())
    {
        QDomText t = node.toText();
        if (!t.isNull())
            text += t.data();
    }
//! [10]

/*
//! [11]
    QDomDocument doc = // ...
    QDomElement root = doc.firstChildElement("database");
    QDomElement elt = root.firstChildElement("entry");
    for (; !elt.isNull(); elt = elt.nextSiblingElelement("entry")) {
        // ...
    }
//! [11]
*/

/*
//! [12]
    <img src="myimg.png">
//! [12]


//! [13]
    <h1>Hello <b>Qt</b> <![CDATA[<xml is cool>]]></h1>
//! [13]


//! [14]
    Hello Qt t;xml is coolt;
//! [14]


//! [15]
        <!-- this is a comment -.
//! [15]
*/

//! [16]
    QDomDocument domDoc = new QDomDocument("mydocument");
    QFile file = new QFile("mydocument.xml");
    QFile.OpenMode mode = new QFile.OpenMode();
    mode.set(QFile.OpenModeFlag.ReadOnly);

    if (!file.open(mode))
        return;
    if (!domDoc.setContent(file).success) {
        file.close();
        return;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();

    QDomNode domNode = docElem.firstChild();
    while(!domNode.isNull()) {
        QDomElement e = domNode.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            System.out.println(e.tagName()); // the node really is an element.
        }
        domNode = domNode.nextSibling();
    }

    // Here we append a new element to the end of the document
    QDomElement elem = doc.createElement("img");
    elem.setAttribute("src", "myimage.png");
    docElem.appendChild(elem);
//! [16]
    }

    public void theRest() {
//! [17]
    QDomDocument doc = new QDomDocument("MyML");
    QDomElement root = doc.createElement("MyML");
    doc.appendChild(root);

    QDomElement tag = doc.createElement("Greeting");
    root.appendChild(tag);

    QDomText t = doc.createTextNode("Hello World");
    tag.appendChild(t);

    java.lang.String xml = doc.toString();
//! [17]


    }
}
