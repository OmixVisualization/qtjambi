#ifndef CONTAINERS_QT5_H
#define CONTAINERS_QT5_H

#include <QtCore/QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

#include <QtCore/QLinkedList>
#include <QtCore/QVector>
#include <QtCore/QString>

#include <QtJambi/typetests.h>

class QLinkedList_int : public QLinkedList<int> { };
class QVector_int : public QVector<int> { };
class QVector_String : public QVector<QString> { };
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

#endif // CONTAINERS_QT5_H
