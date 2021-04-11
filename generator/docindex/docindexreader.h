#ifndef DOCINDEXREADER_H
#define DOCINDEXREADER_H

#include <QtCore>
#include <QtXml>
#include "docmodel.h"

class DocIndexReader
{
public:
    DocIndexReader();
    const DocModel* readDocIndexes(const QDir& docDirectory, QThread* targetThread);
private:
    void analyzeNamespace(const QDir& subdir, DocModel* model, const QString& url, const QDomElement& element);
    void analyzeClass(const QDir& subdir, DocModel* model, const QString& url, const QDomElement& element);
    void analyzeEnum(const QDir& subdir, DocModel* model, const QString& url, const QDomElement& element);
    template<class DocFunctionOwner>
    void analyzeFunction(const QDir& subdir, DocFunctionOwner* owner, const QString& url, const QDomElement& element);
    void analyzeVariable(const QDir& subdir, DocClass* cls, const QString& url, const QDomElement& element);
    void analyzeTypeDef(const QDir& subdir, DocClass* cls, const QString& url, const QDomElement& element);
    void analyzeProperty(const QDir& subdir, DocClass* cls, const QString& url, const QDomElement& element);
};

#endif // DOCINDEXREADER_H
