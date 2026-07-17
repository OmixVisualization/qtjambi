#include "containerconverter.h"

ContainerConverter::ContainerConverter() {}

QList<int> ContainerConverter::do_QList_of_int(const QList<int> &l) { return l; }

QStringList ContainerConverter::do_QStringList(const QStringList &l) { return l; }

QStack<int> ContainerConverter::do_QStack_of_int(const QStack<int> &s) { return s; }

QQueue<int> ContainerConverter::do_QQueue_of_int(const QQueue<int> &q) { return q; }

QSet<int> ContainerConverter::do_QSet_of_int(const QSet<int> &s) { return s; }

QMap<QString, QString> ContainerConverter::do_QMap_of_strings(const QMap<QString, QString> &m) { return m; }

QHash<QString, QString> ContainerConverter::do_QHash_of_strings(const QHash<QString, QString> &h) { return h; }

QPair<int, int> ContainerConverter::do_QPair_of_ints(const QPair<int, int> &p) { return p; }
