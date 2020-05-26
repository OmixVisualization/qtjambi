#ifndef QTJAMBI_XML_REPOSITORY_H
#define QTJAMBI_XML_REPOSITORY_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
class QtXml
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtXml,QDomDocument$Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtXml,QXmlEntityResolver$ResolvedEntity,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_FIELD(error)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(inputSource))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtXml,QXmlNamespaceSupport$ProcessedName,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QtXml,QXmlNamespaceSupport$SplitName,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
private:
    QtXml() = delete;
};
}
#endif // QTJAMBI_XML_REPOSITORY_H
