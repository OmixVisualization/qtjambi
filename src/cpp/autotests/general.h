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

#ifndef GENERAL_H
#define GENERAL_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtGui/QtGui>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif

template<typename T>
bool initializer_list_equals(const std::initializer_list<T>& a,const std::initializer_list<T>& b){
    if(a.size()==b.size()){
        if(a.begin()==b.begin()){
            return true;
        }

        for(size_t i=0; i<a.size(); ++i){
            if(a.begin()[i]!=b.begin()[i]){
                return false;
            }
        }
        return true;
    }
    return false;
}

template<typename T>
bool operator ==(const std::initializer_list<T>& a,const std::initializer_list<T>& b){
    return initializer_list_equals(a, b);
}

class General {

public:
    static bool canVulkan();
    static bool hasVulkanInstance(QWindow* window);
    static bool canCreateVulkanInstance();

#ifndef QTJAMBI_NO_WIDGETS
    static void callPaintCell(QCalendarWidget *w, QPainter *painter);
    static void callPaintCellNull(QCalendarWidget *w);
#endif

    static QList<bool> start_qtjambi_cast_test(jobject list, jobject qObject, jobject graphicsItem, jobject gradient, jobject functionalPointer, jobject functional, jobject customCList, jobject customJavaList, jobject text);

    static void qtjambi_jni_test(jobject object);

    static void run(QRunnable* runnable);

    static void run(QThread* runnable);

    static void std_terminate();

    static void std_exit(int i);

    static void std_abort();

    static void c_kill();

    static void c_exit(int i);

    static void c_abort();

    static void badAccess();

    static void badVirtual();

    static void uncaughtException();

    static QString stringSysctlByName(const char* name);
};

class FunctionalTest{
public:
	FunctionalTest() : m_last1(), m_last2() {}
    virtual ~FunctionalTest(){}
	
	typedef std::function<int(int,bool)> TestFunction1;
	virtual int convert(const TestFunction1& testFunction){
        m_last1 = testFunction;
        return testFunction ? testFunction(279, true) : -1000;
	}
	
	virtual int convert(const TestFunction1& testFunction, int i, bool b){
        m_last1 = testFunction;
        return testFunction ? testFunction(i, b) : -1000;
    }

    virtual int convertWithLast1(int i, bool b){
        return m_last1 ? m_last1(i, b) : -1000;
    }

    const TestFunction1& last1(){
        return m_last1;
    }

    TestFunction1 anyTestFunction1(){
        return [](int i, bool b)->int{return b ? i*2 : -100;};
    }
	
	typedef std::function<QString(QObject*)> TestFunction2;
    typedef std::function<QString(QObject*)> TestFunction3;
    typedef QString(*TestFunction4)(QObject*);
    typedef QString(*TestFunction5)(QObject*);
	virtual QString convert(const TestFunction2& testFunction){
        m_last2 = testFunction;
        if(testFunction){
            QObject o;
            o.setObjectName("TestObject");
            return testFunction(&o);
        }
        return "";
	}
	
	virtual QString convert(const TestFunction2& testFunction, QObject* object){
        m_last2 = testFunction;
        return testFunction ? testFunction(object) : "";
	}

    virtual QString convertWithLast2(QObject* object){
        return m_last2 ? m_last2(object) : "";
    }
	
    const TestFunction2& last2() const{
        return m_last2;
    }

    TestFunction2 anyTestFunction2(){
        return [](QObject* o)->QString{return o->objectName();};
    }
    TestFunction3 anyTestFunction3(){
        return [](QObject* o)->QString{return o->objectName();};
    }
    TestFunction4 anyTestFunction4(){
        return [](QObject* o)->QString{return o->objectName();};
    }
    TestFunction5 anyTestFunction5(){
        return [](QObject* o)->QString{return o->objectName();};
    }

    QString takeTestFunction2(TestFunction2 fn){
        if(fn){
            QObject o;
            o.setObjectName("TestFunction2");
            return fn(&o);
        }
        return {};
    }
    QString takeTestFunction3(TestFunction3 fn){
        if(fn){
            QObject o;
            o.setObjectName("TestFunction3");
            return fn(&o);
        }
        return {};
    }
    QString takeTestFunction4(TestFunction4 fn){
        if(fn){
            QObject o;
            o.setObjectName("TestFunction4");
            return fn(&o);
        }
        return {};
    }
    QString takeTestFunction5(TestFunction5 fn){
        if(fn){
            QObject o;
            o.setObjectName("TestFunction5");
            return fn(&o);
        }
        return {};
    }

    static QFunctionPointer getFunction(int id);
	
	int m_last_integer;
	QString m_last_string;
	QObject* m_last_object;
protected:
	int m_last_integer2;
	QString m_last_string2;
	QObject* m_last_object2;
private:
    TestFunction1 m_last1;
    TestFunction2 m_last2;
    TestFunction2 m_last3;
};

#endif
