#ifndef QTJAMBI_XML_REPOSITORY_H
#define QTJAMBI_XML_REPOSITORY_H

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

namespace Java{
namespace QtXml{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDomDocument$Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QXmlEntityResolver$ResolvedEntity,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_FIELD(error)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(inputSource))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QXmlNamespaceSupport$ProcessedName,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QXmlNamespaceSupport$SplitName,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
#endif
}
}
#endif // QTJAMBI_XML_REPOSITORY_H
