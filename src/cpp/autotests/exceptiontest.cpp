#include <QtCore/QException>
#include <QtCore/QEasingCurve>
#include <QtJambi/QtJambiAPI>
#include "exceptiontest.h"

ExceptionTest::ExceptionTest()
{

}

void ExceptionTest::testExceptions(){
    try{
        throw int(1);
    }catch(int){
        qWarning("catching thrown int");
    }

    try{
        throw int(1);
    }catch(...){
        qWarning("catching thrown int by [...]");
    }

    try{
        throw std::exception();
    }catch(const std::exception&){
        qWarning("catching thrown std::exception");
    }

    try{
        throw std::exception();
    }catch(...){
        qWarning("catching thrown std::exception by [...]");
    }

    try{
        throw QException();
    }catch(const QException&){
        qWarning("catching thrown QException");
    }

    try{
        throw QException();
    }catch(...){
        qWarning("catching thrown QException by [...]");
    }

    QTJAMBI_TRY{
        //QException().raise();
    }QTJAMBI_CATCH(const QException&){
        qWarning("catching rissen QException");
    }QTJAMBI_TRY_END

    QTJAMBI_TRY_ANY{
        //QException().raise();
    }QTJAMBI_CATCH_ANY{
        qWarning("catching rissen QException by QTJAMBI_CATCH_ANY");
    }QTJAMBI_TRY_END

    QTJAMBI_TRY_ANY{
        throw int(5);
    }QTJAMBI_CATCH_ANY{
        qWarning("catching thrown int by QTJAMBI_CATCH_ANY");
    }QTJAMBI_TRY_END

    try{
        throw JavaException();
    }catch(const JavaException&){
        qWarning("catching thrown JavaException");
    }

    try{
        throw JavaException();
    }catch(...){
        qWarning("catching thrown JavaException by [...]");
    }

    QTJAMBI_TRY{
        throw JavaException();
    }QTJAMBI_CATCH(const JavaException&){
        qWarning("catching thrown JavaException by QTJAMBI_CATCH");
    }QTJAMBI_TRY_END

    QTJAMBI_TRY{
        if(JniEnvironment env{16}){
            JavaException::raiseError(env, nullptr QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException&){
        qWarning("catching rissen JavaException by QTJAMBI_CATCH");
    }QTJAMBI_TRY_END

    QTJAMBI_TRY_ANY{
        if(JniEnvironment env{16}){
            JavaException::raiseError(env, nullptr QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH_ANY{
        qWarning("catching rissen JavaException by QTJAMBI_CATCH_ANY");
    }QTJAMBI_TRY_END

    QEasingCurve curve;
    curve.setCustomType([](qreal)->qreal{
        throw int(1);
    });
    try{
        curve.valueForProgress(0.1);
    }catch(int){
        qWarning("catching thrown int through QtCore");
    }

    curve.setCustomType([](qreal)->qreal{
        throw std::exception();
    });
    try{
        curve.valueForProgress(0.1);
    }catch(const std::exception&){
        qWarning("catching thrown std::exception through QtCore");
    }

    curve.setCustomType([](qreal)->qreal{
        throw QException();
    });
    try{
        curve.valueForProgress(0.1);
    }catch(const QException&){
        qWarning("catching thrown QException through QtCore");
    }

    curve.setCustomType([](qreal)->qreal{
        throw JavaException();
    });
    try{
        curve.valueForProgress(0.1);
    }catch(const JavaException&){
        qWarning("catching thrown JavaException through QtCore");
    }

    curve.setCustomType([](qreal d)->qreal{
        //QException().raise();
        return d;
    });
    try{
        curve.valueForProgress(0.1);
    }catch(const QException&){
        qWarning("catching rissen QException through QtCore");
    }catch(...){
        qWarning("catching rissen QException through QtCore by [...]");
    }

    curve.setCustomType([](qreal d)->qreal{
        if(JniEnvironment env{16}){
            JavaException::raiseError(env, nullptr QTJAMBI_STACKTRACEINFO );
        }
        return d;
    });
    QTJAMBI_TRY{
        curve.valueForProgress(0.1);
    }QTJAMBI_CATCH(const JavaException&){
        qWarning("catching rissen JavaException through QtCore by QTJAMBI_CATCH");
    }QTJAMBI_TRY_END

    try{
        curve.valueForProgress(0.1);
    }catch(const JavaException&){
        qWarning("catching rissen JavaException through QtCore by catch");
    }
}
