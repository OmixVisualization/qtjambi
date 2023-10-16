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
    static bool canVulkan(){
#if QT_CONFIG(vulkan)
        return true;
#else
        return false;
#endif
    }
    static bool hasVulkanInstance(QWindow* window){
#if QT_CONFIG(vulkan)
        return window->vulkanInstance();
#else
        Q_UNUSED(window)
        return false;
#endif
    }

#ifndef QTJAMBI_NO_WIDGETS
    static void callPaintCell(QCalendarWidget *w, QPainter *painter);
    static void callPaintCellNull(QCalendarWidget *w);
#endif

    static QList<bool> start_qtjambi_cast_test(jobject list, jobject qObject, jobject graphicsItem, jobject gradient, jobject functionalPointer, jobject functional, jobject customCList, jobject customJavaList, jobject text);

    static void qtjambi_jni_test(jobject object);

    static void run(QRunnable* runnable);

    static void run(QThread* runnable);

    static void terminate();

    static void abort();
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
};

#endif
