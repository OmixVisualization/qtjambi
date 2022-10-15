#include <QtQuick/qsgtexture_platform.h>
#include <qtjambi/qtjambi_global.h>

void* qtjambi_QSGMetalTexture_nativeTexture(JNIEnv *, const void* ptr){
    const QNativeInterface::QSGMetalTexture* texture = reinterpret_cast<const QNativeInterface::QSGMetalTexture*>(ptr);
    return texture->nativeTexture();
}
QSGTexture * qtjambi_QSGMetalTexture_fromNative(JNIEnv *, void* texture, QQuickWindow* window, const QSize& size, QQuickWindow::CreateTextureOptions options){
    id<MTLTexture> ml = reinterpret_cast<id<MTLTexture>>(texture);
    return QNativeInterface::QSGMetalTexture::fromNative(ml, window, size, options);
}
