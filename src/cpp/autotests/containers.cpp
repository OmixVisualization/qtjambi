#include <QtCore/QPoint>
#include "containers.h"
#include <QtJambi/QtJambiAPI>
#include <QtJambi/Cast>

#include <QtCore/QSequentialIterable>
#include <QtCore/QAssociativeIterable>

namespace ContainerTest{

QSet<QString> valueAt(QList<QSet<QString>>& container, int position){
    return container[position];
}

void copyQListFDToJavaList(const QList<QPair<float,double>>& container, jobject results){
    if(JniEnvironment env{200}){
        for(QPair<float,double> f : container){
            QtJambiAPI::addToJavaCollection(env, results, QtJambiAPI::newQPair(env, QtJambiAPI::toJavaFloatObject(env, f.first), QtJambiAPI::toJavaDoubleObject(env, f.second)));
        }
    }
}

QHASHOUT getQListFDHash(const QList<QPair<float,double>>& container){
    return qHash(container);
}

void writeQListFD(QDataStream &s, const QList<QPair<float,double>>& container){
    s << container;
}

void readQListFD(QDataStream &s, QList<QPair<float,double>>& container){
    s >> container;
}

void copyQHashShortDoubleToJavaList(const QHash<short,double>& container, jobject results){
    if(JniEnvironment env{200}){
        for(short s : container){
            QtJambiAPI::putJavaMap(env, results, QtJambiAPI::toJavaShortObject(env, s), QtJambiAPI::toJavaDoubleObject(env, container[s]));
        }
    }
}

QHASHOUT getQHashShortDoubleHash(const QHash<short,double>& container){
    return qHash(container);
}

void writeQHashShortDouble(QDataStream &s, const QHash<short,double>& container){
    s << container;
}

void readQHashShortDouble(QDataStream &s, QHash<short,double>& container){
    s >> container;
}

void copyQSetQPointToJavaList(const QSet<QPoint>& container, jobject results){
    if(JniEnvironment env{200}){
        for(QPoint s : container){
            QtJambiAPI::addToJavaCollection(env, results, qtjambi_cast<jobject>(env, s));
        }
    }
}

QHASHOUT getQSetQPointHash(const QSet<QPoint>& container){
    return qHash(container);
}

void writeQSetQPoint(QDataStream &s, const QSet<QPoint>& container){
    s << container;
}

void readQSetQPoint(QDataStream &s, QSet<QPoint>& container){
    s >> container;
}

void copyQMapShortDoubleToJavaList(const QMap<short,double>& container, jobject results){
    if(JniEnvironment env{200}){
        for(short s : container){
            QtJambiAPI::putJavaMap(env, results, QtJambiAPI::toJavaShortObject(env, s), QtJambiAPI::toJavaDoubleObject(env, container[s]));
        }
    }
}

//size_t getQMapShortDoubleHash(const QMap<short,double>& container){
//    return qHash(container);
//}

void writeQMapShortDouble(QDataStream &s, const QMap<short,double>& container){
    s << container;
}

void readQMapShortDouble(QDataStream &s, QMap<short,double>& container){
    s >> container;
}

void copyQMultiHashShortDoubleToJavaList(const QMultiHash<short,double>& container, jobject results){
    if(JniEnvironment env{200}){
        for(short s : container){
            for(double d : container.values(s)){
                QtJambiAPI::putJavaMap(env, results, QtJambiAPI::toJavaShortObject(env, s), QtJambiAPI::toJavaDoubleObject(env, d));
            }
        }
    }
}

QHASHOUT getQMultiHashShortDoubleHash(const QMultiHash<short,double>& container){
    return qHash(container);
}

void writeQMultiHashShortDouble(QDataStream &s, const QMultiHash<short,double>& container){
    s << container;
}

void readQMultiHashShortDouble(QDataStream &s, QMultiHash<short,double>& container){
    s >> container;
}

void copyQMultiMapShortDoubleToJavaList(const QMultiMap<short,double>& container, jobject results){
    if(JniEnvironment env{200}){
        for(short s : container){
            for(double d : container.values(s)){
                QtJambiAPI::putJavaMap(env, results, QtJambiAPI::toJavaShortObject(env, s), QtJambiAPI::toJavaDoubleObject(env, d));
            }
        }
    }
}

//size_t getQMultiMapShortDoubleHash(const QMultiMap<short,double>& container){
//    return qHash(container);
//}

void writeQMultiMapShortDouble(QDataStream &s, const QMultiMap<short,double>& container){
    s << container;
}

void readQMultiMapShortDouble(QDataStream &s, QMultiMap<short,double>& container){
    s >> container;
}

void copyFromIterable(const QVariant& variant, jobject results){
    if(JniEnvironment env{200}){
        if (variant.canConvert<QVariantList>()) {
            QSequentialIterable iterable = variant.value<QSequentialIterable>();
            for(QVariant v : iterable){
                QtJambiAPI::addToJavaCollection(env, results, QtJambiAPI::convertQVariantToJavaObject(env, v));
            }
        }
        else if (variant.canConvert<QVariantHash>()) {
            QAssociativeIterable iterable = variant.value<QAssociativeIterable>();
            QAssociativeIterable::const_iterator begin = iterable.begin();
            QAssociativeIterable::const_iterator end = iterable.end();
            while(begin!=end){
                QVariant key = begin.key();
                QVariant value = begin.value();
                QtJambiAPI::putJavaMap(env, results, QtJambiAPI::convertQVariantToJavaObject(env, key), QtJambiAPI::convertQVariantToJavaObject(env, value));
                ++begin;
            }
        }
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QVariant sequentialAt(const QVariant& variant, int index){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.value<QSequentialIterable>();
        return iterable.at(index);
    }else{
        return QVariant(QString("cannot convert %1 to QVariantList").arg(variant.metaType().name()));
    }
}

QVariant sequentialRemoveFirst(QVariant variant){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.view<QSequentialIterable>();
        iterable.removeValue(QSequentialIterable::AtBegin);
    }else{
        return QVariant(QString("cannot convert %1 to QVariantList").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant sequentialRemoveLast(QVariant variant){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.view<QSequentialIterable>();
        iterable.removeValue(QSequentialIterable::AtEnd);
    }else{
        return QVariant(QString("cannot convert %1 to QVariantList").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant sequentialSetAt(QVariant variant, int index, const QVariant& value){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.view<QSequentialIterable>();
        iterable.set(index, value);
    }else{
        return QVariant(QString("cannot convert %1 to QVariantList").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant sequentialAppend(QVariant variant, const QVariant& value){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.view<QSequentialIterable>();
        iterable.addValue(value, QSequentialIterable::AtEnd);
    }else{
        return QVariant(QString("cannot convert %1 to QVariantList").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant sequentialPrepend(QVariant variant, const QVariant& value){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.view<QSequentialIterable>();
        iterable.addValue(value, QSequentialIterable::AtBegin);
    }else{
        return QVariant(QString("cannot convert %1 to QVariantList").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant associativeSetValue(QVariant variant, const QVariant& key, const QVariant& value){
    if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.view<QAssociativeIterable>();
        iterable.setValue(key, value);
    }else{
        return QVariant(QString("cannot convert %1 to QVariantHash").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant associativeInsertKey(QVariant variant, const QVariant& key){
    if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.view<QAssociativeIterable>();
        iterable.insertKey(key);
    }else{
        return QVariant(QString("cannot convert %1 to QVariantHash").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant associativeRemoveKey(QVariant variant, const QVariant& key){
    if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.view<QAssociativeIterable>();
        iterable.removeKey(key);
    }else{
        return QVariant(QString("cannot convert %1 to QVariantHash").arg(variant.metaType().name()));
    }
    return variant;
}

QPair<QVariant,QVariant> associativeFindAndReplace(QVariant variant, const QVariant& key, const QVariant& value){
    if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.view<QAssociativeIterable>();
        QAssociativeIterable::iterator it = iterable.mutableFind(key);
        if(it!=iterable.mutableEnd()){
            QVariant r = it.value();
            it.value() = value;
            return {variant,r};
        }else{
            if(!key.canConvert(iterable.metaContainer().keyMetaType())){
                return {variant,QVariant(QString("cannot convert %1 to %2").arg(key.metaType().name(), iterable.metaContainer().keyMetaType().name()))};
            }
            if(!iterable.metaContainer().canCreateIteratorAtKey()){
                return {variant,QVariant(QString("QMetaContainer of %1 cannot create iterator at key").arg(variant.metaType().name()))};
            }
            return {variant,QVariant(QString("found iterator == end"))};
        }
    }else{
        return {variant,QVariant(QString("cannot convert %1 to QVariantHash").arg(variant.metaType().name()))};
    }
    return {variant,QVariant()};
}

#endif

bool associativeFind(const QVariant& variant, const QVariant& key){
    if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.value<QAssociativeIterable>();
        return iterable.find(key) != iterable.end();
    }else{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        qWarning() << "cannot convert" << variant.metaType().name() << " to QVariantHash";
#else
        qWarning() << "cannot convert" << QMetaType::typeName(variant.userType()) << " to QVariantHash";
#endif
    }
    return false;
}

QVariant associativeValue(const QVariant& variant, const QVariant& key){
    if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.value<QAssociativeIterable>();
        return iterable.value(key);
    }else{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        qWarning() << "cannot convert" << variant.metaType().name() << " to QVariantHash";
#else
        qWarning() << "cannot convert" << QMetaType::typeName(variant.userType()) << " to QVariantHash";
#endif
    }
    return QVariant();
}

qsizetype containerSize(const QVariant& variant){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.value<QSequentialIterable>();
        return iterable.size();
    }else if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.value<QAssociativeIterable>();
        return iterable.size();
    }else{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        qWarning() << "cannot convert" << variant.metaType().name() << " to QVariantHash or QVariantList";
#else
        qWarning() << "cannot convert" << QMetaType::typeName(variant.userType()) << " to QVariantHash or QVariantList";
#endif
        return 0;
    }
}
QList<QVariant> toQVariantList(const QVariant& variant){
    return variant.value<QVariantList>();
}

QHash<QString,QVariant> toQVariantHash(const QVariant& variant){
    return variant.value<QVariantHash>();
}

QMap<QString,QVariant> toQVariantMap(const QVariant& variant){
    return variant.value<QVariantMap>();
}

} // namespace ContainerTest
