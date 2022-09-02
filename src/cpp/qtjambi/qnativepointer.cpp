/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include "qtjambi_core.h"
#include <QtCore/QtCore>
#include "qtjambi_repository_p.h"
#include "qtjambitypemanager_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambi_application.h"

#include <stdlib.h>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include "qtjambi_cast.h"

/*******************************************************************************
 * Read functions
 */

#if 1
QT_WARNING_DISABLE_GCC("-Wstringop-overflow")

class UnknownKey{
public:
    UnknownKey(int){}
};

//bool operator <(const UnknownKey&,const UnknownKey&){return false;}
//bool operator ==(const UnknownKey&,const UnknownKey&){return false;}
uint qHash(const UnknownKey&){return 0;}

class UnknownClass{
public:
    UnknownClass(int){}
};
bool operator ==(const UnknownClass&,const UnknownClass&){return false;}
//bool operator <(const UnknownClass&,const UnknownClass&){return false;}
uint qHash(const UnknownClass&){return 0;}
uint qHash(const QMap<QString,int>&){return 0;}
//uint qHash(const QLinkedList<QString,int>&){return 0;}


void test(JNIEnv *env){
    QtJambiScope scope;
    enum E{};

    QTJAMBI_TRY{
    }QTJAMBI_CATCH(const JavaException& exn){
        Q_UNUSED(exn)
    }QTJAMBI_TRY_END

    registerEnumTypeInfoNoMetaObject<E>("qt_name", "java_name");
    registerEnumTypeInfoNoMetaObject<E>("qt_name", "java_name", "flags_qt_name", "flags_qt_name_alias", "flags_java_name");
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        void* ptr = nullptr;
        qtjambi_cast<jobject>(env, *reinterpret_cast<QPromise<void>*>(ptr));
        qtjambi_cast<jobject>(env, *reinterpret_cast<QPromise<int>*>(ptr));
        qtjambi_cast<jobject>(env, *reinterpret_cast<QPromise<QVariant>*>(ptr));
        qtjambi_cast<QPromise<void>&>(env, scope, jobject(nullptr));
        qtjambi_cast<QPromise<int>&>(env, scope, jobject(nullptr));
        qtjambi_cast<QPromise<QVariant>&>(env, scope, jobject(nullptr));
#endif
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
        QHashFunction hashFunction1;
        QHashFunction hashFunction2 = [](const void*, hash_type)->hash_type{ return 0; };
        QHashFunction hashFunction3 = hashFunction2;
        hashFunction3 = [](const void*, hash_type)->hash_type{ return 0; };
        QHashFunction hashFunction4(QHashFunction([](const void*, hash_type)->hash_type{ return 0; }));
        hashFunction3(nullptr,0);
    }
    {
        InternalToExternalConverter c1;
        InternalToExternalConverter c2 = c1;
        InternalToExternalConverter c3 = [](JNIEnv*, QtJambiScope*, const void*, jvalue*, bool)->bool{return false;};
        InternalToExternalConverter c4([](JNIEnv*, QtJambiScope*, const void*, jvalue*, bool)->bool{return false;});
        InternalToExternalConverter c5(InternalToExternalConverter([](JNIEnv*, QtJambiScope*, const void*, jvalue*, bool)->bool{return false;}));
    }
    {
        ExternalToInternalConverter c1;
        ExternalToInternalConverter c2 = c1;
        ExternalToInternalConverter c3 = [](JNIEnv*, QtJambiScope*, const jvalue&, void* &, jValueType)->bool{return false;};
        ExternalToInternalConverter c4([](JNIEnv*, QtJambiScope*, const jvalue&, void* &, jValueType)->bool{return false;});
        ExternalToInternalConverter c5(ExternalToInternalConverter([](JNIEnv*, QtJambiScope*, const jvalue&, void* &, jValueType)->bool{return false;}));
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        {
            QLinkedList<QObject*> qlist;
            qlist << nullptr << nullptr << new QObject();
            jobject o = qtjambi_cast<jobject>(env, qlist);
            results << (qtjambi_cast<QLinkedList<QObject*>>(env, o)==qlist);
            results << (qtjambi_cast<const QLinkedList<QObject*>>(env, o)==qlist);
            //results << (qtjambi_cast<const QLinkedList<QObject*>&>(env, o)==qlist); disallowed
        }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)
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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        {
            QLinkedList<QString> container;
            container << "D";
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<QLinkedList<QString>&>(env, scope, o)==&container);
        }

        {
            const QLinkedList<QObject*> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QLinkedList<QObject*>&>(env, scope, o)==&container);
        }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        {
            QMultiMap<QVariant,QVariant> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<QMultiMap<QVariant,QVariant>&>(env, scope, o)==&container);
        }

        {
            const QMultiMap<QVariant,QVariant> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QMultiMap<QVariant,QVariant>&>(env, scope, o)==&container);
        }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

        {
            std::initializer_list<int> container{1,2,3,4,5,6,7,8,9};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                QLinkedList<UnknownKey>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QLinkedList<UnknownClass>>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                const QLinkedList<UnknownKey>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QLinkedList<UnknownClass>>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                QSharedPointer<QLinkedList<UnknownKey>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QLinkedList<UnknownClass>>>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                QSharedPointer<const QLinkedList<UnknownKey>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QLinkedList<UnknownClass>>>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                QLinkedList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QLinkedList<QString>>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                const QLinkedList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<const QLinkedList<QString>>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                QSharedPointer<QLinkedList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<QLinkedList<QString>>>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                QSharedPointer<const QLinkedList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                qtjambi_cast<QSharedPointer<const QLinkedList<QString>>>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                QLinkedList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QLinkedList<QString>*>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                const QLinkedList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<const QLinkedList<QString>*>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                QSharedPointer<QLinkedList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<QLinkedList<QString>>*>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            {
                QSharedPointer<const QLinkedList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                qtjambi_cast<QSharedPointer<const QLinkedList<QString>>*>(env, scope, o);
            }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

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
        }
    }
}
#endif

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readBoolean)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<bool *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jbyte JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readByte)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<qint8 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jchar JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readChar)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<quint16 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jshort JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readShort)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<qint16 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readInt)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<qint32 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readLong)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<qint64 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jfloat JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readFloat)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<float *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jdouble JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readDouble)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<double *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readPointer)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return reinterpret_cast<jlong>((reinterpret_cast<void **>(ptr))[pos]);
}

bool isValueType(JNIEnv * __jni_env, jclass valueType, int* metaTypeId = nullptr);

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readObject)
  (JNIEnv * __jni_env, jclass, jlong ptr, jclass valueType, jlong pos, jboolean readOnly)
{
    try{
        int metaTypeId;
        bool isValue = isValueType(__jni_env, valueType, &metaTypeId);
        if(readOnly && !isValue){
            JavaException::raiseIllegalArgumentException(__jni_env, "Cannot read non-value typed object from read-only buffer." QTJAMBI_STACKTRACEINFO);
        }
        return qtjambi_from_object(__jni_env, &reinterpret_cast<void **>(ptr)[pos], valueType, isValue, false);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

size_t qtjambi_value_size_for_class(JNIEnv *env, jclass object_class);
size_t qtjambi_shell_size_for_class(JNIEnv *env, jclass object_class);

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_fromArray)
  (JNIEnv * env, jclass, jclass valueType, jobjectArray array)
{
    try{
        jsize len = array ? env->GetArrayLength(array) : 0;
        if (len == 0)
            return nullptr;
        int metaTypeId;
        size_t size = qtjambi_value_size_for_class(env, valueType);
        bool isValue = isValueType(env, valueType, &metaTypeId);
        if(size<=0 || !isValue || metaTypeId==QMetaType::UnknownType){
            JavaException::raiseIllegalArgumentException(env, "Array does not contain clonable values." QTJAMBI_STACKTRACEINFO );
        }

        jobject nativePointer = Java::QtJambi::QNativePointer::newInstance(env,
                                               jint(QNativePointer::Type::Byte), size * size_t(len), 1, false);
        char *buf = reinterpret_cast<char *>(qtjambi_to_cpointer(env, nativePointer, 1));
        for (jsize i=0; i<len; ++i) {
            jobject java_object = env->GetObjectArrayElement(array, i);

            void *ptr = nullptr;
            if (java_object) {
                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java_object))
                    ptr = link->pointer();
                else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
                    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );

            }

            if (ptr){
                QMetaType::construct(metaTypeId, buf + size_t(i) * size, ptr);
            }
        }

        return nativePointer;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromB)
  (JNIEnv * env, jclass, jlong ptr, jbyteArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetByteArrayRegion(array, 0, size, reinterpret_cast<jbyte*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromS)
  (JNIEnv * env, jclass, jlong ptr, jshortArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetShortArrayRegion(array, 0, size, reinterpret_cast<jshort*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromI)
  (JNIEnv * env, jclass, jlong ptr, jintArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetIntArrayRegion(array, 0, size, reinterpret_cast<jint*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromL)
  (JNIEnv * env, jclass, jlong ptr, jlongArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetLongArrayRegion(array, 0, size, reinterpret_cast<jlong*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromF)
  (JNIEnv * env, jclass, jlong ptr, jfloatArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetFloatArrayRegion(array, 0, size, reinterpret_cast<jfloat*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromD)
  (JNIEnv * env, jclass, jlong ptr, jdoubleArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetDoubleArrayRegion(array, 0, size, reinterpret_cast<jdouble*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackB)
  (JNIEnv * env, jclass, jlong ptr, jbyteArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetByteArrayRegion(array, 0, size, reinterpret_cast<const jbyte*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackS)
  (JNIEnv * env, jclass, jlong ptr, jshortArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetShortArrayRegion(array, 0, size, reinterpret_cast<const jshort*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackI)
  (JNIEnv * env, jclass, jlong ptr, jintArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetIntArrayRegion(array, 0, size, reinterpret_cast<const jint*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackL)
  (JNIEnv * env, jclass, jlong ptr, jlongArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetLongArrayRegion(array, 0, size, reinterpret_cast<const jlong*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackF)
  (JNIEnv * env, jclass, jlong ptr, jfloatArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetFloatArrayRegion(array, 0, size, reinterpret_cast<const jfloat*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackD)
  (JNIEnv * env, jclass, jlong ptr, jdoubleArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetDoubleArrayRegion(array, 0, size, reinterpret_cast<const jdouble*>(ptr));
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_fromObject)
  (JNIEnv * __jni_env, jclass, jobject object)
{
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, object))
            return qtjambi_from_cpointer(__jni_env, link->pointer(), QNativePointer::Type::Pointer, 1);
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(__jni_env, object))
            Java::QtJambi::QNoNativeResourcesException::throwNew(__jni_env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(__jni_env, object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_toByteBuffer)
  (JNIEnv * env, jclass, jlong ptr, jlong capacity, jboolean readOnly)
{
	jobject buffer{nullptr};
    try{
        buffer = env->NewDirectByteBuffer(reinterpret_cast<void*>(ptr), capacity);
        if(readOnly){
            buffer = Java::Runtime::ByteBuffer::asReadOnlyBuffer(env, buffer);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return buffer;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_fromBuffer)
  (JNIEnv * env, jclass, jobject buffer)
{
    try{
        return Java::QtJambi::QNativePointer::newInstance2(env, buffer, jlong(JBufferConstData(env, buffer, false).data()));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readString)
  (JNIEnv *env, jclass, jlong ptr, jlong pos)
{
    try{
        return qtjambi_from_qstring(env, reinterpret_cast<QString *>(ptr)[pos]);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

/*******************************************************************************
 * Write functions
 */

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeBoolean)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jboolean value)
{
    ((reinterpret_cast<bool *>(ptr))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeByte)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jbyte value)
{
    (((reinterpret_cast<qint8 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeChar)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jchar value)
{
    (((reinterpret_cast<quint16 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeShort)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jshort value)
{
    (((reinterpret_cast<qint16 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeInt)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jint value)
{
    (((reinterpret_cast<qint32 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeLong)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jlong value)
{
    (((reinterpret_cast<qint64 *>(ptr)))[pos]) = value;
}
extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeFloat)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jfloat value)
{
    (((reinterpret_cast<float *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeDouble)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jdouble value)
{
    (((reinterpret_cast<double *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writePointer)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jlong value)
{
    (reinterpret_cast<void **>(ptr))[pos] = reinterpret_cast<void *>(value);
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeObject)
  (JNIEnv * __jni_env, jclass, jlong buf, jlong _knownSize, jclass valueType, jlong pos, jobject value)
{
    try{
        size_t knownSize = size_t(_knownSize);
        size_t size = qtjambi_value_size_for_class(__jni_env, valueType);
        if(size<=0){
            JavaException::raiseIllegalArgumentException(__jni_env, "Object type is not clonable." QTJAMBI_STACKTRACEINFO );
            return;
        }
        if(knownSize % size != 0){
            JavaException::raiseIllegalArgumentException(__jni_env, "Object type does not fit." QTJAMBI_STACKTRACEINFO );
            return;
        }
        if(size*size_t(pos+1) > knownSize){
            JavaException::raiseIllegalArgumentException(__jni_env, "Object type does not fit." QTJAMBI_STACKTRACEINFO );
            return;
        }
        void *ptr = nullptr;
        if (value) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, value))
                ptr = link->pointer();
            else if(Java::QtJambi::QtObjectInterface::isInstanceOf(__jni_env, value))
                Java::QtJambi::QNoNativeResourcesException::throwNew(__jni_env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(__jni_env, value).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }

        if (buf != 0)
            memcpy(reinterpret_cast<char*>(buf) + size_t(pos) * size, ptr, size);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeString)
  (JNIEnv *env, jclass, jlong ptr, jlong pos, jstring value)
{
    try{
        reinterpret_cast<QString *>(ptr)[pos] = qtjambi_to_qstring(env, value);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}


/*******************************************************************************
 * other stuff...
 */


extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_createPointer)
  (JNIEnv *, jobject, jint type, jlong _size, jint indirections)
{
    size_t size = size_t(_size);
    Q_ASSERT(indirections > 0);
    Q_ASSERT(size > 0);

    if (indirections != 1) {
        return reinterpret_cast<jlong>(new void*[size]);
    }

    if (size == 1) {
        switch (type) {
        case jint(QNativePointer::Type::Boolean): return reinterpret_cast<jlong>(new bool);
        case jint(QNativePointer::Type::Byte): return reinterpret_cast<jlong>(new qint8);
        case jint(QNativePointer::Type::Char): return reinterpret_cast<jlong>(new quint16);
        case jint(QNativePointer::Type::Short): return reinterpret_cast<jlong>(new qint16);
        case jint(QNativePointer::Type::Int): return reinterpret_cast<jlong>(new qint32);
        case jint(QNativePointer::Type::Long): return reinterpret_cast<jlong>(new qint64);
        case jint(QNativePointer::Type::Float): return reinterpret_cast<jlong>(new float);
        case jint(QNativePointer::Type::Double): return reinterpret_cast<jlong>(new double);
        case jint(QNativePointer::Type::Pointer): return reinterpret_cast<jlong>(new void *);
        case jint(QNativePointer::Type::String): return reinterpret_cast<jlong>(new QString());
        }
    } else if (size > 1) {
        switch (type) {
        case jint(QNativePointer::Type::Boolean): return reinterpret_cast<jlong>(new bool[size]);
        case jint(QNativePointer::Type::Byte): return reinterpret_cast<jlong>(new qint8[size]);
        case jint(QNativePointer::Type::Char): return reinterpret_cast<jlong>(new quint16[size]);
        case jint(QNativePointer::Type::Short): return reinterpret_cast<jlong>(new qint16[size]);
        case jint(QNativePointer::Type::Int): return reinterpret_cast<jlong>(new qint32[size]);
        case jint(QNativePointer::Type::Long): return reinterpret_cast<jlong>(new qint64[size]);
        case jint(QNativePointer::Type::Float): return reinterpret_cast<jlong>(new float[size]);
        case jint(QNativePointer::Type::Double): return reinterpret_cast<jlong>(new double[size]);
        case jint(QNativePointer::Type::Pointer): return reinterpret_cast<jlong>(new void *[size]);
        case jint(QNativePointer::Type::String): return reinterpret_cast<jlong>(new QString[size]);
        }
    }
    return 0;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_deletePointer)
  (JNIEnv *, jobject, jlong ptr, jint type, jint deleteMode)
{
    if (deleteMode == 0) { // free()
        switch (type) {
        case jint(QNativePointer::Type::Boolean):       free((reinterpret_cast<bool *>(ptr))); break;
        case jint(QNativePointer::Type::Byte):          free((reinterpret_cast<qint8 *>(ptr))); break;
        case jint(QNativePointer::Type::Char):          free((reinterpret_cast<quint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Short):         free((reinterpret_cast<qint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Int):           free((reinterpret_cast<int *>(ptr))); break;
        case jint(QNativePointer::Type::Long):          free((reinterpret_cast<qint64 *>(ptr))); break;
        case jint(QNativePointer::Type::Float):         free((reinterpret_cast<float *>(ptr))); break;
        case jint(QNativePointer::Type::Double):        free((reinterpret_cast<double *>(ptr))); break;
        default:
            qWarning("Unhandled free of type: %d\n", int(type));
            break;
        }

    } else if (deleteMode == 1) { // delete
        switch (type) {
        case jint(QNativePointer::Type::Boolean):       delete ((reinterpret_cast<bool *>(ptr))); break;
        case jint(QNativePointer::Type::Byte):          delete ((reinterpret_cast<qint8 *>(ptr))); break;
        case jint(QNativePointer::Type::Char):          delete ((reinterpret_cast<quint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Short):         delete ((reinterpret_cast<qint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Int):           delete ((reinterpret_cast<int *>(ptr))); break;
        case jint(QNativePointer::Type::Long):          delete ((reinterpret_cast<qint64 *>(ptr))); break;
        case jint(QNativePointer::Type::Float):         delete ((reinterpret_cast<float *>(ptr))); break;
        case jint(QNativePointer::Type::Double):        delete ((reinterpret_cast<double *>(ptr))); break;
        case jint(QNativePointer::Type::String):        delete ((reinterpret_cast<QString *>(ptr))); break;
        default:
            qWarning("Unhandled delete of type: %d\n", int(type));
            break;
        }

    } else if (deleteMode == 2) { // delete []
        switch (type) {
        case jint(QNativePointer::Type::Boolean):       delete [] ((reinterpret_cast<bool *>(ptr))); break;
        case jint(QNativePointer::Type::Byte):          delete [] ((reinterpret_cast<qint8 *>(ptr))); break;
        case jint(QNativePointer::Type::Char):          delete [] ((reinterpret_cast<quint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Short):         delete [] ((reinterpret_cast<qint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Int):           delete [] ((reinterpret_cast<int *>(ptr))); break;
        case jint(QNativePointer::Type::Long):          delete [] ((reinterpret_cast<qint64 *>(ptr))); break;
        case jint(QNativePointer::Type::Float):         delete [] ((reinterpret_cast<float *>(ptr))); break;
        case jint(QNativePointer::Type::Double):        delete [] ((reinterpret_cast<double *>(ptr))); break;
        case jint(QNativePointer::Type::String):        delete [] ((reinterpret_cast<QString *>(ptr))); break;
        default:
            qWarning("Unhandled delete [] of type: %d\n", int(type));
            break;
        }
    }
}

class DirectAddressIODevice : public QIODevice{
public:
    DirectAddressIODevice(qint64 capacity, char* address, bool readOnly, JNIEnv* env, jobject buffer, QObject *parent = nullptr);
    bool open(OpenMode mode);
protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
private:
    const qint64 m_capacity;
    char* const m_address;
    const bool m_readOnly;
    JObjectWrapper m_buffer;
};

DirectAddressIODevice::DirectAddressIODevice(qint64 capacity, char* address, bool readOnly, JNIEnv* env, jobject object, QObject *parent)
    : QIODevice(parent),
      m_capacity(capacity),
      m_address(address),
      m_readOnly(readOnly),
      m_buffer(env, object) {}

bool DirectAddressIODevice::open(OpenMode mode)
{
    if(mode & QIODevice::Append)
        return false;
    if(bool(mode & QIODevice::WriteOnly) == m_readOnly || m_address==nullptr){
        return false;
    }
    return QIODevice::open(mode);
}

qint64 DirectAddressIODevice::readData(char *data, qint64 maxlen)
{
    if((openMode() & QIODevice::ReadOnly) && maxlen>0){
        qint64 len = qMin(maxlen, m_capacity-pos());
        if(len>0)
            memcpy(data, m_address+pos(), size_t(len));
        return qint64(len);
    }
    return -1;
}

qint64 DirectAddressIODevice::writeData(const char *data, qint64 len)
{
    if((openMode() & QIODevice::WriteOnly)){
        qint64 _len = qMin(len, m_capacity-pos());
        if(_len>0)
            memcpy(m_address+pos(), data, size_t(_len));
        return qint64(_len);
    }
    return -1;
}

QIODevice* qtjambi_new_direct_address_device(qint64 capacity, char* address, bool readOnly, JNIEnv* env, jobject buffer, QObject *parent){
    return new DirectAddressIODevice(capacity, address, readOnly, env, buffer, parent);
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_ioDevice)
(JNIEnv *env,
 jobject buffer)
{
    try{
        jint indirections = Java::QtJambi::QNativePointer::indirections(env,buffer);
        if(indirections==1){
            jlong capacity = Java::QtJambi::QNativePointer::byteSize(env,buffer);
            char* address = reinterpret_cast<char*>(Java::QtJambi::QNativePointer::pointer(env,buffer));
            bool readOnly = Java::QtJambi::QNativePointer::isReadOnly(env, buffer);
            if(address && capacity>0){
                QIODevice* device = new DirectAddressIODevice(capacity, address, readOnly, env, buffer);
                return qtjambi_cast<jobject>(env, device);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}
