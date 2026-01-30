/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/qcompilerdetection.h>
#include <QtCore/QSettings>
QT_WARNING_DISABLE_DEPRECATED
#include "pch_p.h"

#ifndef QT_NO_DEBUG
QT_WARNING_DISABLE_GCC("-Wstringop-overflow")

class UnknownKey{
public:
    UnknownKey(int){
//        QTJAMBI_IN_CONSTRUCTOR_CALL("UnknownKey(int)", this)
    }
    void test(){
//        QTJAMBI_JAVA_METHOD_CALL("UnknownKey::test()")
    }
};

//bool operator <(const UnknownKey&,const UnknownKey&){return false;}
//bool operator ==(const UnknownKey&,const UnknownKey&){return false;}
uint qHash(const UnknownKey&){return 0;}

class UnknownClass{
public:
    UnknownClass(int){
        QTJAMBI_IN_CONSTRUCTOR_CALL("UnknownClass(int)")
    }
};
bool operator ==(const UnknownClass&,const UnknownClass&){return false;}
//bool operator <(const UnknownClass&,const UnknownClass&){return false;}
uint qHash(const UnknownClass&){return 0;}
uint qHash(const QMap<QString,int>&){return 0;}
//uint qHash(const QLinkedList<QString,int>&){return 0;}

enum class EnumClass{
    None
};

void test(JNIEnv *env){
    using namespace RegistryAPI;

    QtJambiScope scope;
    enum E{};

    QTJAMBI_TRY{
    }QTJAMBI_CATCH(const JavaException& exn){
        Q_UNUSED(exn)
    }QTJAMBI_TRY_END

    registerEnumTypeInfoNoMetaObject<E>("qt_name", "java_name");
    registerEnumTypeInfoNoMetaObject<E>("qt_name", "java_name", "flags_qt_name", "flags_qt_name_alias", "flags_java_name");

    jstring js = nullptr;
    jobject jo = nullptr;
    {
        QString s;
        qtjambi_cast<jobject>(env, s);
        qtjambi_cast<jcoreobject>(env, s);
        qtjambi_cast<jstring>(env, s);
        qtjambi_cast<jstring>(env, &s);

        qtjambi_cast<QString>(env, js);
        qtjambi_cast<const QString>(env, js);
        qtjambi_cast<const QString*>(env, scope, js);
        qtjambi_cast<const QString&>(env, scope, js);

        qtjambi_cast<QString>(env, jo);
        qtjambi_cast<const QString>(env, jo);
        qtjambi_cast<QString*>(env, jo);
        qtjambi_cast<QString&>(env, jo);
        qtjambi_cast<const QString*>(env, jo);
        qtjambi_cast<const QString&>(env, jo);
    }

    {
        QStringView s;
        qtjambi_cast<jobject>(env, s);
        qtjambi_cast<jstring>(env, s);
        qtjambi_cast<jstring>(env, &s);

        qtjambi_cast<QStringView>(env, scope, js);
        qtjambi_cast<const QStringView>(env, scope, js);
        qtjambi_cast<QStringView*>(env, scope, js);
        qtjambi_cast<QStringView&>(env, scope, js);
        qtjambi_cast<const QStringView*>(env, scope, js);
        qtjambi_cast<const QStringView&>(env, scope, js);

        qtjambi_cast<QStringView>(env, jo);
        qtjambi_cast<const QStringView>(env, jo);
        qtjambi_cast<QStringView*>(env, scope, jo);
        qtjambi_cast<QStringView&>(env, scope, jo);
        qtjambi_cast<const QStringView*>(env, scope, jo);
        qtjambi_cast<const QStringView&>(env, scope, jo);
    }

    {
        QLatin1String s;
        qtjambi_cast<jobject>(env, s);
        qtjambi_cast<jstring>(env, s);
        qtjambi_cast<jstring>(env, &s);

        qtjambi_cast<QLatin1String>(env, scope, js);
        qtjambi_cast<const QLatin1String>(env, scope, js);
        qtjambi_cast<QLatin1String*>(env, scope, js);
        qtjambi_cast<QLatin1String&>(env, scope, js);
        qtjambi_cast<const QLatin1String*>(env, scope, js);
        qtjambi_cast<const QLatin1String&>(env, scope, js);

        qtjambi_cast<QLatin1String>(env, jo);
        qtjambi_cast<const QLatin1String>(env, jo);
        qtjambi_cast<QLatin1String&>(env, scope, jo);
        qtjambi_cast<const QLatin1String&>(env, scope, jo);
    }

    {
        QByteArray s;
        qtjambi_cast<jobject>(env, s);
        qtjambi_cast<jstring>(env, s);
        qtjambi_cast<jstring>(env, &s);

        qtjambi_cast<QByteArray>(env, js);
        qtjambi_cast<const QByteArray>(env, js);
        qtjambi_cast<const QByteArray*>(env, scope, js);
        qtjambi_cast<const QByteArray&>(env, scope, js);
    }

    {
        const char* s{nullptr};
        qtjambi_cast<jobject>(env, s);
        qtjambi_cast<jstring>(env, s);
        qtjambi_cast<jstring>(env, &s);
    }
    {
        QByteArrayView s;
        qtjambi_cast<jobject>(env, s);
        qtjambi_cast<jstring>(env, s);
        qtjambi_cast<jstring>(env, &s);

        qtjambi_cast<QByteArrayView>(env, scope, js);
        qtjambi_cast<const QByteArrayView>(env, scope, js);
        qtjambi_cast<QByteArrayView*>(env, scope, js);
        qtjambi_cast<QByteArrayView&>(env, scope, js);
        qtjambi_cast<const QByteArrayView*>(env, scope, js);
        qtjambi_cast<const QByteArrayView&>(env, scope, js);
    }

    {
        QAnyStringView s;
        qtjambi_cast<jobject>(env, s);
        qtjambi_cast<jstring>(env, s);
        qtjambi_cast<jstring>(env, &s);

        qtjambi_cast<QAnyStringView>(env, scope, js);
        qtjambi_cast<const QAnyStringView>(env, scope, js);
        qtjambi_cast<QAnyStringView*>(env, scope, js);
        qtjambi_cast<QAnyStringView&>(env, scope, js);
        qtjambi_cast<const QAnyStringView*>(env, scope, js);
        qtjambi_cast<const QAnyStringView&>(env, scope, js);

        qtjambi_cast<QAnyStringView>(env, jo);
        qtjambi_cast<const QAnyStringView>(env, jo);
        qtjambi_cast<QAnyStringView*>(env, scope, jo);
        qtjambi_cast<QAnyStringView&>(env, scope, jo);
        qtjambi_cast<const QAnyStringView*>(env, scope, jo);
        qtjambi_cast<const QAnyStringView&>(env, scope, jo);
    }

    {
        QUtf8StringView s;
        qtjambi_cast<jobject>(env, s);
        qtjambi_cast<jstring>(env, s);
        qtjambi_cast<jstring>(env, &s);

        qtjambi_cast<QUtf8StringView>(env, scope, js);
        qtjambi_cast<const QUtf8StringView>(env, scope, js);
        qtjambi_cast<QUtf8StringView*>(env, scope, js);
        qtjambi_cast<QUtf8StringView&>(env, scope, js);
        qtjambi_cast<const QUtf8StringView*>(env, scope, js);
        qtjambi_cast<const QUtf8StringView&>(env, scope, js);

        qtjambi_cast<QUtf8StringView>(env, jo);
        qtjambi_cast<const QUtf8StringView>(env, jo);
        qtjambi_cast<QUtf8StringView*>(env, scope, jo);
        qtjambi_cast<QUtf8StringView&>(env, scope, jo);
        qtjambi_cast<const QUtf8StringView*>(env, scope, jo);
        qtjambi_cast<const QUtf8StringView&>(env, scope, jo);
    }
    {
        qtjambi_cast<jobject>(env, QFutureInterface<void>());
        qtjambi_cast<jobject>(env, QFutureInterface<int>());
        qtjambi_cast<jobject>(env, QFutureInterface<QVariant>());
        qtjambi_cast<QFutureInterface<void>>(env, jobject(nullptr));
        qtjambi_cast<QFutureInterface<int>>(env, jobject(nullptr));
        qtjambi_cast<QFutureInterface<QVariant>>(env, jobject(nullptr));
        qtjambi_cast<const QFutureInterface<void>&>(env, scope, jobject(nullptr));
        qtjambi_cast<const QFutureInterface<int>&>(env, scope, jobject(nullptr));
        qtjambi_cast<const QFutureInterface<QVariant>&>(env, scope, jobject(nullptr));
        qtjambi_cast<QFutureInterface<void>&>(env, scope, jobject(nullptr));
        qtjambi_cast<QFutureInterface<int>&>(env, scope, jobject(nullptr));
        qtjambi_cast<QFutureInterface<QVariant>&>(env, scope, jobject(nullptr));
    }
    {
        qtjambi_cast<jobject>(env, QFuture<void>());
        qtjambi_cast<jobject>(env, QFuture<int>());
        qtjambi_cast<jobject>(env, QFuture<QVariant>());
        qtjambi_cast<QFuture<void>>(env, jobject(nullptr));
        qtjambi_cast<QFuture<int>>(env, jobject(nullptr));
        qtjambi_cast<QFuture<QVariant>>(env, scope, jobject(nullptr));
        qtjambi_cast<const QFuture<void>&>(env, scope, jobject(nullptr));
        qtjambi_cast<const QFuture<int>&>(env, scope, jobject(nullptr));
        qtjambi_cast<const QFuture<QVariant>&>(env, scope, jobject(nullptr));
        qtjambi_cast<QFuture<void>&>(env, scope, jobject(nullptr));
        qtjambi_cast<QFuture<int>&>(env, scope, jobject(nullptr));
        qtjambi_cast<QFuture<QVariant>&>(env, scope, jobject(nullptr));
    }
    {
        void* ptr = nullptr;
        qtjambi_cast<jobject>(env, *reinterpret_cast<QPromise<void>*>(ptr));
        qtjambi_cast<jobject>(env, scope, *reinterpret_cast<QPromise<int>*>(ptr));
        qtjambi_cast<jobject>(env, *reinterpret_cast<QPromise<QVariant>*>(ptr));
        qtjambi_cast<QPromise<void>&>(env, scope, jobject(nullptr));
        qtjambi_cast<QPromise<int>&>(env, scope, jobject(nullptr));
        qtjambi_cast<QPromise<QVariant>&>(env, scope, jobject(nullptr));
    }
    {
        qtjambi_cast<jobject>(env, std::vector<int>());
        qtjambi_cast<std::vector<int>>(env, jobject(nullptr));
        qtjambi_cast<jobject>(env, QWeakPointer<QPoint>());
        qtjambi_cast<QWeakPointer<QPoint>>(env, jobject(nullptr));
        qtjambi_cast<jobject>(env, std::vector<QWeakPointer<QPoint>>());
        qtjambi_cast<std::vector<QWeakPointer<QPoint>>>(env, jobject(nullptr));
    }
    {
        QList<int> list;
        QDataStream stream;
        stream << list;
        stream >> list;
        Q_STATIC_ASSERT(QtJambiPrivate::supports_stream_operators<QList<int>>::value);
        Q_STATIC_ASSERT(!QtJambiPrivate::supports_stream_operators<QList<UnknownClass>>::value);
    }
    {
        QtJambiUtils::QHashFunction hashFunction1;
        QtJambiUtils::QHashFunction hashFunction2 = [](const void*, size_t)->size_t{ return 0; };
        QtJambiUtils::QHashFunction hashFunction3 = hashFunction2;
        hashFunction3 = [](const void*, size_t)->size_t{ return 0; };
        QtJambiUtils::QHashFunction hashFunction4(QtJambiUtils::QHashFunction([](const void*, size_t)->size_t{ return 0; }));
        hashFunction3(nullptr,0);
        QtJambiUtils::QHashFunction hashFunction5 = nullptr;
    }
    {
        QtJambiUtils::InternalToExternalConverter c1;
        QtJambiUtils::InternalToExternalConverter c2 = c1;
        QtJambiUtils::InternalToExternalConverter c3 = [](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{return false;};
        QtJambiUtils::InternalToExternalConverter c4([](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{return false;});
        QtJambiUtils::InternalToExternalConverter c5(QtJambiUtils::InternalToExternalConverter([](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{return false;}));
        QtJambiUtils::InternalToExternalConverter c6 = nullptr;
    }
    {
        QtJambiUtils::ExternalToInternalConverter c1;
        QtJambiUtils::ExternalToInternalConverter c2 = c1;
        QtJambiUtils::ExternalToInternalConverter c3 = [](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType)->bool{return false;};
        QtJambiUtils::ExternalToInternalConverter c4([](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType)->bool{return false;});
        QtJambiUtils::ExternalToInternalConverter c5(QtJambiUtils::ExternalToInternalConverter([](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType)->bool{return false;}));
        QtJambiUtils::ExternalToInternalConverter c6 = nullptr;
    }
    {
        QtJambiUtils::Runnable c1;
        QtJambiUtils::Runnable c2 = c1;
        QtJambiUtils::Runnable c3 = [](){};
        QtJambiUtils::Runnable c4([](){});
        QtJambiUtils::Runnable c5(QtJambiUtils::Runnable([](){}));
        QtJambiUtils::Runnable c6 = nullptr;
    }
    {
        QStringList l;
        const QStringList& lr = l;
        qtjambi_cast<jobject>(env, l);
        QtJambiScope __qtjambi_scope;
        qtjambi_cast<jobject>(env, __qtjambi_scope, lr);
    }
    {
        jobject list(nullptr);
        QtJambiScope __qtjambi_scope;
        qtjambi_cast<const QList<QPair<QString, QString>>&>(env, __qtjambi_scope, list);
        qtjambi_cast<QList<QPair<QString, QString>>>(env, __qtjambi_scope, list);
        qtjambi_cast<QList<QPair<QString, QString>>&>(env, __qtjambi_scope, list);
        qtjambi_cast<QList<QPair<QString, QString>>*>(env, __qtjambi_scope, list);
        qtjambi_cast<const QList<QPair<QString, QString>*>&>(env, __qtjambi_scope, list);
        qtjambi_cast<QList<QPair<QString, QString>*>>(env, __qtjambi_scope, list);
        qtjambi_cast<QList<QPair<QString, QString>*>&>(env, __qtjambi_scope, list);
        qtjambi_cast<QList<QPair<QString, QString>*>*>(env, __qtjambi_scope, list);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        qtjambi_cast<const std::map<QString,QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::map<QString,QString>>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::map<QString,QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::map<QString,QString>*>(env, __qtjambi_scope, obj);
        std::map<QString,QString> map;
        qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        qtjambi_cast<const std::multimap<QString,QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::multimap<QString,QString>>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::multimap<QString,QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::multimap<QString,QString>*>(env, __qtjambi_scope, obj);
        std::multimap<QString,QString> map;
        qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        qtjambi_cast<const std::unordered_map<QString,QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::unordered_map<QString,QString>>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::unordered_map<QString,QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::unordered_map<QString,QString>*>(env, __qtjambi_scope, obj);
        std::unordered_map<QString,QString> map;
        qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        qtjambi_cast<const std::unordered_set<QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::unordered_set<QString>>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::unordered_set<QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::unordered_set<QString>*>(env, __qtjambi_scope, obj);
        std::unordered_set<QString> map;
        qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        qtjambi_cast<const std::set<QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::set<QString>>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::set<QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::set<QString>*>(env, __qtjambi_scope, obj);
        std::set<QString> map;
        qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        qtjambi_cast<const std::multiset<QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::multiset<QString>>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::multiset<QString>&>(env, __qtjambi_scope, obj);
        qtjambi_cast<std::multiset<QString>*>(env, __qtjambi_scope, obj);
        std::multiset<QString> map;
        qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
    QString string;
    qtjambi_cast<uchar>(env, string);
    qtjambi_cast<char>(env, string);
    qtjambi_cast<ushort>(env, string);
    qtjambi_cast<short>(env, string);
    qtjambi_cast<uint>(env, string);
    qtjambi_cast<int>(env, string);
    qtjambi_cast<ulong>(env, string);
    qtjambi_cast<long>(env, string);
    qtjambi_cast<qulonglong>(env, string);
    qtjambi_cast<qlonglong>(env, string);
    qtjambi_cast<bool>(env, string);
    qtjambi_cast<float>(env, string);
    qtjambi_cast<double>(env, string);
    qtjambi_cast<jstring>(env, string);
    qtjambi_cast<jobject>(env, string);
    qtjambi_cast<Qt::Orientation>(env, string);
    }
    {
    QString _string;
    QString& string = _string;
    qtjambi_cast<uchar>(env, string);
    qtjambi_cast<char>(env, string);
    qtjambi_cast<ushort>(env, string);
    qtjambi_cast<short>(env, string);
    qtjambi_cast<uint>(env, string);
    qtjambi_cast<int>(env, string);
    qtjambi_cast<ulong>(env, string);
    qtjambi_cast<long>(env, string);
    qtjambi_cast<qulonglong>(env, string);
    qtjambi_cast<qlonglong>(env, string);
    qtjambi_cast<bool>(env, string);
    qtjambi_cast<float>(env, string);
    qtjambi_cast<double>(env, string);
    qtjambi_cast<jstring>(env, string);
    qtjambi_cast<jobject>(env, string);
    }
    {
    QChar c('a');
    qtjambi_cast<uchar>(env, c);
    qtjambi_cast<char>(env, c);
    qtjambi_cast<ushort>(env, c);
    qtjambi_cast<short>(env, c);
    qtjambi_cast<uint>(env, c);
    qtjambi_cast<int>(env, c);
    qtjambi_cast<ulong>(env, c);
    qtjambi_cast<long>(env, c);
    qtjambi_cast<qulonglong>(env, c);
    qtjambi_cast<qlonglong>(env, c);
    qtjambi_cast<bool>(env, c);
    qtjambi_cast<float>(env, c);
    qtjambi_cast<double>(env, c);
    qtjambi_cast<jobject>(env, c);
    }
    {
    QLatin1Char c('a');
    qtjambi_cast<uchar>(env, c);
    qtjambi_cast<char>(env, c);
    qtjambi_cast<ushort>(env, c);
    qtjambi_cast<short>(env, c);
    qtjambi_cast<uint>(env, c);
    qtjambi_cast<int>(env, c);
    qtjambi_cast<ulong>(env, c);
    qtjambi_cast<long>(env, c);
    qtjambi_cast<qulonglong>(env, c);
    qtjambi_cast<qlonglong>(env, c);
    qtjambi_cast<bool>(env, c);
    qtjambi_cast<float>(env, c);
    qtjambi_cast<double>(env, c);
    qtjambi_cast<jobject>(env, c);
    }
    {
    wchar_t c('a');
    qtjambi_cast<uchar>(env, c);
    qtjambi_cast<char>(env, c);
    qtjambi_cast<ushort>(env, c);
    qtjambi_cast<short>(env, c);
    qtjambi_cast<uint>(env, c);
    qtjambi_cast<int>(env, c);
    qtjambi_cast<ulong>(env, c);
    qtjambi_cast<long>(env, c);
    qtjambi_cast<qulonglong>(env, c);
    qtjambi_cast<qlonglong>(env, c);
    qtjambi_cast<bool>(env, c);
    qtjambi_cast<float>(env, c);
    qtjambi_cast<double>(env, c);
    qtjambi_cast<jobject>(env, c);
    }
    jbyte b = 0;
    qtjambi_cast<jobject>(env, b);
    QObject* o(nullptr);
    qtjambi_cast<jobject>(env, o);
    {
    jobject jo(nullptr);
    qtjambi_cast<QObject*>(env, jo);
    qtjambi_cast<QSize>(env, jo);
    qtjambi_cast<QString>(env, jo);
    qtjambi_cast<QList<QSize>>(env, jo);
    qtjambi_cast<QStringList>(env, jo);
    }
    {
        QSize s;
        qtjambi_cast<jobject>(env, s);
    }
    {
        QList<QSize> s;
        qtjambi_cast<jobject>(env, s);
    }
    {
        Qt::Orientation o = Qt::Orientation(0);
        qtjambi_cast<jobject>(env, o);
        qtjambi_cast<int>(env, o);
        qtjambi_cast<QString>(env, o);
    }
    {
        Qt::Orientations o;
        qtjambi_cast<jobject>(env, o);
        qtjambi_cast<int>(env, o);
    }
    {
        int i = 0;
        qtjambi_cast<Qt::Orientation>(env, i);
        qtjambi_cast<Qt::Orientations>(env, i);
    }

    QList<bool> results;
    {
        {
            QList<int> qlist;
            qlist << 4 << 6 << 12;
            jobject o = qtjambi_cast<jobject>(env, qlist);
            results << (qtjambi_cast<QList<int>>(env, o)==qlist);
            results << (qtjambi_cast<const QList<int>>(env, o)==qlist);
            //results << (qtjambi_cast<const QList<int>&>(env, o)==qlist); disallowed
        }
        {
            QStringList qlist;
            qlist << "4" << "6" << "12";
            jobject o = qtjambi_cast<jobject>(env, qlist);

            results << (qtjambi_cast<QStringList>(env, o)==qlist);
            results << (qtjambi_cast<const QStringList>(env, o)==qlist);
            //results << (qtjambi_cast<const QStringList&>(env, o)==qlist); disallowed
        }
        {
            QByteArrayList qlist;
            qlist << "A" << "B" << "C";
            jobject o = qtjambi_cast<jobject>(env, qlist);

            results << (qtjambi_cast<QByteArrayList>(env, o)==qlist);
            results << (qtjambi_cast<const QByteArrayList>(env, o)==qlist);
            //results << (qtjambi_cast<const QByteArrayList&>(env, o)==qlist); // this does not work because of returning a reference to local variable.
        }
        {
            QUrl::FormattingOptions option = QUrl::DecodeReserved | QUrl::RemoveQuery;
            jobject o = qtjambi_cast<jobject>(env, option);
            jint i  = qtjambi_cast<jint>(env, option);

            results << (qtjambi_cast<QUrl::FormattingOptions>(env, o)==option);
            results << (qtjambi_cast<const QUrl::FormattingOptions>(env, o)==option);
            //results << (qtjambi_cast<const QUrl::FormattingOptions&>(env, o)==option); disallowed
            results << (qtjambi_cast<QUrl::FormattingOptions>(env, i)==option);
            results << (qtjambi_cast<const QUrl::FormattingOptions>(env, i)==option);
            //results << (qtjambi_cast<const QUrl::FormattingOptions&>(env, i)==option); disallowed
        }
        {
            QHash<QString,QFileInfo> qmap;
            qmap["test"] = QFileInfo("test");
            qmap["path"] = QFileInfo("path");
            qmap["hash"] = QFileInfo("hash");
            jobject o = qtjambi_cast<jobject>(env, qmap);

            results << (qtjambi_cast<QHash<QString,QFileInfo>>(env, o)==qmap);
            results << (qtjambi_cast<const QHash<QString,QFileInfo>>(env, o)==qmap);
            //results << (qtjambi_cast<const QHash<QString,QFileInfo>&>(env, o)==qmap);// this does not work because of returning a reference to local variable.
        }
        {
            double d = 2.3;
            jobject o = qtjambi_cast<jobject>(env, d);

            results << qFuzzyCompare(qtjambi_cast<double>(env, o), d);
            results << qFuzzyCompare(qtjambi_cast<const double>(env, o), d);
            //results << qFuzzyCompare(qtjambi_cast<const double&>(env, o), d); disallowed
        }
        {
            int i = 41;
            jobject o = qtjambi_cast<jobject>(env, i);

            results << (qtjambi_cast<int>(env, o) == i);
        }
        {
            jint i = Qt::ExtraButton6;
            Qt::MouseButton o = qtjambi_cast<Qt::MouseButton>(env, i);
            results << (qtjambi_cast<int>(env, o) == i);
        }
        {
            QMap<QString,QFileInfo> qmap;
            qmap["test"] = QFileInfo("test");
            qmap["path"] = QFileInfo("path");
            qmap["map"] = QFileInfo("map");
            jobject o = qtjambi_cast<jobject>(env, qmap);

            results << (qtjambi_cast<QMap<QString,QFileInfo>>(env, o)==qmap);
            results << (qtjambi_cast<const QMap<QString,QFileInfo>>(env, o)==qmap);
            //results << (qtjambi_cast<const QMap<QString,QFileInfo>&>(env, o)==qmap); // this does not work because of returning a reference to local variable.
        }
        {
            jobject o = qtjambi_cast<jobject>(env, &QObject::staticMetaObject);

            results << (qtjambi_cast<const QMetaObject*>(env, o) == &QObject::staticMetaObject);
        }

        {
            jobject o = qtjambi_cast<jobject>(env, QObject::staticMetaObject.property(0));

            results << (qtjambi_cast<QMetaProperty>(env, o).name() == QObject::staticMetaObject.property(0).name());
        }

        {
            QScopedPointer<double> widget(new double(9.876));
            jobject o = qtjambi_cast<jobject>(env, widget);

            results << qFuzzyCompare(qtjambi_cast<double>(env, o), 9.876);
        }

        {
            QBuffer buffer;
            QIODevice& device = buffer;
            jobject o = qtjambi_cast<jobject>(env, device);

            results << (&qtjambi_cast<QIODevice&>(env, o)==&device);
        }

        {
            QList<QObject const*> container;
            container << QCoreApplication::instance();
            container << QAbstractEventDispatcher::instance();
            container << QThread::currentThread();
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            QList<QObject const*>& recast = qtjambi_cast<QList<QObject const*>&>(env, scope, o);
            results << (&recast==&container);
        }

        {
            const QList<QVariant> container{8,7,6,5,4.3f};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QList<QVariant>&>(env, scope, o)==&container);
        }

        {
            QSet<QString> container;
            container << "set";
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<QSet<QString>&>(env, scope, o)==&container);
        }

        {
            const QSet<QObject const*> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QSet<QObject const*>&>(env, scope, o)==&container);
        }

        {
            const QQueue<float> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QQueue<float>&>(env, scope, o)==&container);
        }

        {
            const QVector<float> container{1, 2, 3, 4, 6};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QVector<float>&>(env, scope, o)==&container);
        }

        {
            const QStack<float> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QStack<float>&>(env, scope, o)==&container);
        }

        {
            const QMap<float,QVariant> container{{3.987f, "6"}, {8.f, "3"}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QMap<float,QVariant>&>(env, scope, o)==&container);
        }

        {
            QMap<QString,QVariant> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<QMap<QString,QVariant>&>(env, scope, o)==&container);
        }

        {
            const QHash<QString,int> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QHash<QString,int>&>(env, scope, o)==&container);
        }
        {
            const QMultiHash<QString,int> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QMultiHash<QString,int>&>(env, scope, o)==&container);
        }

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        {
            jintArray array{nullptr};
            qtjambi_cast<QSpan<int>>(env, scope, array);

            qtjambi_cast<QSpan<const int>>(env, scope, jobject{nullptr});
            qtjambi_cast<QSpan<QStringView>>(env, scope, jobject{nullptr});
            qtjambi_cast<QSpan<char>>(env, scope, jobject{nullptr});
            qtjambi_cast<QSpan<const char>>(env, scope, jobject{nullptr});

            QSpan<int> span;
            qtjambi_cast<jintArray>(env, scope, span);
        }
        {
            jintArray array{nullptr};
            qtjambi_cast<QSpan<const int>>(env, scope, array);

            QSpan<const int> span;
            qtjambi_cast<jintArray>(env, span);
        }
        {
            jintArray array{nullptr};
            qtjambi_cast<QSpan<const int>>(env, scope, array);

            QSpan<char> span;
            qtjambi_cast<jobject>(env, span);
        }
#endif
        {
            std::optional<int> opt1;
            qtjambi_cast<jobject>(env, opt1);
            std::optional<QString> opt2;
            qtjambi_cast<jobject>(env, opt2);
            std::optional<char> opt3;
            qtjambi_cast<jobject>(env, opt3);
            jobject o{nullptr};
            qtjambi_cast<std::optional<int>>(env, o);
            qtjambi_cast<std::optional<QString>>(env, o);
            qtjambi_cast<std::optional<char>>(env, o);
        }

        {
            std::initializer_list<int> container{1,2,3,4,5,6,7,8,9};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            qtjambi_cast<std::initializer_list<int>>(env, scope, jobject{nullptr});

            std::initializer_list<int> container2 = qtjambi_cast<std::initializer_list<int>>(env, scope, o);
            results << (QList<int>(container2)==QList<int>(container));
        }
        {
            std::initializer_list<QList<QMap<QString,int>>> container{
                /*QList*/{
                    /*QMap*/{
                        {"A", 5}, {"B", 123}, {"C", 9}
                    },
                    /*QMap*/{
                        {"Y", 6}, {"Z", 39}
                    }
                },
                /*QList*/{}
            };
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            qtjambi_cast<std::initializer_list<QList<QMap<QString,int>>>>(env, scope, o);
        }

        {
            QStringList stringList{"A", "B", "C", "D"};
            QStringList* container = &stringList;
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (qtjambi_cast<QStringList*>(env, scope, o)==container);
        }

        {
            QSharedPointer<QHash<QString,QString>> ptr(new QHash<QString,QString>{ {"A", "B"} });
            QSharedPointer<QHash<QString,QString>>* container = &ptr;
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            QSharedPointer<QHash<QString,QString>>* ptr2 = qtjambi_cast<QSharedPointer<QHash<QString,QString>>*>(env, scope, o);
            results << (ptr2!=nullptr);
            if(ptr2){
                results << (ptr2->data()==ptr.data());
            }
        }

        {
            Qt::Orientation enm = Qt::Horizontal;
            qtjambi_cast<int>(enm);
            qtjambi_cast<int>(Qt::Horizontal);
            qtjambi_cast<QString>(enm);
            qtjambi_cast<QEvent::Type>(enm);
            int i = 5;
            qtjambi_cast<QString>(i);
            qtjambi_cast<double>(i);
            qtjambi_cast<Qt::Orientation>(i);
            qtjambi_cast<Qt::Orientation>(0);
        }

        if(QString("The following lines are for compilation only.").isEmpty()){
            /*{
                QList<const int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QList<const int>>(env, scope, o);
            }*/
            {
                QList<int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QList<int>>(env, scope, o);
            }
            {
                QList<QObject*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QList<QObject*>>(env, scope, o);
            }
            {
                QList<QList<int>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QList<QList<int>>>(env, scope, o);
            }
            {
                QList<QList<QObject*>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QList<QList<QObject*>>>(env, scope, o);
            }
            {
                QMap<int, QRect>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMap<int, QRect>>(env, scope, o);
            }
            {
                QMap<int, QObject*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMap<int, QObject*>>(env, scope, o);
            }
            {
                QMap<QObject*,int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMap<QObject*,int>>(env, scope, o);
            }
            {
                QMap<QObject*,QRunnable*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMap<QObject*,QRunnable*>>(env, scope, o);
            }
            {
                QMap<QObject*,QList<QRunnable*>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMap<QObject*,QList<QRunnable*>>>(env, scope, o);
            }
            {
                QMultiMap<int, QRect>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiMap<int, QRect>>(env, scope, o);
            }
            {
                QMultiMap<int, QObject*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiMap<int, QObject*>>(env, scope, o);
            }
            {
                QMultiMap<QObject*,int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiMap<QObject*,int>>(env, scope, o);
            }
            {
                QMultiMap<QObject*,QRunnable*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiMap<QObject*,QRunnable*>>(env, scope, o);
            }
            {
                QMultiMap<QObject*,QList<QRunnable*>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiMap<QObject*,QList<QRunnable*>>>(env, scope, o);
            }
            {
                QHash<int, QRect>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QHash<int, QRect>>(env, scope, o);
            }
            {
                QHash<int, QObject*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QHash<int, QObject*>>(env, scope, o);
            }
            {
                QHash<QObject*,int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QHash<QObject*,int>>(env, scope, o);
            }
            {
                QHash<QObject*,QRunnable*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QHash<QObject*,QRunnable*>>(env, scope, o);
            }
            {
                QHash<QObject*,QList<QRunnable*>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QHash<QObject*,QList<QRunnable*>>>(env, scope, o);
            }
            {
                QMultiHash<int, QRect>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiHash<int, QRect>>(env, scope, o);
            }
            {
                QMultiHash<int, QObject*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiHash<int, QObject*>>(env, scope, o);
            }
            {
                QMultiHash<QObject*,int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiHash<QObject*,int>>(env, scope, o);
            }
            {
                QMultiHash<QObject*,QRunnable*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiHash<QObject*,QRunnable*>>(env, scope, o);
            }
            {
                QMultiHash<QObject*,QList<QRunnable*>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiHash<QObject*,QList<QRunnable*>>>(env, scope, o);
            }
            /*
            {
                QMap<UnknownKey, UnknownClass>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QMap<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                QHash<UnknownKey,UnknownClass>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QHash<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                const QMap<UnknownKey, UnknownClass>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<const QMap<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                const QHash<UnknownKey,UnknownClass>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<const QHash<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                QMultiMap<UnknownKey, UnknownClass>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QMultiMap<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                QMultiHash<UnknownKey,UnknownClass>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QMultiHash<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                const QMultiMap<UnknownKey, UnknownClass>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<const QMultiMap<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                const QMultiHash<UnknownKey,UnknownClass>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QMultiHash<UnknownKey, UnknownClass>>(env, scope, o);
            }
            {
                QSharedPointer<QMap<UnknownKey, UnknownClass>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QSharedPointer<QMap<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<QHash<UnknownKey,UnknownClass>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QSharedPointer<QHash<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMap<UnknownKey, UnknownClass>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QSharedPointer<const QMap<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QHash<UnknownKey,UnknownClass>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QSharedPointer<const QHash<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<QMultiMap<UnknownKey, UnknownClass>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QSharedPointer<QMultiMap<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<QMultiHash<UnknownKey,UnknownClass>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QSharedPointer<QMultiHash<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiMap<UnknownKey, UnknownClass>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QSharedPointer<const QMultiMap<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiHash<UnknownKey,UnknownClass>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
//                qtjambi_cast<QSharedPointer<const QMultiHash<UnknownKey,UnknownClass>>>(env, scope, o);
            }
*/
            {
                QList<UnknownKey>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QList<UnknownClass>>(env, scope, o);
            }

            {
                QQueue<UnknownKey>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QQueue<UnknownClass>>(env, scope, o);
            }

            {
                QStack<UnknownKey>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QStack<UnknownClass>&>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QVector<UnknownKey>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QVector<UnknownClass>&>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QSet<UnknownClass>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSet<UnknownClass>>(env, scope, o);
            }

            {
                const QList<UnknownKey>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QList<UnknownClass>>(env, scope, o);
            }

            {
                const QQueue<UnknownKey>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QQueue<UnknownClass>>(env, scope, o);
            }

            {
                const QStack<UnknownKey>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<const QStack<UnknownClass>&>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                const QVector<UnknownKey>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<const QVector<UnknownClass>&>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                const QSet<UnknownClass>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QSet<UnknownClass>>(env, scope, o);
            }

            {
                QSharedPointer<QList<UnknownKey>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QList<UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<QQueue<UnknownKey>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QQueue<UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<QStack<UnknownKey>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QSharedPointer<QStack<UnknownClass>>>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QSharedPointer<QVector<UnknownKey>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QSharedPointer<QVector<UnknownClass>>>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QSharedPointer<QSet<UnknownClass>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QSet<UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QList<UnknownKey>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QList<UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QQueue<UnknownKey>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QQueue<UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QStack<UnknownKey>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QSharedPointer<const QStack<UnknownClass>>>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QSharedPointer<const QVector<UnknownKey>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QSharedPointer<const QVector<UnknownClass>>>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QSharedPointer<const QSet<UnknownClass>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QSet<UnknownClass>>>(env, scope, o);
            }

            {
                QMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMap<QString, QString>>(env, scope, o);
            }

            {
                QHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QHash<QString, QString>>(env, scope, o);
            }

            {
                const QMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QMap<QString, QString>>(env, scope, o);
            }

            {
                const QHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QHash<QString, QString>>(env, scope, o);
            }

            {
                QMultiMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiMap<QString, QString>>(env, scope, o);
            }

            {
                QMultiHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiHash<QString, QString>>(env, scope, o);
            }

            {
                const QMultiMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QMultiMap<QString, QString>>(env, scope, o);
            }

            {
                const QMultiHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QMultiHash<QString, QString>>(env, scope, o);
            }
            {
                QSharedPointer<QMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QMap<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QHash<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QMap<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QHash<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QMultiMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QMultiMap<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QMultiHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QMultiHash<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QMultiMap<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QMultiHash<QString,QString>>>(env, scope, o);
            }

            {
                QList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QList<QString>>(env, scope, o);
            }

            {
                QQueue<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QQueue<QString>>(env, scope, o);
            }

            {
                QStack<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QStack<QString>>(env, scope, o);
            }

            {
                QVector<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QVector<QString>>(env, scope, o);
            }

            {
                QSet<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSet<QString>>(env, scope, o);
            }

            {
                const QList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QList<QString>>(env, scope, o);
            }

            {
                const QQueue<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QQueue<QString>>(env, scope, o);
            }

            {
                const QStack<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QStack<QString>>(env, scope, o);
            }

            {
                const QVector<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QVector<QString>>(env, scope, o);
            }

            {
                const QSet<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QSet<QString>>(env, scope, o);
            }

            {
                QSharedPointer<QList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QList<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QQueue<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QQueue<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QStack<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QStack<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QVector<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QVector<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QSet<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QSet<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QList<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QQueue<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QQueue<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QStack<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QStack<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QVector<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QVector<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QSet<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QSet<QString>>>(env, scope, o);
            }

            {
                QWeakPointer<const QSet<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QWeakPointer<const QSet<QString>>>(env, scope, o);
            }

            {
                QMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QMap<QString, QString>*>(env, scope, o);
            }

            {
                QHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QHash<QString, QString>*>(env, scope, o);
            }

            {
                const QMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<const QMap<QString, QString>*>(env, scope, o);
            }

            {
                const QHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<const QHash<QString, QString>*>(env, scope, o);
            }

            {
                QMultiMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QMultiMap<QString, QString>*>(env, scope, o);
            }

            {
                QMultiHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QMultiHash<QString, QString>*>(env, scope, o);
            }

            {
                const QMultiMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<const QMultiMap<QString, QString>*>(env, scope, o);
            }

            {
                const QMultiHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QMultiHash<QString, QString>*>(env, scope, o);
            }
            {
                QSharedPointer<QMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<QMap<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<QHash<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<const QMap<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<const QHash<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QMultiMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<QMultiMap<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QMultiHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<QMultiHash<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<const QMultiMap<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<const QMultiHash<QString,QString>>*>(env, scope, o);
            }

            {
                QList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QList<QString>*>(env, scope, o);
            }

            {
                QQueue<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QQueue<QString>*>(env, scope, o);
            }

            {
                QStack<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QStack<QString>*>(env, scope, o);
            }

            {
                QVector<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QVector<QString>*>(env, scope, o);
            }

            {
                QSet<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSet<QString>*>(env, scope, o);
            }

            {
                const QList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<const QList<QString>*>(env, scope, o);
            }

            {
                const QQueue<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<const QQueue<QString>*>(env, scope, o);
            }

            {
                const QStack<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<const QStack<QString>*>(env, scope, o);
            }

            {
                const QVector<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<const QVector<QString>*>(env, scope, o);
            }

            {
                const QSet<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<const QSet<QString>*>(env, scope, o);
            }

            {
                QSharedPointer<QList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<QList<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QQueue<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<QQueue<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QStack<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<QStack<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QVector<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<QVector<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QSet<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<QSet<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<const QList<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QQueue<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<const QQueue<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QStack<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<const QStack<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QVector<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<const QVector<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QSet<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<const QSet<QString>>*>(env, scope, o);
            }

            {
                QWeakPointer<const QSet<QString>>* container = nullptr;
                qtjambi_cast<jobject>(env, scope, container);
                //qtjambi_cast<QWeakPointer<const QSet<QString>>*>(env, scope, o); //  Cannot cast to QWeakPointer<T> *
            }
            {
                QSettings::ReadFunc fn = [](QIODevice &, QSettings::SettingsMap &)->bool{return false;};
                jobject o = qtjambi_cast<jobject>(env, fn, "QSettings::ReadFunc");
                qtjambi_cast<QSettings::ReadFunc>(env, o);
            }
            {
                std::function<bool(QIODevice &, QSettings::SettingsMap &)> fn = [](QIODevice &, QSettings::SettingsMap &)->bool{return false;};
                jobject o = qtjambi_cast<jobject>(env, fn);
                qtjambi_cast<std::function<bool(QIODevice &, QSettings::SettingsMap &)>>(env, o);
            }
            {
                EnumClass ec = EnumClass::None;
                jobject o = qtjambi_cast<jobject>(env, ec);
                qtjambi_cast<EnumClass>(env, o);
            }
        }
    }
}
#endif
