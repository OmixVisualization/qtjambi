/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui/QGuiApplication>
#include <QtGui/QGradient>
#include <QtCore/QMutex>

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtGui/QOpenGLContext>
#include <QtGui/qopenglversionfunctions.h>
#include <QtGui/QOpenGLFunctions_ES2>
#else
#include <QtGui/QPointerEvent>
#include <QtGui/QAction>
#endif
#include <QGradient>
#include <QPixmapCache>
#include <QtJambi/CoreAPI>
#include <QtJambi/guiapi.h>
#include <private/qguiapplication_p.h>
#include <qpa/qplatformintegration.h>
#include <QtJambi/RegistryAPI>

#include <QtJambi/qtjambi_cast.h>
#include "utils_p.h"
#include "hashes.h"

namespace Java{
namespace QtGui{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QClipboard$Text,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QQuaternion$Axes,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/gui/QVector3D;Lio/qt/gui/QVector3D;Lio/qt/gui/QVector3D;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QQuaternion$AxisAndAngle,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/gui/QVector3D;F)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QQuaternion$EulerAngles,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(FFF)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QTextCursor$SelectedTableCells,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIII)
)
}
namespace QtWidgets{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QMenu,
)
#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
#define QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(M,P) QTJAMBI_REPOSITORY_DEFINE_METHOD(M,P)
#else
#define QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(M,P)
#endif

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QApplication,
                                 QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(exec,()I)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(resolveInterface,(Ljava/lang/Class;)Lio/qt/QtObjectInterface;)
                                )
}
}

void initialize_meta_info_gui(){
    QPixmapCache::cacheLimit();
    RegistryAPI::registerPolymorphyHandler(typeid(QGradient), typeid(std::nullptr_t), [](void *ptr, qintptr& offset) -> bool {
                          QGradient *object = reinterpret_cast<QGradient *>(ptr);
                          Q_ASSERT(object);
                          offset = 0;
                          return object->type() == QGradient::NoGradient;
                      });
    GuiAPI::installThreadedPixmapsChecker([]()->bool{ return QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::ThreadedPixmaps); });
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QPainter_threadCheck)
(JNIEnv *env, jclass, jobject _object)
{
    QTJAMBI_TRY{
        QObject* object = QtJambiAPI::convertJavaObjectToQObject<QObject>(env, _object);
        QtJambiAPI::checkNullPointer(env, object);
        QtJambiAPI::checkThreadOnArgument(env, "device", object);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

// QOpenGLContext::versionFunctions<T>() const
QAbstractOpenGLFunctions* qtjambi_QOpenGLContext_versionFunctions(JNIEnv *__jni_env, const QOpenGLContext *__qt_this, jclass type){
    QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
    QString className = QtJambiAPI::getClassName(__jni_env, type);
    if(className == "io.qt.gui.QOpenGLFunctions_ES2"){
#if defined(QT_OPENGL_ES_2)
        return __qt_this->versionFunctions<QOpenGLFunctions_ES2>();
#else
    }else if(className.startsWith("io.qt.gui.QOpenGLFunctions_")){
        className = className.mid(27);
        int idx = className.indexOf('_');
        if(idx>0){
            bool ok = false;
            int majorVersion = className.left(idx).toInt(&ok);
            if(ok){
                className = className.mid(idx+1);
                QSurfaceFormat::OpenGLContextProfile profile = QSurfaceFormat::NoProfile;
                if(className.endsWith("_Core")){
                    className = className.chopped(5);
                    profile = QSurfaceFormat::CoreProfile;
                }else if(className.endsWith("_Compatibility")){
                    className = className.chopped(14);
                    profile = QSurfaceFormat::CompatibilityProfile;
                }
                ok = false;
                int minorVersion = className.toInt(&ok);
                if(ok){
                    QOpenGLVersionProfile version;
                    version.setProfile(profile);
                    version.setVersion(majorVersion, minorVersion);
                    return __qt_this->versionFunctions(version);
                }
            }
        }
#endif
    }
    return nullptr;
}
#else

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QPointerEvent_setPoint)
(JNIEnv *__jni_env,
 jobject _this,
 jlong i0,
 jobject __point)
{
    QTJAMBI_TRY{
        QPointerEvent *__qt_this = QtJambiAPI::convertJavaObjectToNative<QPointerEvent>(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QPointerEvent::setPoint(qsizetype, QEventPoint)", __qt_this)
        if(QEventPoint *__qt_point = qtjambi_cast<QEventPoint*>(__jni_env, __point)){
            QEventPoint& point = __qt_this->point(static_cast<qsizetype>(i0));
            point = *__qt_point;
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,12,0)
// QGradient::QGradient(QGradient::Preset arg__1)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QGradient_create)
(JNIEnv *__jni_env, jclass, int preset)
{
    jobject _result{nullptr};
    QTJAMBI_NATIVE_METHOD_CALL("QGradient::QGradient(QGradient::Preset arg__1)")
    QTJAMBI_TRY{
        _result = qtjambi_cast<jobject>(__jni_env, QGradient(QGradient::Preset(preset)));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtGui/private/qcolortransform_p.h>

hash_type qHash(const QColorVector &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    if(!value.isNull()){
        seed = hash(seed, value.x);
        seed = hash(seed, value.y);
        seed = hash(seed, value.z);
    }
    return seed;
}

hash_type qHash(const QColorMatrix &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.r);
    seed = hash(seed, value.g);
    seed = hash(seed, value.b);
    return seed;
}
#endif

hash_type qHash(const QColorTransform &value, hash_type seed)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QColorTransformPrivate* p = *reinterpret_cast<QColorTransformPrivate*const*>(&value);
#else
    QColorTransformPrivate* p = QColorTransformPrivate::get(value);
#endif
    QtPrivate::QHashCombine hash;
    if(p){
        seed = hash(seed, p->colorMatrix);
        seed = hash(seed, bool(p->colorSpaceIn));
        if(p->colorSpaceIn){
            seed = hash(seed, *reinterpret_cast<const QColorSpace*>(&p->colorSpaceIn));
        }
        seed = hash(seed, bool(p->colorSpaceOut));
        if(p->colorSpaceOut){
            seed = hash(seed, *reinterpret_cast<const QColorSpace*>(&p->colorSpaceOut));
        }
    }
    return seed;
}
#endif //QT_VERSION >= QT_VERSION_CHECK(5,14,0)
