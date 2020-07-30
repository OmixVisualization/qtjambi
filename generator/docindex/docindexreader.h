#ifndef DOCINDEXREADER_H
#define DOCINDEXREADER_H

#include <QtCore>
#include <QtXml>
#include "docmodel.h"

class DocIndexReader
{
public:
    DocIndexReader();
    const DocModel* readDocIndexes(const QDir& docDirectory);
private:
    void analyzeNamespace(const QDir& subdir, DocModel* model, QDomElement element);
    void analyzeClass(const QDir& subdir, DocModel* model, QDomElement element);
    void analyzeEnum(const QDir& subdir, DocModel* model, QDomElement element);
    template<class DocFunctionOwner>
    void analyzeFunction(const QDir& subdir, DocFunctionOwner* owner, QDomElement element);
    void analyzeVariable(const QDir& subdir, DocClass* cls, QDomElement element);
    void analyzeProperty(const QDir& subdir, DocClass* cls, QDomElement element);
};

#endif // DOCINDEXREADER_H
