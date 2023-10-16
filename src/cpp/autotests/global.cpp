/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "global.h"

#include <QtCore/QDebug>
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#ifndef QTJAMBI_NO_QUICK
#include <QtQuick/QQuickItem>
#endif
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif
#include <QtJambi/QtJambiAPI>
#include <QtJambi/registryapi.h>
#include "general.h"
#include <QtJambi/qtjambi_cast.h>

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

#ifndef QTJAMBI_NO_WIDGETS
class CalendarWidgetAccessor: public QCalendarWidget {
public:
    void paintCellAccess(QPainter *p) {
        paintCell(p, QRect(), QDate::currentDate());
    }
};
void General::callPaintCell(QCalendarWidget *w, QPainter *painter) {
    QPainter localPainter;
    if (!painter)
        painter = &localPainter;

    static_cast<CalendarWidgetAccessor *>(w)->paintCellAccess(painter);
}

void General::callPaintCellNull(QCalendarWidget *w) {
    static_cast<CalendarWidgetAccessor *>(w)->paintCellAccess(nullptr);
}
#endif

class Runner : public QThread{
public: void runMe(){run();}
};
void General::run(QThread* runnable){
    if(runnable)static_cast<Runner*>(runnable)->runMe();
}

void General::run(QRunnable* runnable){
    if(runnable)runnable->run();
}

void General::terminate(){
    std::terminate();
}

void General::abort(){
    std::abort();
}

void General::qtjambi_jni_test(jobject object){
    if(JniEnvironment env{128}){
        jclass cls = env->FindClass("Ljava/lang/String;");
        jobject null = nullptr;
        bool isInstance1 = env->IsInstanceOf(null, cls);
        bool isInstance2 = env->IsInstanceOf(object, cls);
        jfieldID mtd = env->GetFieldID(cls, "value", "[B");
        jobject l = env->GetObjectField(object, mtd);
        printf("isInstance1=%i\n", isInstance1);
        printf("isInstance2=%i\n", isInstance2);
        printf("result=%p\n", l);
    }
}

struct BoolList : QList<bool>{
};

inline BoolList& operator<<(BoolList& list, bool v){
    list.append(v);
    if(!v){
        list.begin();
    }
    return list;
}

QList<bool> General::start_qtjambi_cast_test(jobject list, jobject qObject, jobject graphicsItem, jobject gradient, jobject functionalPointer, jobject functional, jobject customCList, jobject customJavaList, jobject text){
    QtJambiScope scope(nullptr);
    BoolList results;
    if(JniEnvironment env{300}){
#ifndef QTJAMBI_NO_WIDGETS
        {
            QWidget* wdg = new QLabel();
            jobject o = qtjambi_cast<jobject>(env, wdg);
            QtJambiAPI::setJavaOwnership(env, o);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QWidget*>(env, o)==wdg);
            results << (qtjambi_cast<const QWidget*>(env, o)==wdg);
        }
        {
            QGraphicsItem* item = new QGraphicsPixmapItem();
            jobject o = qtjambi_cast<jobject>(env, item);
            QtJambiAPI::setJavaOwnership(env, o);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QGraphicsItem*>(env, o)==item);
            results << (qtjambi_cast<const QGraphicsItem*>(env, o)==item);
        }
#endif
        {
            QColor c(0x123456);
            jobject o = qtjambi_cast<jobject>(env, c);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QColor>(env, o)==c);
            results << (qtjambi_cast<const QColor>(env, o)==c);
            results << (qtjambi_cast<const QColor&>(env, o)==c);
        }
        {
            QColor c(0x234567);
            const QColor& cref = c;
            jobject o = qtjambi_cast<jobject>(env, cref);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QColor>(env, o)==c);
        }
        {
            QColor c(0x345678);
            QColor& cref = c;
            jobject o = qtjambi_cast<jobject>(env, cref);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<const QColor>(env, o)==c);
        }
        {
            QEvent* event = new QHideEvent();
            jobject o = qtjambi_cast<jobject>(env, event);
            QtJambiAPI::setJavaOwnership(env, o);
            QtJambiAPI::addToJavaCollection(env, list, o);
            qtjambi_cast<const QEvent*>(env, o);
            results << (qtjambi_cast<QEvent*>(env, o)==event);
        }
        {
            QSharedPointer<QEvent> event(new QHideEvent());
            jobject o = qtjambi_cast<jobject>(env, event);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<const QEvent*>(env, o)==event);
        }
        {
            Qt::ItemFlags flags(Qt::ItemIsDropEnabled | Qt::ItemIsEditable);
            jobject  o = qtjambi_cast<jobject>(env, flags);
            QtJambiAPI::addToJavaCollection(env, list, o);
            jint i = qtjambi_cast<jint>(env, flags);
            jobject wrapper = QtJambiAPI::toJavaIntegerObject(env, i);
            QtJambiAPI::addToJavaCollection(env, list, wrapper);
            results << (qtjambi_cast<Qt::ItemFlags>(env, o)==flags);
            results << (qtjambi_cast<Qt::ItemFlags>(env, wrapper)==flags);
            results << (qtjambi_cast<Qt::ItemFlags>(env, i)==flags);
            results << (qtjambi_cast<const Qt::ItemFlags>(env, o)==flags);
            results << (qtjambi_cast<const Qt::ItemFlags>(env, wrapper)==flags);
            results << (qtjambi_cast<const Qt::ItemFlags>(env, i)==flags);
            //results << (qtjambi_cast<const Qt::ItemFlags&>(env, wrapper)==flags);
            //results << (qtjambi_cast<const Qt::ItemFlags&>(env, i)==flags); disallowed
        }
#ifndef QTJAMBI_NO_WIDGETS
        {
            QSharedPointer<QLayoutItem> item(new QWidgetItem(new QWidget()));
            QWeakPointer<QLayoutItem> ptr = item.toWeakRef();
            jobject o = qtjambi_cast<jobject>(env, ptr);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QWidgetItem*>(env, o)==item.data());
            results << (qtjambi_cast<const QWidgetItem*>(env, o)==item.data());
            results << (qtjambi_cast<QSharedPointer<QLayoutItem>>(env, o)==item);
            results << (qtjambi_cast<const QSharedPointer<QLayoutItem>>(env, o)==item);
            results << (qtjambi_cast<const QSharedPointer<QLayoutItem>&>(env, o)==item);
        }
        {
            QSharedPointer<QObject> item(new QPushButton());
            QWeakPointer<QObject> ptr = item.toWeakRef();
            jobject o = qtjambi_cast<jobject>(env, ptr);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QObject*>(env, o)==item.data());
            results << (qtjambi_cast<QWeakPointer<QObject>>(env, o)==ptr);
            results << (qtjambi_cast<const QWeakPointer<QObject>>(env, o)==ptr);
            //results << (qtjambi_cast<const QWeakPointer<QObject>&>(env, o)==ptr); disallowed
        }
        {
            QSharedPointer<QObject> ptr(new QSlider());
            jobject o = qtjambi_cast<jobject>(env, ptr);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QObject*>(env, o)==ptr.data());
            results << (qtjambi_cast<QSharedPointer<QObject>>(env, o)==ptr);
            results << (qtjambi_cast<const QSharedPointer<QObject>>(env, o)==ptr);
            results << (qtjambi_cast<const QSharedPointer<QObject>&>(env, o)==ptr);
        }
#endif
        {
            QList<int> qlist;
            qlist << 4 << 6 << 12;
            jobject o = qtjambi_cast<jobject>(env, qlist);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QList<int>>(env, o)==qlist);
            results << (qtjambi_cast<const QList<int>>(env, o)==qlist);
            //results << (qtjambi_cast<const QList<int>&>(env, o)==qlist); disallowed
        }
        {
            QPair<int,QBrush> p(8, QBrush(Qt::gray));
            jobject o = qtjambi_cast<jobject>(env, p);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QPair<int,QBrush>>(env, o)==p);
            results << (qtjambi_cast<const QPair<int,QBrush>>(env, o)==p);
            //results << (qtjambi_cast<const QPair<int,QBrush>&>(env, o)==p); disallowed
        }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        {
            QLinkedList<QObject*> qlist;
            qlist << nullptr << nullptr << new QObject();
            jobject o = qtjambi_cast<jobject>(env, qlist);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QLinkedList<QObject*>>(env, o)==qlist);
            results << (qtjambi_cast<const QLinkedList<QObject*>>(env, o)==qlist);
            //results << (qtjambi_cast<const QLinkedList<QObject*>&>(env, o)==qlist); disallowed
        }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)
        {
            QStringList qlist;
            qlist << "4" << "6" << "12";
            jobject o = qtjambi_cast<jobject>(env, qlist);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QStringList>(env, o)==qlist);
            results << (qtjambi_cast<const QStringList>(env, o)==qlist);
            //results << (qtjambi_cast<const QStringList&>(env, o)==qlist); disallowed
        }
        {
            QByteArrayList qlist;
            qlist << "A" << "B" << "C";
            jobject o = qtjambi_cast<jobject>(env, qlist);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QByteArrayList>(env, o)==qlist);
            results << (qtjambi_cast<const QByteArrayList>(env, o)==qlist);
            //results << (qtjambi_cast<const QByteArrayList&>(env, o)==qlist); // this does not work because of returning a reference to local variable.
        }
        {
            QUrl::FormattingOptions option = QUrl::DecodeReserved | QUrl::RemoveQuery;
            jobject o = qtjambi_cast<jobject>(env, option);
            jint i  = qtjambi_cast<jint>(env, option);
            QtJambiAPI::addToJavaCollection(env, list, o);
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
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QHash<QString,QFileInfo>>(env, o)==qmap);
            results << (qtjambi_cast<const QHash<QString,QFileInfo>>(env, o)==qmap);
            //results << (qtjambi_cast<const QHash<QString,QFileInfo>&>(env, o)==qmap);// this does not work because of returning a reference to local variable.
        }
        {
            double d = 2.3;
            jobject o = qtjambi_cast<jobject>(env, d);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << qFuzzyCompare(qtjambi_cast<double>(env, o), d);
            results << qFuzzyCompare(qtjambi_cast<const double>(env, o), d);
            //results << qFuzzyCompare(qtjambi_cast<const double&>(env, o), d); disallowed
        }
        {
            int i = 41;
            jobject o = qtjambi_cast<jobject>(env, i);
            QtJambiAPI::addToJavaCollection(env, list, o);
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
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QMap<QString,QFileInfo>>(env, o)==qmap);
            results << (qtjambi_cast<const QMap<QString,QFileInfo>>(env, o)==qmap);
            //results << (qtjambi_cast<const QMap<QString,QFileInfo>&>(env, o)==qmap); // this does not work because of returning a reference to local variable.
        }
        {
            const FunctionalTest::TestFunction1 f1 = [](int i,bool b)->int{ return b ? i : -i; };
            jobject o = qtjambi_cast<jobject>(env, f1, "FunctionalTest::TestFunction1");
            QtJambiAPI::addToJavaCollection(env, list, o);
            FunctionalTest::TestFunction1 _f1 = qtjambi_cast<FunctionalTest::TestFunction1>(env, o, "FunctionalTest::TestFunction1");
#ifdef Q_CC_MSVC
            results << (_f1.target<int(int,bool)>() == f1.target<int(int,bool)>());
#endif
            results << (_f1.target_type() == f1.target_type());
            //results << (qtjambi_cast<const FunctionalTest::TestFunction1&>(env, o).target<int(int,bool)>() == f1.target<int(int,bool)>()); disallowed
        }
        {
            QEasingCurve::EasingFunction f1 = [](qreal progress)->qreal{ return progress; };
            jobject o = qtjambi_cast<jobject>(env, f1, "QEasingCurve::EasingFunction");
            QtJambiAPI::addToJavaCollection(env, list, o);
            QEasingCurve::EasingFunction _f1 = qtjambi_cast<QEasingCurve::EasingFunction>(env, o, "QEasingCurve::EasingFunction");
            results << (_f1 == f1);
            //results << (qtjambi_cast<const QEasingCurve::EasingFunction&>(env, o) == f1); disallowed
        }
        {
            QString strg = QLatin1String("test");
            jobject o = qtjambi_cast<jobject>(env, strg);
            QtJambiAPI::addToJavaCollection(env, list, o);
            jstring s = qtjambi_cast<jstring>(env, strg);
            QtJambiAPI::addToJavaCollection(env, list, s);
            results << (qtjambi_cast<QString>(env, o) == strg);
            results << (qtjambi_cast<const QString>(env, o) == strg);
            //results << (qtjambi_cast<const QString&>(env, o) == strg); disallowed
            results << (qtjambi_cast<QString>(env, s) == strg);
            results << (qtjambi_cast<const QString>(env, s) == strg);
            results << (qtjambi_cast<QString>(env, jint(5)) == QString("5"));
        }

        {
            jobject o = qtjambi_cast<jobject>(env, &QObject::staticMetaObject);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<const QMetaObject*>(env, o) == &QObject::staticMetaObject);
        }

#ifndef QTJAMBI_NO_WIDGETS
        {
            jobject o = qtjambi_cast<jobject>(env, QWidget::staticMetaObject.method(10));
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QMetaMethod>(env, o) == QWidget::staticMetaObject.method(10));
        }
#endif

        {
            jobject o = qtjambi_cast<jobject>(env, QObject::staticMetaObject.property(0));
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QMetaProperty>(env, o).name() == QObject::staticMetaObject.property(0).name());
        }

        {
            QStandardItemModel model;
            model.setRowCount(2);
            model.setColumnCount(2);
            jobject o = qtjambi_cast<jobject>(env, model.index(1,0));
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QModelIndex>(env, o) == model.index(1,0));
        }

#ifndef QTJAMBI_NO_WIDGETS
        {
            QPointer<QObject> widget(new QDialog());
            jobject o = qtjambi_cast<jobject>(env, widget);
            QtJambiAPI::setJavaOwnership(env, o);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QWidget*>(env, o)==widget);
        }
#endif

        {
            QScopedPointer<double> widget(new double(9.876));
            jobject o = qtjambi_cast<jobject>(env, widget);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << qFuzzyCompare(qtjambi_cast<double>(env, o), 9.876);
        }

#ifndef QTJAMBI_NO_QUICK
        {
            QQuickItem item;
            jobject o = qtjambi_cast<jobject>(env, item.transform());
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QQmlListProperty<QQuickTransform>>(env, o)==item.transform());
        }
#endif

        {
            QBuffer buffer;
            QIODevice& device = buffer;
            jobject o = qtjambi_cast<jobject>(env, device);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (&qtjambi_cast<QIODevice&>(env, o)==&device);
        }

        {
            QList<QObject const*> container;
            container << QCoreApplication::instance();
            container << QAbstractEventDispatcher::instance();
            container << QThread::currentThread();
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<QList<QObject const*>&>(env, scope, o));
        }

        {
            const QList<QVariant> container{8,7,6,5,4.3f};
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<const QList<QVariant>&>(env, scope, o));
        }

        {
            QSet<QString> container;
            container << "set";
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (container==qtjambi_cast<QSet<QString>&>(env, scope, o));
        }

        {
            const QSet<QObject const*> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<const QSet<QObject const*>&>(env, scope, o)==container);
        }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        {
            QLinkedList<QString> container;
            container << "D";
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QLinkedList<QString>&>(env, scope, o)==container);
        }

        {
            const QLinkedList<QObject*> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<const QLinkedList<QObject*>&>(env, scope, o)==container);
        }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

#ifndef QTJAMBI_NO_WIDGETS
        {
            QQueue<QGraphicsItem const*> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<QQueue<QGraphicsItem const*>&>(env, scope, o));
        }
#endif

        {
            const QQueue<float> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<const QQueue<float>&>(env, scope, o));
        }

#ifndef QTJAMBI_NO_WIDGETS
        {
            QVector<QGraphicsItem const*> container{nullptr, nullptr};
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<QVector<QGraphicsItem const*>&>(env, scope, o));
        }
#endif

        {
            const QVector<float> container{1, 2, 3, 4, 6};
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<const QVector<float>&>(env, scope, o));
        }

#ifndef QTJAMBI_NO_WIDGETS
        {
            QStack<QGraphicsItem const*> container;
            container << nullptr;
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<QStack<QGraphicsItem const*>&>(env, scope, o));
        }
#endif

        {
            const QStack<float> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<const QStack<float>&>(env, scope, o));
        }

        {
            const QMap<float,QVariant> container{{3.987f, "6"}, {8.f, "3"}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<const QMap<float,QVariant>&>(env, scope, o));
        }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        {
            QMultiMap<QVariant,QVariant> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<QMultiMap<QVariant,QVariant>&>(env, scope, o));
        }

        {
            const QMultiMap<QVariant,QVariant> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<const QMultiMap<QVariant,QVariant>&>(env, scope, o));
        }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

        {
            QMap<QString,QVariant> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<QMap<QString,QVariant>&>(env, scope, o));
        }

        {
            const QHash<QString,int> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<const QHash<QString,int>&>(env, scope, o));
        }

#ifndef QTJAMBI_NO_WIDGETS
        {
            QHash<const QGraphicsItem*, int> container;
            container.insert(nullptr, 6);
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<QHash<const QGraphicsItem*, int>&>(env, scope, o));
        }
#endif
        {
            const QMultiHash<QString,int> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<const QMultiHash<QString,int>&>(env, scope, o));
        }

#ifndef QTJAMBI_NO_WIDGETS
        {
            QMultiHash<const QGraphicsItem*, int> container;
            container.insert(nullptr, 5);
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << container.isSharedWith(qtjambi_cast<QMultiHash<const QGraphicsItem*, int>&>(env, scope, o));
        }
#endif
        {
            std::initializer_list<int> container{1,2,3,4,5,6,7,8,9};
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<std::initializer_list<int>>(env, scope, o)==container);
        }
        {
            const std::initializer_list<QList<QMap<QString,int>>> container{
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
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<std::initializer_list<QList<QMap<QString,int>>>>(env, scope, o)==container);
        }

        {
            static QStringList stringList{"A", "B", "C", "D"};
            QStringList* container = &stringList;
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QStringList*>(env, scope, o)==container);
        }

        {
            static QQueue<QColor> q;
            QQueue<QColor>* container = &q;
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            results << (qtjambi_cast<QQueue<QColor>*>(env, scope, o)==container);
        }

        {
            QSharedPointer<QHash<QString,QString>> ptr(new QHash<QString,QString>{ {"A", "B"} });
            QSharedPointer<QHash<QString,QString>>* container = &ptr;
            jobject o = qtjambi_cast<jobject>(env, scope, container);
            QtJambiAPI::addToJavaCollection(env, list, o);
            QSharedPointer<QHash<QString,QString>>* ptr2 = qtjambi_cast<QSharedPointer<QHash<QString,QString>>*>(env, scope, o);
            results << (ptr2!=nullptr);
            if(ptr2){
                results << (ptr2->data()==ptr.data());
            }
        }

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        {
            QUtf8StringView stringView(u8"U8 ö");
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, stringView));
        }
        {
            QAnyStringView stringView(u"U16 Ք");
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, stringView));
        }
        {
            QAnyStringView stringView(u"U16 שּ");
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, stringView));
        }
#endif //QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        {
            QObject* q = qtjambi_cast<QObject*>(env, qObject);
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, q));
        }
        {
            QGraphicsItem* q = qtjambi_cast<QGraphicsItem*>(env, graphicsItem);
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, q));
        }
        {
            const QGradient& q = qtjambi_cast<const QGradient&>(env, gradient);
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, q));
        }
        {
            QEasingCurve::EasingFunction q = qtjambi_cast<QEasingCurve::EasingFunction>(env, functionalPointer, "QEasingCurve::EasingFunction");
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, q));
        }
        {
            FunctionalTest::TestFunction1 q = qtjambi_cast<FunctionalTest::TestFunction1>(env, functional, "FunctionalTest::TestFunction1");
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, q));
        }
        {
            const QList<QString>& q = qtjambi_cast<const QList<QString>&>(env, scope, customCList);
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, &q));
        }
        {
            const QList<QString>& q = qtjambi_cast<const QList<QString>&>(env, scope, customJavaList);
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, q));
        }

        {
            QStringView stringView = qtjambi_cast<QStringView>(env, scope, jstring(text));
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, stringView));
        }

        {
            Qt::Orientation enm = Qt::Horizontal;
            qtjambi_cast<int>(enm);
            qtjambi_cast<int>(Qt::Horizontal);
            qtjambi_cast<jobject>(QLinearGradient());
            qtjambi_cast<QString>(enm);
            qtjambi_cast<QEvent::Type>(enm);
            int i = 5;
            qtjambi_cast<QString>(i);
            qtjambi_cast<double>(i);
            qtjambi_cast<Qt::Orientation>(i);
            qtjambi_cast<Qt::Orientation>(0);
        }
#if 0
        if(QString("The following lines are for compilation only.").isEmpty()){
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
#endif
    }
    return results;
}

void fun0(){
    qInfo() << "Hello World!";
}

void fun1(int i){
    qInfo() << "arg: " << i;
}

void fun2(bool b){
    qInfo() << "arg: " << b;
}

void fun3(double d){
    qInfo() << "arg: " << d;
}

void fun4(float f){
    qInfo() << "arg: " << f;
}

void fun5(QChar c){
    qInfo() << "arg: " << c;
}

void fun6(QString strg){
    qInfo() << "arg: " << strg;
}

void fun7(const QString& strg){
    qInfo() << "arg: " << strg;
}

void fun8(QObject* obj){
    qInfo() << "arg: " << (obj ? obj->objectName() : "null");
}

void fun9(std::nullptr_t n){
    qInfo() << "arg: " << (n==nullptr ? "null" : "not null");
}

void fun10(JNIEnv* env){
    qInfo() << "arg: " << quint64(env);
}

void fun11(char, int i){
    qInfo() << "arg: " << i;
}

void fun12(char, bool b){
    qInfo() << "arg: " << b;
}

void fun13(int, double d){
    qInfo() << "arg: " << d;
}

void fun14(long long, float f){
    qInfo() << "arg: " << f;
}

void fun15(char, QChar c){
    qInfo() << "arg: " << c;
}

void fun16(char, QString strg){
    qInfo() << "arg: " << strg;
}

void fun17(char, const QString& strg){
    qInfo() << "arg: " << strg;
}

void fun18(char, QObject* obj){
    qInfo() << "arg: " << (obj ? obj->objectName() : "null");
}

void fun19(char, std::nullptr_t n){
    qInfo() << "arg: " << (n==nullptr ? "null" : "not null");
}

void fun20(char, JNIEnv* env){
    qInfo() << "arg: " << quint64(env);
}

int fun21(int i){
    return i;
}

bool fun22(bool b){
    return b;
}

double fun23(double d){
    return d;
}

float fun24(float f){
    return f;
}

QChar fun25(QChar c){
    return c;
}

QString fun26(QString strg){
    return strg;
}

QString fun27(const QString& strg){
    return strg;
}

QObject* fun28(QObject* obj){
    return obj;
}

QColor fun29(){
    return QColor(Qt::darkBlue);
}

QList<QVariant> fun30(QString s, QObject* o, qint64 j, qint16 i16, double d, const QColor& color, const QStringList& list, QRectF rect, QFont font){
    return {QVariant::fromValue(s),
                QVariant::fromValue(o),
                QVariant::fromValue(j),
                QVariant::fromValue(i16),
                QVariant::fromValue(d),
                QVariant::fromValue(color),
                QVariant::fromValue(list),
                QVariant::fromValue(rect),
                QVariant::fromValue(font)};
}

Qt::AlignmentFlag fun31(Qt::AlignmentFlag flag){
    return flag;
}

Qt::Alignment fun32(Qt::Alignment flag){
    return flag;
}

QDir fun33(const QDir& dir){
    return dir;
}

QVariant fun34(const QVariant& value){
    return value;
}

char16_t fun35(char16_t value){
    return value;
}

wchar_t fun36(wchar_t value){
    return value;
}

void fun37(char c, short s, int i, long long j, Qt::GlobalColor col, Qt::Alignment flag){
    qInfo() << c << " " << s << " " << i << " " << j << " " << col << " " << flag;
}

void fun38(const QDir& arg){
    qInfo() << arg;
}

void fun39(const QColor& arg){
    qInfo() << arg;
}

void fun40(QColor& arg){
    qInfo() << arg;
    arg = QColor(Qt::darkMagenta);
}

void fun41(const QRect& arg){
    qInfo() << arg;
}

void fun42(QRect arg){
    qInfo() << arg;
}

void fun43(QDir arg){
    qInfo() << arg;
}

QRect fun44(){
    qInfo() << "here I am";
    return QRect(8,2,4,9);
}

Qt::Alignment fun45(){
    qInfo() << "here I am";
    return Qt::Alignment(Qt::AlignVCenter | Qt::AlignJustify);
}

void fun46(QMap<QString,QRect> arg){
    qInfo() << arg;
}

void fun47(const QMap<QString,QRect>& arg){
    qInfo() << arg;
}

void fun48(QString& strg){
    qInfo() << strg;
    strg = QLatin1String("Test");
}

void fun49(int* array, int size){
    for(int i=0; i<size; ++i){
        qInfo() << "array[" << i << "] = " << array[i];
        array[i] = i;
    }
}

void fun50(QRectF* array, int size){
    for(int i=0; i<size; ++i){
        qInfo() << "array[" << i << "] = " << array[i];
        array[i] = QRectF(0,0,i+1,i*2+1);
    }
}

void fun51(const char* format,...){
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
}

typedef void (*Runnable)();

Runnable fun52(Runnable runnable){
    if(runnable)
        runnable();
    return runnable;
}

typedef void (*UnknownFunction1)(int);

void fun53(UnknownFunction1 function){
    if(function)
        function(987654321);
}

typedef qlonglong(*UnknownFunction2)(double, int, const QColor&, Qt::Alignment);

qlonglong fun54(UnknownFunction2 function){
    if(function)
        return function(9.12345, 10, Qt::blue, Qt::AlignLeft | Qt::AlignTop);
    return 0;
}

typedef UnknownFunction1(*UnknownFunction3)(UnknownFunction2 f2);

UnknownFunction1 fun55(UnknownFunction3 function){
    if(function)
        return function([](double d, int i, const QColor& c, Qt::Alignment a) -> qlonglong{
            return qlonglong((i + int(c.rgba()) + int(a)) * d);
        });
    return nullptr;
}

QFunctionPointer FunctionalTest::getFunction(int id){
    static QFunctionPointer functionPointers[] = {
        reinterpret_cast<QFunctionPointer>(&fun0),
        reinterpret_cast<QFunctionPointer>(&fun1),
        reinterpret_cast<QFunctionPointer>(&fun2),
        reinterpret_cast<QFunctionPointer>(&fun3),
        reinterpret_cast<QFunctionPointer>(&fun4),
        reinterpret_cast<QFunctionPointer>(&fun5),
        reinterpret_cast<QFunctionPointer>(&fun6),
        reinterpret_cast<QFunctionPointer>(&fun7),
        reinterpret_cast<QFunctionPointer>(&fun8),
        reinterpret_cast<QFunctionPointer>(&fun9),
        reinterpret_cast<QFunctionPointer>(&fun10),
        reinterpret_cast<QFunctionPointer>(&fun11),
        reinterpret_cast<QFunctionPointer>(&fun12),
        reinterpret_cast<QFunctionPointer>(&fun13),
        reinterpret_cast<QFunctionPointer>(&fun14),
        reinterpret_cast<QFunctionPointer>(&fun15),
        reinterpret_cast<QFunctionPointer>(&fun16),
        reinterpret_cast<QFunctionPointer>(&fun17),
        reinterpret_cast<QFunctionPointer>(&fun18),
        reinterpret_cast<QFunctionPointer>(&fun19),
        reinterpret_cast<QFunctionPointer>(&fun20),
        reinterpret_cast<QFunctionPointer>(&fun21),
        reinterpret_cast<QFunctionPointer>(&fun22),
        reinterpret_cast<QFunctionPointer>(&fun23),
        reinterpret_cast<QFunctionPointer>(&fun24),
        reinterpret_cast<QFunctionPointer>(&fun25),
        reinterpret_cast<QFunctionPointer>(&fun26),
        reinterpret_cast<QFunctionPointer>(&fun27),
        reinterpret_cast<QFunctionPointer>(&fun28),
        reinterpret_cast<QFunctionPointer>(&fun29),
        reinterpret_cast<QFunctionPointer>(&fun30),
        reinterpret_cast<QFunctionPointer>(&fun31),
        reinterpret_cast<QFunctionPointer>(&fun32),
        reinterpret_cast<QFunctionPointer>(&fun33),
        reinterpret_cast<QFunctionPointer>(&fun34),
        reinterpret_cast<QFunctionPointer>(&fun35),
        reinterpret_cast<QFunctionPointer>(&fun36),
        reinterpret_cast<QFunctionPointer>(&fun37),
        reinterpret_cast<QFunctionPointer>(&fun38),
        reinterpret_cast<QFunctionPointer>(&fun39),
        reinterpret_cast<QFunctionPointer>(&fun40),
        reinterpret_cast<QFunctionPointer>(&fun41),
        reinterpret_cast<QFunctionPointer>(&fun42),
        reinterpret_cast<QFunctionPointer>(&fun43),
        reinterpret_cast<QFunctionPointer>(&fun44),
        reinterpret_cast<QFunctionPointer>(&fun45),
        reinterpret_cast<QFunctionPointer>(&fun46),
        reinterpret_cast<QFunctionPointer>(&fun47),
        reinterpret_cast<QFunctionPointer>(&fun48),
        reinterpret_cast<QFunctionPointer>(&fun49),
        reinterpret_cast<QFunctionPointer>(&fun50),
        reinterpret_cast<QFunctionPointer>(&fun51),
        reinterpret_cast<QFunctionPointer>(&fun52),
        reinterpret_cast<QFunctionPointer>(&fun53),
        reinterpret_cast<QFunctionPointer>(&fun54),
        reinterpret_cast<QFunctionPointer>(&fun55)
    };
    if(id>=0 && id<int(sizeof(functionPointers))){
        return functionPointers[id];
    }
    return nullptr;
}
