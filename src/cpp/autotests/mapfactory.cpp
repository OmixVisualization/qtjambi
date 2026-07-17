#include "mapfactory.h"

MapFactory::MapFactory() {}

QSet<QRunnable*> MapFactory::createSetOfRunnables() {return {};}
QHash<QString,QRunnable*> MapFactory::createStringHashOfRunnables() {return {};}
QMultiHash<QString,QRunnable*> MapFactory::createStringMultiHashOfRunnables() {return {};}
QMap<QString,QRunnable*> MapFactory::createStringMapOfRunnables() {return {};}
QMultiMap<QString,QRunnable*> MapFactory::createStringMultiMapOfRunnables() {return {};}
QSet<QObject*> MapFactory::createSetOfObjects() {return {};}
QHash<QString,QObject*> MapFactory::createStringHashOfObjects() {return {};}
QMultiHash<QString,QObject*> MapFactory::createStringMultiHashOfObjects() {return {};}
QMap<QString,QObject*> MapFactory::createStringMapOfObjects() {return {};}
QMultiMap<QString,QObject*> MapFactory::createStringMultiMapOfObjects() {return {};}
