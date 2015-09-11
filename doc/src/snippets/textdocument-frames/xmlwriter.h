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

    QDomDocument *document;
//! [0]
    QTextDocument *textDocument;
//! [0]
};

#endif
