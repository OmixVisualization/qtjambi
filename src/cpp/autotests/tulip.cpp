#include "tulip.h"

QList<int> Tulip::do_QList_of_int(const QList<int> &l) { return l; }

QStringList Tulip::do_QStringList(const QStringList &l) { return l; }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
QLinkedList<int> Tulip::do_QLinkedList_of_int(const QLinkedList<int> &l) { return l; }
QVector<int> Tulip::do_QVector_of_int(const QVector<int> &l) { return l; }
#endif

QStack<int> Tulip::do_QStack_of_int(const QStack<int> &s) { return s; }

QQueue<int> Tulip::do_QQueue_of_int(const QQueue<int> &q) { return q; }

QSet<int> Tulip::do_QSet_of_int(const QSet<int> &s) { return s; }

QMap<QString, QString> Tulip::do_QMap_of_strings(const QMap<QString, QString> &m) { return m; }

QHash<QString, QString> Tulip::do_QHash_of_strings(const QHash<QString, QString> &h) { return h; }

QPair<int, int> Tulip::do_QPair_of_ints(const QPair<int, int> &p) { return p; }

const QList<QString>& Tulip::constList() const {return m_list;}
QList<QString>& Tulip::listRef() {return m_list;}

const QList<QList<float>>& Tulip::constListList() const {return m_listlist;}
QList<QList<float>>& Tulip::listListRef() {return m_listlist;}

const QQueue<QString>& Tulip::constQueue() const {return m_queue;}
QQueue<QString>& Tulip::queueRef() {return m_queue;}

const QSet<QString>& Tulip::constSet() const {return m_set;}
QSet<QString>& Tulip::setRef() {return m_set;}

const QStack<QPair<int,QString>>& Tulip::constStack() const {return m_stack;}
QStack<QPair<int,QString>>& Tulip::stackRef() {return m_stack;}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
const QLinkedList<QPoint>& Tulip::constLinkedList() const {return m_linkedlist;}
QLinkedList<QPoint>& Tulip::linkedListRef() {return m_linkedlist;}
const QVector<double>& Tulip::constVector() const {return m_vector;}
QVector<double>& Tulip::vectorRef() {return m_vector;}
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

const QMap<QString,QPoint>& Tulip::constMap() const {return m_map;}
QMap<QString,QPoint>& Tulip::mapRef() {return m_map;}

const QHash<int,QPoint>& Tulip::constHash() const {return m_hash;}
QHash<int,QPoint>& Tulip::hashRef() {return m_hash;}

const QMultiMap<QString,QPoint>& Tulip::constMultiMap() const {return m_multimap;}
QMultiMap<QString,QPoint>& Tulip::multiMapRef() {return m_multimap;}

const QMultiHash<int,QPoint>& Tulip::constMultiHash() const {return m_multihash;}
QMultiHash<int,QPoint>& Tulip::multiHashRef() {return m_multihash;}

const QStringList& Tulip::constStringList() const {return m_stringlist;}
QStringList& Tulip::stringListRef() {return m_stringlist;}

const QByteArrayList& Tulip::constByteArrayList() const {return m_bytearraylist;}
QByteArrayList& Tulip::byteArrayListRef() {return m_bytearraylist;}

QList<QRunnable*> Tulip::createListOfRunnables() {return {};}

QList<QEasingCurve::EasingFunction> Tulip::createListOfEasingFunctions() {return {[](qreal r) -> qreal { return r; }};}
void Tulip::testEasingFunctions(const QList<QEasingCurve::EasingFunction>& functions){
    int i=0;
    for(QEasingCurve::EasingFunction fun : functions){
        fun(i++);
    }
}
QList<Tulip::TestStdFunction> Tulip::createListOfStdFunctions() {return {[](int,bool,double){}};}
void Tulip::testStdFunctions(const QList<Tulip::TestStdFunction>& functions){
    int i=0;
    for(Tulip::TestStdFunction fun : functions){
        fun(i, i%2==1, i);
        ++i;
    }
}

void Tulip::consumeIntList(const QList<int>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}
void Tulip::consumeStringList(const QList<QString>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}
void Tulip::consumeColorList(const QList<QColor>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}
void Tulip::consumeQObjectList(const QList<QObject*>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}

QSet<QRunnable*> Tulip::createSetOfRunnables() {return {};}
QHash<QString,QRunnable*> Tulip::createStringHashOfRunnables() {return {};}
QMultiHash<QString,QRunnable*> Tulip::createStringMultiHashOfRunnables() {return {};}
QMap<QString,QRunnable*> Tulip::createStringMapOfRunnables() {return {};}
QMultiMap<QString,QRunnable*> Tulip::createStringMultiMapOfRunnables() {return {};}
QList<QObject*> Tulip::createListOfObjects() {return {};}
QSet<QObject*> Tulip::createSetOfObjects() {return {};}
QHash<QString,QObject*> Tulip::createStringHashOfObjects() {return {};}
QMultiHash<QString,QObject*> Tulip::createStringMultiHashOfObjects() {return {};}
QMap<QString,QObject*> Tulip::createStringMapOfObjects() {return {};}
QMultiMap<QString,QObject*> Tulip::createStringMultiMapOfObjects() {return {};}
QList<QList<QRunnable*>> Tulip::createListOfListOfRunnables() {return {};}
QList<QList<QObject*>> Tulip::createListOfListOfObjects() {return {};}

qint64 Tulip::fillIntList(qint32 capacity){
    QList<int> list;
    QTime t1 = QTime::currentTime();
    for (int i = 0; i < capacity; i++) {
        list.append(i);
    }
    QTime t2 = QTime::currentTime();
    return t1.msecsTo(t2);
}
