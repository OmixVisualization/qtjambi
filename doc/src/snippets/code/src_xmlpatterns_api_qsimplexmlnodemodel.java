import org.qtjambi.qt.gui.QApplication;


public class src_xmlpatterns_api_qsimplexmlnodemodel {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
  QXmlItemIteratorPointer MyTreeModel.nextFromSimpleAxis(SimpleAxis axis, QXmlNodeModelIndex origin)
  {
    // Convert the QXmlNodeModelIndex to a value that is specific to what we represent.
    MyValue value = toMyValue(ni);

    switch(axis)
    {
        case Parent:
            return toNodeIndex(value.parent());
        case FirstChild:
        case PreviousSibling:
        case NextSibling:
            // and so on
    }
  }
//! [0]


    }
}
