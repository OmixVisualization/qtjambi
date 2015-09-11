import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;

import java.util.*;

public class xmlwriter
{
    QDomDocument document;
    QTextDocument textDocument;

    public xmlwriter(QTextDocument document)
    {
        textDocument = document;
    }

    public QDomDocument toXml()
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
        QTextFrame root = textDocument.rootFrame();
    //! [0]

        if (root != null)
            processFrame(documentElement, root);

        return document;
    }

    public void processBlock(QDomElement parent, QTextBlock block)
    {
        QDomElement blockElement = document.createElement("block");
        blockElement.setAttribute("position", block.position());
        blockElement.setAttribute("length", block.length());
        parent.appendChild(blockElement);

        QTextBlock_iterator it;
        for (it = block.begin(); !(it.atEnd()); it.next()) {
            QTextFragment fragment = it.fragment();

            if (fragment.isValid()) {
                QDomElement fragmentElement = document.createElement("fragment");
                blockElement.appendChild(fragmentElement);

                fragmentElement.setAttribute("length", fragment.length());
                QDomText fragmentText = document.createTextNode(fragment.text());

                fragmentElement.appendChild(fragmentText);
            }
        }
    }

    public void processFrame(QDomElement parent, QTextFrame frame)
    {
        QDomElement frameElement = document.createElement("frame");
        frameElement.setAttribute("begin", frame.firstPosition());
        frameElement.setAttribute("end", frame.lastPosition());
        parent.appendChild(frameElement);

    //! [1]
        QTextFrame_iterator it;
        for (it = frame.begin(); !(it.atEnd()); it.next()) {
            QTextFrame childFrame = it.currentFrame();
            QTextBlock childBlock = it.currentBlock();

            if (childFrame != null)
    //! [1] //! [2]
                processFrame(frameElement, childFrame);
            else if (childBlock.isValid())
                processBlock(frameElement, childBlock);
        }
    //! [2]
    }
}
