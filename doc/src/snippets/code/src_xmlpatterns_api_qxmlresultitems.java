import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.xmlpatterns.*;


public class src_xmlpatterns_api_qxmlresultitems {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
  QXmlQuery query = new QXmlQuery();
  query.setQuery("<e/>, 1, 'two'");
  QXmlResultItems result = new QXmlResultItems();

  if(query.isValid())
  {
    query.evaluateToResult(result);
    QXmlItem item = result.next();
    while(!item.isNull())
    {
        // Use item
        item = result.next();
    }

    if(result.hasError())
        throw new RuntimeException("Runtime error!");
  }
//! [0]
    }
}
