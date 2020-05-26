#ifndef QTJAMBI_TEXTTOSPEECH_H
#define QTJAMBI_TEXTTOSPEECH_H

#include <qtjambi/qtjambi_repository.h>

namespace Java{
    namespace Private{
        class Runtime{
            QTJAMBI_REPOSITORY_DECLARE_CLASS(Runtime,QTextToSpeechPlugin$CreateException,
                QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR()
                QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getMessage))
        };
    }
}

#endif // QTJAMBI_TEXTTOSPEECH_H
