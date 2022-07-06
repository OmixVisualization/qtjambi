#include <QtCore/QPoint>
#include "containers.h"
#include <qtjambi/qtjambi_core.h>

#include <QtCore/QSequentialIterable>
#include <QtCore/QAssociativeIterable>

namespace ContainerTest{

QSet<QString> valueAt(QList<QSet<QString>>& container, int position){
    return container[position];
}

void copyQListFDToJavaList(const QList<QPair<float,double>>& container, jobject results){
    JNIEnv* env = qtjambi_current_environment();
    for(QPair<float,double> f : container){
        qtjambi_collection_add(env, results, qtjambi_pair_new(env, qtjambi_from_float(env, f.first), qtjambi_from_double(env, f.second)));
    }
}

size_t getQListFDHash(const QList<QPair<float,double>>& container){
    return qHash(container);
}

void writeQListFD(QDataStream &s, const QList<QPair<float,double>>& container){
    s << container;
}

void readQListFD(QDataStream &s, QList<QPair<float,double>>& container){
    s >> container;
}

void copyQHashShortDoubleToJavaList(const QHash<short,double>& container, jobject results){
    JNIEnv* env = qtjambi_current_environment();
    for(short s : container){
        qtjambi_map_put(env, results, qtjambi_from_short(env, s), qtjambi_from_double(env, container[s]));
    }
}

size_t getQHashShortDoubleHash(const QHash<short,double>& container){
    return qHash(container);
}

void writeQHashShortDouble(QDataStream &s, const QHash<short,double>& container){
    s << container;
}

void readQHashShortDouble(QDataStream &s, QHash<short,double>& container){
    s >> container;
}

void copyQSetQPointToJavaList(const QSet<QPoint>& container, jobject results){
    JNIEnv* env = qtjambi_current_environment();
    for(QPoint s : container){
        qtjambi_collection_add(env, results, qtjambi_from_object(env, &s, true));
    }
}

size_t getQSetQPointHash(const QSet<QPoint>& container){
    return qHash(container);
}

void writeQSetQPoint(QDataStream &s, const QSet<QPoint>& container){
    s << container;
}

void readQSetQPoint(QDataStream &s, QSet<QPoint>& container){
    s >> container;
}

void copyQMapShortDoubleToJavaList(const QMap<short,double>& container, jobject results){
    JNIEnv* env = qtjambi_current_environment();
    for(short s : container){
        qtjambi_map_put(env, results, qtjambi_from_short(env, s), qtjambi_from_double(env, container[s]));
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
    JNIEnv* env = qtjambi_current_environment();
    for(short s : container){
        for(double d : container.values(s)){
            qtjambi_map_put(env, results, qtjambi_from_short(env, s), qtjambi_from_double(env, d));
        }
    }
}

size_t getQMultiHashShortDoubleHash(const QMultiHash<short,double>& container){
    return qHash(container);
}

void writeQMultiHashShortDouble(QDataStream &s, const QMultiHash<short,double>& container){
    s << container;
}

void readQMultiHashShortDouble(QDataStream &s, QMultiHash<short,double>& container){
    s >> container;
}

void copyQMultiMapShortDoubleToJavaList(const QMultiMap<short,double>& container, jobject results){
    JNIEnv* env = qtjambi_current_environment();
    for(short s : container){
        for(double d : container.values(s)){
            qtjambi_map_put(env, results, qtjambi_from_short(env, s), qtjambi_from_double(env, d));
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
    JNIEnv* env = qtjambi_current_environment();
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.value<QSequentialIterable>();
        for(QVariant v : iterable){
            qtjambi_collection_add(env, results, qtjambi_from_qvariant(env, v));
        }
    }
    else if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.value<QAssociativeIterable>();
        QAssociativeIterable::const_iterator begin = iterable.begin();
        QAssociativeIterable::const_iterator end = iterable.end();
        while(begin!=end){
            QVariant key = begin.key();
            QVariant value = begin.value();
            qtjambi_map_put(env, results, qtjambi_from_qvariant(env, key), qtjambi_from_qvariant(env, value));
            ++begin;
        }
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QVariant sequentialAt(const QVariant& variant, int index){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.value<QSequentialIterable>();
        return iterable.at(index);
    }
    return QVariant();
}

QVariant sequentialRemoveFirst(QVariant variant){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.view<QSequentialIterable>();
        iterable.removeValue(QSequentialIterable::AtBegin);
    }
    return variant;
}

QVariant sequentialRemoveLast(QVariant variant){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.view<QSequentialIterable>();
        iterable.removeValue(QSequentialIterable::AtEnd);
    }
    return variant;
}

QVariant sequentialSetAt(QVariant variant, int index, const QVariant& value){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.view<QSequentialIterable>();
        iterable.set(index, value);
    }
    return variant;
}

QVariant sequentialAppend(QVariant variant, const QVariant& value){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.view<QSequentialIterable>();
        iterable.addValue(value, QSequentialIterable::AtEnd);
    }
    return variant;
}

QVariant sequentialPrepend(QVariant variant, const QVariant& value){
    if (variant.canConvert<QVariantList>()) {
        QSequentialIterable iterable = variant.view<QSequentialIterable>();
        iterable.addValue(value, QSequentialIterable::AtBegin);
    }
    return variant;
}

QVariant associativeSetValue(QVariant variant, const QVariant& key, const QVariant& value){
    if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.view<QAssociativeIterable>();
        iterable.setValue(key, value);
    }
    return variant;
}

QVariant associativeInsertKey(QVariant variant, const QVariant& key){
    if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.view<QAssociativeIterable>();
        iterable.insertKey(key);
    }
    return variant;
}

QVariant associativeRemoveKey(QVariant variant, const QVariant& key){
    if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.view<QAssociativeIterable>();
        iterable.removeKey(key);
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
        }
    }
    return {variant,QVariant()};
}

#endif

bool associativeFind(const QVariant& variant, const QVariant& key){
    if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.value<QAssociativeIterable>();
        return iterable.find(key) != iterable.end();
    }
    return false;
}

QVariant associativeValue(const QVariant& variant, const QVariant& key){
    if (variant.canConvert<QVariantHash>()) {
        QAssociativeIterable iterable = variant.value<QAssociativeIterable>();
        return iterable.value(key);
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
