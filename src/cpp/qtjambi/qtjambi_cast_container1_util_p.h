#ifndef QTJAMBI_CAST_CONTAINER1_UTIL_P_H
#define QTJAMBI_CAST_CONTAINER1_UTIL_P_H

#include "qtjambi_cast_util_p.h"

namespace QtJambiPrivate {

template<template<typename T> class Container, typename T>
struct IntermediateContainer : Container<T>{
    IntermediateContainer(JNIEnv *env, jobject object, QtJambiScope& scope) : Container<T>(), m_scope(scope), m_object(env->NewWeakGlobalRef(object)){}
    ~IntermediateContainer(){
        try{
            if(JNIEnv *env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                try{
                    jobject object = env->NewLocalRef(m_object);
                    if(!env->IsSameObject(object, nullptr)){
                        qtjambi_collection_clear(env, object);
                        for(typename Container<T>::const_iterator i = Container<T>::constBegin(); i!=Container<T>::constEnd(); ++i){
                            qtjambi_collection_add(env, object, qtjambi_scoped_cast<true,jobject,decltype(*i)>::cast(env, *i, nullptr, &m_scope));
                        }
                    }
                }catch(const JavaException& exn){
                    exn.raiseInJava(env);
                }
            }
        }catch(const std::exception& exn){
            printf("An exception occurred: %s\n", exn.what());
        }catch(...){
            printf("An unknown exception occurred.\n");
        }
    }
    QtJambiScope& m_scope;
    jobject m_object;
};

}

#endif // QTJAMBI_CAST_CONTAINER1_UTIL_P_H
