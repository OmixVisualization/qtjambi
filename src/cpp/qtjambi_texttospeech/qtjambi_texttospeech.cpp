#include "qtjambi_texttospeech.h"

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gMutex, (QMutex::Recursive))

namespace Java{
    namespace Private{
        QTJAMBI_REPOSITORY_DEFINE_CLASS(Runtime,io/qt/texttospeech,QTextToSpeechPlugin$CreateException,
            QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
            QTJAMBI_REPOSITORY_DEFINE_METHOD(getMessage,()Ljava/lang/String;)
        )
    }
}
