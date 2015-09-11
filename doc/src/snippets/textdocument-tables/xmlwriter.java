import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class xmlwriter
{

    private QDomDocument document;
    private QTextDocument textDocument;

    private QDomDocument toXml()
    {
        QDomImplementation implementation;
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

        QTextFrame root = textDocument.rootFrame();

        if (root)
            processFrame(documentElement, root);

        return document;
    }

    private void processBlock(QDomElement parent, QTextBlock block)
    {
        QDomElement blockElement = document.createElement("block");
        blockElement.setAttribute("position", block.position());
        blockElement.setAttribute("length", block.length());
        parent.appendChild(blockElement);

        QTextBlock.iterator it;
        for (it = block.begin(); !(it.atEnd()); ++it) {
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

    private void processFrame(QDomElement parent, QTextFrame frame)
    {
        QDomElement frameElement = document.createElement("frame");
        frameElement.setAttribute("begin", frame.firstPosition());
        frameElement.setAttribute("end", frame.lastPosition());
        parent.appendChild(frameElement);

    //! [0]
        QTextFrame_iterator it;
        for (it = frame.begin(); !(it.atEnd()); it.next()) {

            QTextFrame childFrame = it.currentFrame();
            QTextBlock childBlock = it.currentBlock();

            if (childFrame) {
                QTextTable childTable = (QTextTable) childFrame;

                if (childTable)
                    processTable(frameElement, childTable);
                else
                    processFrame(frameElement, childFrame);

            } else if (childBlock.isValid())
    //! [0] //! [1]
                processBlock(frameElement, childBlock);
        }
    //! [1]
    }

    private void processTable(QDomElement parent, QTextTable table)
    {
        QDomElement element = document.createElement("table");

        for (int row = 0; row < table.rows(); ++row) {
            for (int column = 0; column < table.columns(); ++column) {
                QTextTableCell cell = table.cellAt(row, column);
                processTableCell(element, cell);
            }
        }
        parent.appendChild(element);
    }

    private void processTableCell(QDomElement parent, QTextTableCell cell)
    {
        QDomElement element = document.createElement("cell");
        element.setAttribute("row", cell.row());
        element.setAttribute("column", cell.column());

        QTextFrame.iterator it;
        for (it = cell.begin(); !(it.atEnd()); it.next()) {

            QTextFrame childFrame = it.currentFrame();
            QTextBlock childBlock = it.currentBlock();

            if (childFrame)
                processFrame(element, childFrame);
            else if (childBlock.isValid())
                processBlock(element, childBlock);
        }
        parent.appendChild(element);
    }
}
