#include "qtjambi_xml_repository.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
QTJAMBI_REPOSITORY_DEFINE_CLASS(QtXml,io/qt/xml,QDomDocument$Result,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ZLjava/lang/String;II)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtXml,io/qt/xml,QXmlEntityResolver$ResolvedEntity,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ZLio/qt/xml/QXmlInputSource;)
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(error,Z)
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(inputSource,Lio/qt/xml/QXmlInputSource;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtXml,io/qt/xml,QXmlNamespaceSupport$ProcessedName,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(QtXml,io/qt/xml,QXmlNamespaceSupport$SplitName,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;Ljava/lang/String;)
)
}
