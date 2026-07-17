#include "containerfactory.h"

ContainerFactory::ContainerFactory() {}

QList<QRunnable*> ContainerFactory::createListOfRunnables() {return {};}

QList<QObject*> ContainerFactory::createListOfObjects() {return {};}
QList<QList<QRunnable*>> ContainerFactory::createListOfListOfRunnables() {return {};}
QList<QList<QObject*>> ContainerFactory::createListOfListOfObjects() {return {};}

QList<QEasingCurve::EasingFunction> ContainerFactory::createListOfEasingFunctions() {return {[](qreal r) -> qreal { return r; }};}
void ContainerFactory::testEasingFunctions(const QList<QEasingCurve::EasingFunction>& functions){
    int i=0;
    for(QEasingCurve::EasingFunction fun : functions){
        if(fun)
            fun(i++);
    }
}
QList<ContainerFactory::TestStdFunction> ContainerFactory::createListOfStdFunctions() {return {[](int,bool,double){}};}
void ContainerFactory::testStdFunctions(const QList<ContainerFactory::TestStdFunction>& functions){
    int i=0;
    for(const ContainerFactory::TestStdFunction& fun : functions){
        if(fun){
            fun(i, i%2==1, i);
            ++i;
        }
    }
}

void ContainerFactory::consumeIntList(const QList<int>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}
void ContainerFactory::consumeStringList(const QList<QString>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}
void ContainerFactory::consumeColorList(const QList<QColor>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}
void ContainerFactory::consumeQObjectList(const QList<QObject*>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}

qint64 ContainerFactory::fillIntList(qint32 capacity){
    QList<int> list;
    QTime t1 = QTime::currentTime();
    for (int i = 0; i < capacity; i++) {
        list.append(i);
    }
    QTime t2 = QTime::currentTime();
    return t1.msecsTo(t2);
}
