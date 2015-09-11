/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#include "qtjambi_global.h"
#include "qtjambi_core.h"

#include <common/qnativepointer.h>

#include <stdlib.h>

/*******************************************************************************
 * Read functions
 */

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_readBoolean)
  (JNIEnv *, jclass, jlong ptr, jint pos)
{
    return (reinterpret_cast<bool *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jbyte JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_readByte)
  (JNIEnv *, jclass, jlong ptr, jint pos)
{
    return (reinterpret_cast<qint8 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jchar JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_readChar)
  (JNIEnv *, jclass, jlong ptr, jint pos)
{
    return (reinterpret_cast<quint16 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jshort JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_readShort)
  (JNIEnv *, jclass, jlong ptr, jint pos)
{
    return (reinterpret_cast<qint16 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_readInt)
  (JNIEnv *, jclass, jlong ptr, jint pos)
{
    return (reinterpret_cast<qint32 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_readLong)
  (JNIEnv *, jclass, jlong ptr, jint pos)
{
    return (reinterpret_cast<qint64 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jfloat JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_readFloat)
  (JNIEnv *, jclass, jlong ptr, jint pos)
{
    return (reinterpret_cast<float *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jdouble JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_readDouble)
  (JNIEnv *, jclass, jlong ptr, jint pos)
{
    return (reinterpret_cast<double *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_readPointer)
  (JNIEnv *, jclass, jlong ptr, jint pos)
{
    return reinterpret_cast<jlong>((reinterpret_cast<void **>(ptr))[pos]);
}

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_readString)
  (JNIEnv *env, jclass, jlong ptr, jint pos)
{
    return qtjambi_from_qstring(env, reinterpret_cast<QString *>(ptr)[pos]);
}

/*******************************************************************************
 * Write functions
 */

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_writeBoolean)
  (JNIEnv *, jclass, jlong ptr, jint pos, jboolean value)
{
    ((reinterpret_cast<bool *>(ptr))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_writeByte)
  (JNIEnv *, jclass, jlong ptr, jint pos, jbyte value)
{
    (((reinterpret_cast<qint8 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_writeChar)
  (JNIEnv *, jclass, jlong ptr, jint pos, jchar value)
{
    (((reinterpret_cast<quint16 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_writeShort)
  (JNIEnv *, jclass, jlong ptr, jint pos, jshort value)
{
    (((reinterpret_cast<qint16 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_writeInt)
  (JNIEnv *, jclass, jlong ptr, jint pos, jint value)
{
    (((reinterpret_cast<qint32 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_writeLong)
  (JNIEnv *, jclass, jlong ptr, jint pos, jlong value)
{
    (((reinterpret_cast<qint64 *>(ptr)))[pos]) = value;
}
extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_writeFloat)
  (JNIEnv *, jclass, jlong ptr, jint pos, jfloat value)
{
    (((reinterpret_cast<float *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_writeDouble)
  (JNIEnv *, jclass, jlong ptr, jint pos, jdouble value)
{
    (((reinterpret_cast<double *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_writePointer)
  (JNIEnv *, jclass, jlong ptr, jint pos, jlong value)
{
    (reinterpret_cast<void **>(ptr))[pos] = reinterpret_cast<void *>(value);
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_writeString)
  (JNIEnv *env, jclass, jlong ptr, jint pos, jstring value)
{
    reinterpret_cast<QString *>(ptr)[pos] = qtjambi_to_qstring(env, value);
}


/*******************************************************************************
 * other stuff...
 */


extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_createPointer)
  (JNIEnv *, jobject, jint type, jint size, jint indirections)
{
    Q_ASSERT(indirections > 0);
    Q_ASSERT(size > 0);

    if (indirections != 1) {
        return reinterpret_cast<jlong>(new void*[size]);
    }

    if (size == 1) {
        switch (type) {
        case BooleanType: return reinterpret_cast<jlong>(new bool);
        case ByteType: return reinterpret_cast<jlong>(new qint8);
        case CharType: return reinterpret_cast<jlong>(new quint16);
        case ShortType: return reinterpret_cast<jlong>(new qint16);
        case IntType: return reinterpret_cast<jlong>(new qint32);
        case LongType: return reinterpret_cast<jlong>(new qint64);
        case FloatType: return reinterpret_cast<jlong>(new float);
        case DoubleType: return reinterpret_cast<jlong>(new double);
        case PointerType: return reinterpret_cast<jlong>(new void *);
        case StringType: return reinterpret_cast<jlong>(new QString());
        }
    } else if (size > 1) {
        switch (type) {
        case BooleanType: return reinterpret_cast<jlong>(new bool[size]);
        case ByteType: return reinterpret_cast<jlong>(new qint8[size]);
        case CharType: return reinterpret_cast<jlong>(new quint16[size]);
        case ShortType: return reinterpret_cast<jlong>(new qint16[size]);
        case IntType: return reinterpret_cast<jlong>(new qint32[size]);
        case LongType: return reinterpret_cast<jlong>(new qint64[size]);
        case FloatType: return reinterpret_cast<jlong>(new float[size]);
        case DoubleType: return reinterpret_cast<jlong>(new double[size]);
        case PointerType: return reinterpret_cast<jlong>(new void *[size]);
        case StringType: return reinterpret_cast<jlong>(new QString[size]);
        }
    }
    return 0;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QNativePointer_deletePointer)
  (JNIEnv *, jobject, jlong ptr, jint type, jint deleteMode)
{
    if (deleteMode == 0) { // free()
        switch (type) {
        case BooleanType:       free((reinterpret_cast<bool *>(ptr))); break;
        case ByteType:          free((reinterpret_cast<qint8 *>(ptr))); break;
        case CharType:          free((reinterpret_cast<quint16 *>(ptr))); break;
        case ShortType:         free((reinterpret_cast<qint16 *>(ptr))); break;
        case IntType:           free((reinterpret_cast<int *>(ptr))); break;
        case LongType:          free((reinterpret_cast<qint64 *>(ptr))); break;
        case FloatType:         free((reinterpret_cast<float *>(ptr))); break;
        case DoubleType:        free((reinterpret_cast<double *>(ptr))); break;
        default:
            qWarning("Unhandled free of type: %d\n", (int) type);
            break;
        }

    } else if (deleteMode == 1) { // delete
        switch (type) {
        case BooleanType:       delete ((reinterpret_cast<bool *>(ptr))); break;
        case ByteType:          delete ((reinterpret_cast<qint8 *>(ptr))); break;
        case CharType:          delete ((reinterpret_cast<quint16 *>(ptr))); break;
        case ShortType:         delete ((reinterpret_cast<qint16 *>(ptr))); break;
        case IntType:           delete ((reinterpret_cast<int *>(ptr))); break;
        case LongType:          delete ((reinterpret_cast<qint64 *>(ptr))); break;
        case FloatType:         delete ((reinterpret_cast<float *>(ptr))); break;
        case DoubleType:        delete ((reinterpret_cast<double *>(ptr))); break;
        case StringType:        delete ((reinterpret_cast<QString *>(ptr))); break;
        default:
            qWarning("Unhandled delete of type: %d\n", (int) type);
            break;
        }

    } else if (deleteMode == 2) { // delete []
        switch (type) {
        case BooleanType:       delete [] ((reinterpret_cast<bool *>(ptr))); break;
        case ByteType:          delete [] ((reinterpret_cast<qint8 *>(ptr))); break;
        case CharType:          delete [] ((reinterpret_cast<quint16 *>(ptr))); break;
        case ShortType:         delete [] ((reinterpret_cast<qint16 *>(ptr))); break;
        case IntType:           delete [] ((reinterpret_cast<int *>(ptr))); break;
        case LongType:          delete [] ((reinterpret_cast<qint64 *>(ptr))); break;
        case FloatType:         delete [] ((reinterpret_cast<float *>(ptr))); break;
        case DoubleType:        delete [] ((reinterpret_cast<double *>(ptr))); break;
        case StringType:        delete [] ((reinterpret_cast<QString *>(ptr))); break;
        default:
            qWarning("Unhandled delete [] of type: %d\n", (int) type);
            break;
        }
    }
}

