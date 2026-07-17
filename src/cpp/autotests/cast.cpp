/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "global.h"

#if QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)
#include <QtGui/QVulkanInstance>
#endif
#ifndef QTJAMBI_NO_QUICK
#include <QtQuick/QQuickItem>
#endif
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif

#include <QtJambi/Cast>
#include <QtJambi/ModelCast>
#include <QtJambi/FutureCast>
#include <QtJambi/ArrayCast>
#include <QtJambi/EnumCast>
#include <QtJambi/ArithmeticCast>
#include <QtJambi/SmartPointerCast>
#include <QtJambi/Template1Cast>
#include <QtJambi/Template2Cast>
#include <QtJambi/Template3Cast>
#include <QtJambi/Template4Cast>
#include <QtJambi/Template5Cast>
#include <QtJambi/ContainerCast>
#include <QtJambi/StringAPI>
#include <QtJambi/BufferAPI>
#include <QtJambiQml/Cast>
#include <QtJambiCore/Cast>
#include <QtJambi/QList>
#include <QtJambi/QVariantList>
#include <QtJambi/QObjectList>
#include <QtJambi/QStringList>
#include <QtJambi/QByteArrayList>
#include <QtJambi/QPair>
#include <QtJambi/QSet>
#include <QtJambi/QMap>
#include <QtJambi/QHash>
#include <QtJambi/QMultiMap>
#include <QtJambi/QMultiHash>

struct BoolList : QList<bool>{
};

inline BoolList& operator<<(BoolList& list, bool v){
    list.append(v);
// for debugging purpose
#if 0
    if(!v){
        list.begin();
    }
#endif
    return list;
}

void containerCast(QList<bool>& results, JNIEnv * env, QtJambiScope& scope, jobject list);
void containerCast2(QList<bool>& results, JNIEnv * env, QtJambiScope& scope, jobject list);
void containerCast3(QList<bool>& results, JNIEnv * env, QtJambiScope& scope, jobject list);

QList<bool> General::start_qtjambi_cast_test(JNIEnv * env, jobject list, jobject qObject, jobject graphicsItem, jobject gradient, jobject functionalPointer, jobject functional, jobject customCList, jobject customJavaList, jobject text, jobject utf16Text){
    QtJambiScope scope(nullptr);
    BoolList results;
    {
        QTJAMBI_JNI_LOCAL_FRAME(env,300);
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
            (void)qtjambi_cast<const QEvent*>(env, o);
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
            results << (qtjambi_cast<const QSharedPointer<QLayoutItem>&>(env, scope, o)==item);
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
            results << (qtjambi_cast<const QSharedPointer<QObject>&>(env, scope, o)==ptr);
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
            const QString strg = QLatin1String("test");
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

        containerCast(results, env, scope, list);
        containerCast2(results, env, scope, list);
        containerCast3(results, env, scope, list);
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
            std::string std_strg = qtjambi_cast<std::string>(env, scope, jstring(text));
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, std_strg));
        }
        {
            std::string_view std_strg = qtjambi_cast<std::string_view>(env, scope, jstring(text));
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, std_strg));
        }
        {
            std::u16string std_strg = qtjambi_cast<std::u16string>(env, scope, jstring(utf16Text));
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, std_strg));
        }
        {
            std::u16string_view std_strg = qtjambi_cast<std::u16string_view>(env, scope, jstring(utf16Text));
            QtJambiAPI::addToJavaCollection(env, list, qtjambi_cast<jobject>(env, std_strg));
        }

        {
            Qt::Orientation enm = Qt::Horizontal;
            (void)qtjambi_cast<int>(enm);
            (void)qtjambi_cast<int>(Qt::Horizontal);
            (void)qtjambi_cast<jobject>(QLinearGradient());
            (void)qtjambi_cast<QString>(enm);
            (void)qtjambi_cast<QEvent::Type>(enm);
            int i = 5;
            (void)qtjambi_cast<QString>(i);
            (void)qtjambi_cast<double>(i);
            (void)qtjambi_cast<Qt::Orientation>(i);
            (void)qtjambi_cast<Qt::Orientation>(0);
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
        }
#endif
    }
    return results;
}
