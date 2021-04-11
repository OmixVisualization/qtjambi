#include <QtCore/QMutex>
#include "qtjambi_texttospeech.h"

Q_GLOBAL_STATIC(QRecursiveMutex, gMutex)

namespace Java{
    namespace QtTextToSpeech{
        QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/texttospeech,QTextToSpeechPlugin$CreateException,
            QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
            QTJAMBI_REPOSITORY_DEFINE_METHOD(getMessage,()Ljava/lang/String;)
        )
    }
}
