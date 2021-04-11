#ifndef QTJAMBI_QUICK_HASHES_H
#define QTJAMBI_QUICK_HASHES_H

#include <QtCore/qglobal.h>
#include <qtjambi/qtjambi_core.h>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtQuick/QQuickRenderTarget>

#ifndef QT_JAMBI_RUN
#include <QtQuick/private/qquickrendertarget_p.h>
#endif

inline hash_type qHash(const QQuickRenderTarget &value)
{
    const QQuickRenderTargetPrivate* p = QQuickRenderTargetPrivate::get(&value);
    if(!p){
        return 0;
    }
    hash_type hashCode = qHash(int(p->type));
    hashCode = hashCode * 31 + qHash(p->pixelSize);
    hashCode = hashCode * 31 + qHash(p->sampleCount);
    switch(p->type){
    case QQuickRenderTargetPrivate::Type::NativeTexture:
        hashCode = hashCode * 31 + qHash(p->u.nativeTexture.layout);
        hashCode = hashCode * 31 + qHash(p->u.nativeTexture.object);
        break;
    case QQuickRenderTargetPrivate::Type::RhiRenderTarget:
        hashCode = hashCode * 31 + qHash(p->u.rhiRt);
        break;
    default:
        break;
    }
    return hashCode;
}
#endif

#endif // QTJAMBI_QUICK_HASHES_H
