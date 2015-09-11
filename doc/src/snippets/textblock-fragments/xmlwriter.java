import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;

public class xmlwriter
{
    QDomDocument document;
    QTextDocument textDocument;

    QDomDocument toXml()
    {
        QDomImplementation implementation = new QDomImplementation();
        QDomDocumentType docType = implementation.createDocumentType(
            "scribe-document", "scribe", "www.trolltech.com/scribe");

        document = new QDomDocument(docType);

        // ### This processing instruction is required to ensure that any kind
        // of encoding is given when the document is written.
        QDomProcessingInstruction process = document.createProcessingInstruction(
            "xml", "version=\"1.0\" encoding=\"utf-8\"");
        document.appendChild(process);

        QDomElement documentElement = document.createElement("document");
        document.appendChild(documentElement);

    //! [0]
        QTextBlock currentBlock = textDocument.begin();

        while (currentBlock.isValid()) {
    //! [0]
            QDomElement blockElement = document.createElement("block");
            document.appendChild(blockElement);

            readFragment(currentBlock, blockElement, document);

    //! [1]
            processBlock(currentBlock);
    //! [1]

    //! [2]
            currentBlock = currentBlock.next();
        }
    //! [2]

        return document;
    }

    void readFragment(QTextBlock currentBlock,
                      QDomElement blockElement,
                      QDomDocument document)
    {
    //! [3] //! [4]
        QTextBlock_iterator it = new QTextBlock_iterator();
        for (it = currentBlock.begin(); !(it.atEnd()); it.next()) {
            QTextFragment currentFragment = it.fragment();
            if (currentFragment.isValid())
    //! [3] //! [5]
                processFragment(currentFragment);
    //! [4] //! [5]

            if (currentFragment.isValid()) {
                QDomElement fragmentElement = document.createElement("fragment");
                blockElement.appendChild(fragmentElement);

                fragmentElement.setAttribute("length", currentFragment.length());
                QDomText fragmentText = document.createTextNode(currentFragment.text());

                fragmentElement.appendChild(fragmentText);
    //! [6]
            }
    //! [7]
        }
    //! [6] //! [7]
    }

    void processBlock(QTextBlock currentBlock)
    {
    }

    void processFragment(QTextFragment currentFragment)
    {
    }

}
