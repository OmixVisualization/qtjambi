#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QDomDocument>

class QTextDocument;

class XmlWriter
{
public:
    XmlWriter(QTextDocument *document) : textDocument(document) {}
    QDomDocument *toXml();

private:
    void processBlock(QDomElement &parent, const QTextBlock &block);
    void processFrame(QDomElement &parent, QTextFrame *frame);
    void processTable(QDomElement &parent, QTextTable *table);
    void processTableCell(QDomElement &parent, const QTextTableCell &cell);

    QDomDocument *document;
    QTextDocument *textDocument;
};

#endif
