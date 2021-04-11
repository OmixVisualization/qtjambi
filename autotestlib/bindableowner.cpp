#include "bindableowner.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

BindableOwner::BindableOwner(QObject *parent) : QObject(parent)
{

}

bool BindableOwner::bind(QObject* targetObject, QStringView targetPropertyName, QObject* sourceObject, QStringView sourcePropertyName){
    QUntypedBindable target(targetObject->metaObject()->property(targetObject->metaObject()->indexOfProperty(targetPropertyName.toLatin1())).bindable(targetObject));
    QUntypedBindable source(sourceObject->metaObject()->property(sourceObject->metaObject()->indexOfProperty(sourcePropertyName.toLatin1())).bindable(sourceObject));

    if(target.isValid() && source.isValid()){
        return target.setBinding(source.makeBinding());
    }
    return false;
}

#endif //QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
