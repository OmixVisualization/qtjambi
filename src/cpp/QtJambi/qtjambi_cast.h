/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBI_CONVERT_H
#define QTJAMBI_CONVERT_H
#define QTJAMBI_CONTAINERACCESS_H

#include "qtjambi_cast_object.h"

#ifdef QNAMESPACE_H
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,Qt::InputMethodQuery&>(JNIEnv *, Qt::InputMethodQuery&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,Qt::ItemSelectionMode&>(JNIEnv *, Qt::ItemSelectionMode&);
#endif

#ifdef QVARIANT_H
extern template QTJAMBI_EXPORT QVariant qtjambi_cast<QVariant,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QVariant>(JNIEnv *, QVariant&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QVariant&>(JNIEnv *, QVariant&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QVariant&>(JNIEnv *, const QVariant&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QVariant*&>(JNIEnv *, const QVariant*&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jcoreobject,QVariant>(JNIEnv *, QVariant&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jcoreobject,QVariant&>(JNIEnv *, QVariant&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jcoreobject,QVariant*&>(JNIEnv *, QVariant*&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jcoreobject,const QVariant&>(JNIEnv *, const QVariant&);

extern template QTJAMBI_EXPORT QList<QVariant> qtjambi_cast<QList<QVariant>,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT QList<QVariant> qtjambi_cast<QList<QVariant>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT const QList<QVariant>& qtjambi_cast<const QList<QVariant>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QList<QVariant>>(JNIEnv *, QList<QVariant>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QList<QVariant>&>(JNIEnv *, const QList<QVariant>&);
#endif

#ifdef QSTRING_H
extern template QTJAMBI_EXPORT QString qtjambi_cast<QString,jstring&>(JNIEnv *, jstring&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QString>(JNIEnv *, QString&&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QString&>(JNIEnv *, QString&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,const QString&>(JNIEnv *, const QString&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jcoreobject,QString>(JNIEnv *, QString&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jcoreobject,QString&>(JNIEnv *, QString&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jcoreobject,QString*&>(JNIEnv *, QString*&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jcoreobject,const QString&>(JNIEnv *, const QString&);

extern template QTJAMBI_EXPORT QStringList qtjambi_cast<QStringList,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT QStringList qtjambi_cast<QStringList,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT const QStringList& qtjambi_cast<const QStringList&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QStringList>(JNIEnv *, QStringList&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QStringList&>(JNIEnv *, const QStringList&);
#endif

#ifdef QBYTEARRAY_H
extern template QTJAMBI_EXPORT QByteArray qtjambi_cast<QByteArray,jstring&>(JNIEnv *, jstring&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QByteArray>(JNIEnv *, QByteArray&&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QByteArray&>(JNIEnv *, QByteArray&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,const QByteArray&>(JNIEnv *, const QByteArray&);

extern template QTJAMBI_EXPORT QByteArray qtjambi_cast<QByteArray,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QByteArray>(JNIEnv *, QByteArray&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QByteArray&>(JNIEnv *, QByteArray&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QByteArray&>(JNIEnv *, const QByteArray&);
#endif

#ifdef QBYTEARRAYVIEW_H
extern template QTJAMBI_EXPORT QByteArrayView qtjambi_cast<QByteArrayView,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QByteArrayView>(JNIEnv *, QByteArrayView&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QByteArrayView&>(JNIEnv *, QByteArrayView&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QByteArrayView&>(JNIEnv *, const QByteArrayView&);

extern template QTJAMBI_EXPORT QByteArrayView qtjambi_cast<QByteArrayView,jstring&>(JNIEnv *, QtJambiScope&, jstring&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QByteArrayView>(JNIEnv *, QByteArrayView&&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QByteArrayView&>(JNIEnv *, QByteArrayView&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,const QByteArrayView&>(JNIEnv *, const QByteArrayView&);
#endif

#ifdef QSTRINGVIEW_H
extern template QTJAMBI_EXPORT QStringView qtjambi_cast<QStringView,jstring&>(JNIEnv *, QtJambiScope&, jstring&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QStringView>(JNIEnv *, QStringView&&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QStringView&>(JNIEnv *, QStringView&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,const QStringView&>(JNIEnv *, const QStringView&);
#endif

#ifdef QLATIN1STRINGVIEW_H
extern template QTJAMBI_EXPORT QLatin1StringView qtjambi_cast<QLatin1StringView,jstring&>(JNIEnv *, QtJambiScope&, jstring&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QLatin1StringView>(JNIEnv *, QLatin1StringView&&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QLatin1StringView&>(JNIEnv *, QLatin1StringView&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,const QLatin1StringView&>(JNIEnv *, const QLatin1StringView&);
#endif

#ifdef QANYSTRINGVIEW_H
extern template QTJAMBI_EXPORT QAnyStringView qtjambi_cast<QAnyStringView,jstring&>(JNIEnv *, QtJambiScope&, jstring&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QAnyStringView>(JNIEnv *, QAnyStringView&&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,QAnyStringView&>(JNIEnv *, QAnyStringView&);
extern template QTJAMBI_EXPORT jstring qtjambi_cast<jstring,const QAnyStringView&>(JNIEnv *, const QAnyStringView&);
#endif

#ifdef QOBJECT_H
extern template QTJAMBI_EXPORT QObject* qtjambi_cast<QObject*,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT QList<QObject*> qtjambi_cast<QList<QObject*>,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT QList<QObject*> qtjambi_cast<QList<QObject*>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT const QList<QObject*>& qtjambi_cast<const QList<QObject*>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QList<QObject*>>(JNIEnv *, QList<QObject*>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QList<QObject*>&>(JNIEnv *, const QList<QObject*>&);
#endif

#ifdef QCOREEVENT_H
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QEvent*&>(JNIEnv *, QEvent*&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QTimerEvent*&>(JNIEnv *, QTimerEvent*&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QChildEvent*&>(JNIEnv *, QChildEvent*&);
#endif

#ifdef QMETAOBJECT_H
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QMetaMethod&>(JNIEnv *, const QMetaMethod&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QMetaProperty&>(JNIEnv *, const QMetaProperty&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QMetaObject*&>(JNIEnv *, const QMetaObject*&);
#endif

#ifdef QRECT_H
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QRect>(JNIEnv *, QRect&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QRect&>(JNIEnv *, const QRect&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QRectF>(JNIEnv *, QRectF&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QRectF&>(JNIEnv *, const QRectF&);
#endif

#ifdef QSIZE_H
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QSize>(JNIEnv *, QSize&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QSize&>(JNIEnv *, const QSize&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QSizeF>(JNIEnv *, QSizeF&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QSizeF&>(JNIEnv *, const QSizeF&);
extern template QTJAMBI_EXPORT QList<QSize> qtjambi_cast<QList<QSize>,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT QList<QSize> qtjambi_cast<QList<QSize>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT const QList<QSize>& qtjambi_cast<const QList<QSize>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QList<QSize>>(JNIEnv *, QList<QSize>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QList<QSize>&>(JNIEnv *, const QList<QSize>&);
#endif

#ifdef QPOINT_H
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QPoint>(JNIEnv *, QPoint&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QPoint&>(JNIEnv *, const QPoint&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QPointF>(JNIEnv *, QPointF&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QPointF&>(JNIEnv *, const QPointF&);
extern template QTJAMBI_EXPORT QList<QPointF> qtjambi_cast<QList<QPointF>,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT QList<QPointF> qtjambi_cast<QList<QPointF>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT const QList<QPointF>& qtjambi_cast<const QList<QPointF>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QList<QPointF>>(JNIEnv *, QList<QPointF>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QList<QPointF>&>(JNIEnv *, const QList<QPointF>&);
extern template QTJAMBI_EXPORT QList<QPoint> qtjambi_cast<QList<QPoint>,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT QList<QPoint> qtjambi_cast<QList<QPoint>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT const QList<QPoint>& qtjambi_cast<const QList<QPoint>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QList<QPoint>>(JNIEnv *, QList<QPoint>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QList<QPoint>&>(JNIEnv *, const QList<QPoint>&);
#endif

#ifdef QURL_H
extern template QTJAMBI_EXPORT QUrl qtjambi_cast<QUrl,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QUrl>(JNIEnv *, QUrl&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QUrl&>(JNIEnv *, QUrl&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QUrl&>(JNIEnv *, const QUrl&);
#endif

#ifdef QABSTRACTITEMMODEL_H
extern template QTJAMBI_EXPORT QModelIndex qtjambi_cast<QModelIndex,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QModelIndex>(JNIEnv *, QModelIndex&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QModelIndex&>(JNIEnv *, QModelIndex&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QModelIndex&>(JNIEnv *, const QModelIndex&);
#endif

#ifdef QMARGINS_H
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QMargins>(JNIEnv *, QMargins&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QMarginsF>(JNIEnv *, QMarginsF&&);
#endif

#ifdef QPROPERTY_H
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QBindable<int>>(JNIEnv *, QBindable<int>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QBindable<bool>>(JNIEnv *, QBindable<bool>&&);
#ifdef QSTRING_H
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QBindable<QString>>(JNIEnv *, QBindable<QString>&&);
#endif

#ifdef QBYTEARRAY_H
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QBindable<QByteArray>>(JNIEnv *, QBindable<QByteArray>&&);
#endif

#ifdef QOBJECT_H
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QBindable<QObject*>>(JNIEnv *, QBindable<QObject*>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QBindable<const QObject*>>(JNIEnv *, QBindable<const QObject*>&&);
#endif

extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QBindable<float>>(JNIEnv *, QBindable<float>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QBindable<double>>(JNIEnv *, QBindable<double>&&);
#endif

#ifdef QTJAMBI_JOBJECTWRAPPER_H
extern template QTJAMBI_EXPORT QList<JObjectWrapper> qtjambi_cast<QList<JObjectWrapper>,jobject&>(JNIEnv *, jobject&);
extern template QTJAMBI_EXPORT QList<JObjectWrapper> qtjambi_cast<QList<JObjectWrapper>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT const QList<JObjectWrapper>& qtjambi_cast<const QList<JObjectWrapper>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,QList<JObjectWrapper>>(JNIEnv *, QList<JObjectWrapper>&&);
extern template QTJAMBI_EXPORT jobject qtjambi_cast<jobject,const QList<JObjectWrapper>&>(JNIEnv *, const QList<JObjectWrapper>&);
#endif

namespace QtJambiPrivate {

template<class O, class T, typename... Args>
struct qtjambi_nojni_plain_cast{
    typedef std::remove_reference_t<O> O_noref;
    typedef std::remove_reference_t<T> T_noref;
    typedef std::remove_cv_t<O_noref> O_noconst;
    typedef std::remove_cv_t<T_noref> T_noconst;
    static constexpr bool is_same = std::is_same_v<O_noref, T_noref>;
    static constexpr bool is_O_arithmetic = std::is_arithmetic_v<O_noconst>;
    static constexpr bool is_O_enum = std::is_enum_v<O_noconst>;

    static O cast(T in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(is_same){
            return in;
        }else if constexpr(is_O_arithmetic || is_O_enum){
            return static_cast<O>(in);
        }else if constexpr(std::is_same_v<T_noconst,QtJambiNativeID>){
            if constexpr(std::is_pointer_v<O>){
                return QtJambiAPI::objectFromNativeId<std::remove_pointer_t<O>>(in);
            }else if constexpr(std::is_reference_v<O>){
                if constexpr(std::is_const_v<O_noref>
                              && is_default_constructible_v<O_noconst>){
                    return QtJambiAPI::valueReferenceFromNativeId<O_noconst>(in);
                }else{
                    auto env = cast_var_args<Args...>::env(args...);
                    return QtJambiAPI::objectReferenceFromNativeId<O_noconst>(env, in);
                }
            }else{
                return QtJambiAPI::valueFromNativeId<O_noconst>(in);
            }
        }else{
            Q_STATIC_ASSERT_X(is_same, "Cannot cast types");
            throw;
        }
    }
};

template<typename O, typename T, typename I, bool fixSize, typename... Args>
struct qtjambi_array_cast;

template<class O, class T, class I, typename... Args>
static constexpr auto qtjambi_cast_array() {
    constexpr bool fixSize = std::is_same_v<I,void> ? true : !(std::is_reference_v<I> || std::is_pointer_v<I>) || std::is_const_v<I>;
    constexpr bool hasCastImpl = is_complete_v<qtjambi_array_cast<O, std::remove_reference_t<T>, I, fixSize, Args...>>;
    Q_STATIC_ASSERT_X(hasCastImpl, "Cannot cast without including <QtJambi/ArrayCast>");
    return qtjambi_array_cast<O, std::remove_reference_t<T>, I, fixSize, Args...>{};
}

template<bool forward,
         typename EnumType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_cast;

template<bool forward,
         typename EnumType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
static constexpr auto qtjambi_cast_enum() {
    constexpr bool hasCastImpl = is_complete_v<qtjambi_enum_cast<forward, EnumType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>>;
    Q_STATIC_ASSERT_X(hasCastImpl, "Cannot cast without including <QtJambi/EnumCast>");
    return qtjambi_enum_cast<forward, EnumType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
}

template<bool forward,
         typename ArithmeticType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_cast;

template<bool forward,
         typename ArithmeticType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
static constexpr auto qtjambi_cast_arithmetic() {
    constexpr bool hasCastImpl = is_complete_v<qtjambi_arithmetic_cast<forward, ArithmeticType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>>;
    Q_STATIC_ASSERT_X(hasCastImpl, "Cannot cast without including <QtJambi/ArithmeticCast>");
    return qtjambi_arithmetic_cast<forward, ArithmeticType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
}

template<class O, class T_in, typename... Args>
static constexpr auto qtjambi_cast_all() {
    constexpr bool is_rvalue = !((std::is_array_v<std::remove_reference_t<T_in>> || std::is_reference_v<T_in> || std::is_pointer_v<std::decay_t<T_in>>) && !std::is_rvalue_reference_v<T_in>);
    typedef std::conditional_t<std::is_array_v<std::remove_reference_t<T_in>>, std::decay_t<T_in>, T_in> T;
    if constexpr(is_jni_object_type_v<O>){
        return qtjambi_jobject_cast<true,
                                    typename qtjambi_cast_types<O>::T_noconst,
                                    std::remove_cv_t<std::remove_pointer_t<typename qtjambi_cast_types<T>::T_noconst>>,
                                    std::is_pointer_v<typename qtjambi_cast_types<T>::T_noref>,
                                    std::is_const_v<std::conditional_t<std::is_pointer_v<typename qtjambi_cast_types<T>::T_noref>, std::remove_pointer_t<typename qtjambi_cast_types<T>::T_noref>, typename qtjambi_cast_types<T>::T_noref>>,
                                    (std::is_reference_v<T> || is_rvalue) && !std::is_pointer_v<typename qtjambi_cast_types<T>::T_noref>, is_rvalue, Args...>{};
    }else if constexpr(is_jni_object_type_v<typename qtjambi_cast_types<T>::T_noconst>){
        return qtjambi_jobject_cast<false,
                                    typename qtjambi_cast_types<T>::T_noconst,
                                    std::remove_cv_t<std::remove_pointer_t<typename qtjambi_cast_types<O>::T_noconst>>,
                                    std::is_pointer_v<typename qtjambi_cast_types<O>::T_noref>,
                                    std::is_const_v<std::conditional_t<std::is_pointer_v<typename qtjambi_cast_types<O>::T_noref>, std::remove_pointer_t<typename qtjambi_cast_types<O>::T_noref>, typename qtjambi_cast_types<O>::T_noref>>,
                                    std::is_reference_v<O> && !std::is_pointer_v<typename qtjambi_cast_types<O>::T_noref>, false, Args...>{};

    }else if constexpr(std::is_arithmetic_v<O>
                         || std::is_same_v<O,QChar>
                         || std::is_same_v<O,QLatin1Char>){
        return qtjambi_cast_arithmetic<true,
                                       O,
                                       std::remove_cv_t<std::remove_pointer_t<typename qtjambi_cast_types<T>::T_noconst>>,
                                       std::is_pointer_v<typename qtjambi_cast_types<T>::T_noref>,
                                       std::is_const_v<std::conditional_t<std::is_pointer_v<typename qtjambi_cast_types<T>::T_noref>, std::remove_pointer_t<typename qtjambi_cast_types<T>::T_noref>, typename qtjambi_cast_types<T>::T_noref>>,
                                       (std::is_reference_v<T> || is_rvalue) && !std::is_pointer_v<typename qtjambi_cast_types<T>::T_noref>, is_rvalue, Args...>();

    }else if constexpr(std::is_arithmetic_v<typename qtjambi_cast_types<T>::T_noconst>
                         || std::is_same_v<typename qtjambi_cast_types<T>::T_noconst,QChar>
                         || std::is_same_v<typename qtjambi_cast_types<T>::T_noconst,QLatin1Char>){
        return qtjambi_cast_arithmetic<false,
                                       typename qtjambi_cast_types<T>::T_noconst,
                                       std::remove_cv_t<std::remove_pointer_t<typename qtjambi_cast_types<O>::T_noconst>>,
                                       std::is_pointer_v<typename qtjambi_cast_types<O>::T_noref>,
                                       std::is_const_v<std::conditional_t<std::is_pointer_v<typename qtjambi_cast_types<O>::T_noref>, std::remove_pointer_t<typename qtjambi_cast_types<O>::T_noref>, typename qtjambi_cast_types<O>::T_noref>>,
                                       std::is_reference_v<O> && !std::is_pointer_v<typename qtjambi_cast_types<O>::T_noref>, false, Args...>();

    }else if constexpr(std::is_enum_v<O>){
        return qtjambi_cast_enum<true,
                                 O,
                                 std::remove_cv_t<std::remove_pointer_t<typename qtjambi_cast_types<T>::T_noconst>>,
                                 std::is_pointer_v<typename qtjambi_cast_types<T>::T_noref>,
                                 std::is_const_v<std::conditional_t<std::is_pointer_v<typename qtjambi_cast_types<T>::T_noref>, std::remove_pointer_t<typename qtjambi_cast_types<T>::T_noref>, typename qtjambi_cast_types<T>::T_noref>>,
                                 (std::is_reference_v<T> || is_rvalue) && !std::is_pointer_v<typename qtjambi_cast_types<T>::T_noref>, is_rvalue, Args...>();

    }else if constexpr(std::is_enum_v<typename qtjambi_cast_types<T>::T_plain> && !std::is_same_v<typename qtjambi_cast_types<T>::T_plain,QtJambiNativeID>){
        return qtjambi_cast_enum<false,
                                 typename qtjambi_cast_types<T>::T_noconst,
                                 std::remove_cv_t<std::remove_pointer_t<typename qtjambi_cast_types<O>::T_noconst>>,
                                 std::is_pointer_v<typename qtjambi_cast_types<O>::T_noref>,
                                 std::is_const_v<std::conditional_t<std::is_pointer_v<typename qtjambi_cast_types<O>::T_noref>, std::remove_pointer_t<typename qtjambi_cast_types<O>::T_noref>, typename qtjambi_cast_types<O>::T_noref>>,
                                 std::is_reference_v<O> && !std::is_pointer_v<typename qtjambi_cast_types<O>::T_noref>, false, Args...>();

    }else{
        return qtjambi_nojni_plain_cast<O,T, Args...>{};
    }
}

template<typename Iterator, typename... Args>
struct qtjambi_associative_iterator_cast;

template<typename Iterator, typename... Args>
struct qtjambi_sequential_iterator_cast;

template<class T, typename... Args>
static constexpr auto qtjambi_cast_iterator() {
    if constexpr(supports_key_v<T>){
        constexpr bool hasCastImpl = is_complete_v<qtjambi_associative_iterator_cast<std::remove_reference_t<T>, Args...>>;
        Q_STATIC_ASSERT_X(hasCastImpl, "Cannot cast without including <QtJambi/IteratorCast>");
        return qtjambi_associative_iterator_cast<std::remove_reference_t<T>, Args...>{};
    }else{
        constexpr bool hasCastImpl = is_complete_v<qtjambi_sequential_iterator_cast<std::remove_reference_t<T>, Args...>>;
        Q_STATIC_ASSERT_X(hasCastImpl, "Cannot cast without including <QtJambi/IteratorCast>");
        return qtjambi_sequential_iterator_cast<std::remove_reference_t<T>, Args...>{};
    }
}

template<typename...Args>
struct iterator_arg_test : std::false_type{
};

template<class ID, class Iter, typename...Args>
struct iterator_arg_test<ID,Iter,Args...> : std::is_same<QtJambiNativeID,std::remove_cv_t<std::remove_reference_t<ID>>>{
};

template<typename...Args>
struct int_arg_test : std::false_type{
};

template<class I, typename...Args>
struct int_arg_test<I,Args...> : std::is_integral<std::remove_reference_t<I>>{
};

template<class O, class T, typename... Args>
static constexpr auto qtjambi_cast_args() {
    using O_noref = std::remove_reference_t<O>;
    using T_noref = std::remove_reference_t<T>;
    if constexpr(int_arg_test<Args...>::value){
        return qtjambi_cast_array<O, T_noref, Args...>();
    }else if constexpr(std::is_pointer_v<O> && is_jni_array_type_v<T_noref>){
        return qtjambi_cast_array<O,T_noref,void,Args...>();
    }else if constexpr(std::is_array_v<T_noref> && (is_jni_array_type_v<O> || std::is_same_v<O, jobject> || std::is_same_v<O, jcoreobject>)){
        return qtjambi_cast_array<O,T,void,Args...>();
    }else if constexpr(std::is_array_v<O_noref> && std::extent_v<O_noref> > 0 && (is_jni_array_type_v<T_noref>
                                                                                    || std::is_same_v<T_noref, jstring>
                                                                                    || std::is_same_v<T_noref, jobject>)){
        return qtjambi_cast_array<O,T,void,Args...>();
    }else if constexpr(std::is_same_v<O, jobject> && iterator_arg_test<T,Args...>::value){
        return qtjambi_cast_iterator<Args...>();
    }else{
        return qtjambi_cast_all<O, T, Args...>();
    }
}

template<class O, typename... Args>
struct qtjambi_cast_impl : decltype(qtjambi_cast_args<O, Args...>()){};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CONVERT_H
