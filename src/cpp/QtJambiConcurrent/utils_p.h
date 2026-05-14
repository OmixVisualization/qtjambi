#ifndef UTILS_P_H
#define UTILS_P_H

#include <QtJambi/FutureAPI>
#include <QtConcurrent/QtConcurrent>
#include <QtJambi/CoreAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/FutureCast>

template<typename V>
struct JavaSequence{
    typedef V value_type;

    class const_iterator{
    public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef V value_type;
        typedef const value_type *pointer;
        typedef const value_type &reference;

        const_iterator()
            : m_collection(), m_cursor(-1), m_current(), m_isList(false) {}
        const_iterator(const const_iterator& other)
            : m_collection(other.m_collection), m_size(other.m_size), m_cursor(other.m_cursor), m_current(other.m_current), m_isList(other.m_isList) {}

        const_iterator operator ++(int){
            JavaSequence::const_iterator result(*this);
            ++m_cursor;
            if(m_cursor>=m_size){
                m_cursor = -1;
            }
            return result;
        }
        const_iterator operator --(int){
            JavaSequence::const_iterator result(*this);
            if(m_cursor>0){
                --m_cursor;
                if(m_cursor>=m_size){
                    m_cursor = -1;
                }
            }
            return result;
        }
        const_iterator& operator ++(){
            ++m_cursor;
            if(m_cursor>=m_size){
                m_cursor = -1;
            }
            return *this;
        }
        const_iterator& operator --(){
            if(m_cursor>0){
                --m_cursor;
                if(m_cursor>=m_size){
                    m_cursor = -1;
                }
            }
            return *this;
        }

        reference operator*() const{
            if(m_cursor>=0 && m_cursor<m_size){
                if(JniEnvironment env{200}){
                    jobject _object = m_collection.object(env);
                    if(_object){
                        jobject result = nullptr;
                        if(m_isList){
                            result = QtJambiAPI::getAtJavaList(env, _object, m_cursor);
                        }else{
                            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, _object);
                            for(int i=0; i<m_cursor; ++i){
                                QtJambiAPI::nextOfJavaIterator(env, iterator);
                            }
                            result = QtJambiAPI::nextOfJavaIterator(env, iterator);
                        }
                        QtJambiPrivate::JavaValue<V>::assign(m_current, env, result);
                    }else{
                        QtJambiPrivate::JavaValue<V>::assign(m_current, env, nullptr);
                    }
                }else{
                    m_current = V();
                }
            }else{
                m_current = V();
            }
            return m_current;
        }
        pointer operator->() const{
            return &operator*();
        }
        inline operator pointer() const { return operator->(); }
        bool operator<(const const_iterator& o) const{
            if(JniEnvironment env{300}){
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                return env->IsSameObject(m_collection.object(env), o.m_collection.object(env)) && m_cursor<o.m_cursor;
            }else return false;
        }
        bool operator>(const const_iterator& o) const{
            if(JniEnvironment env{300}){
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                return env->IsSameObject(m_collection.object(env), o.m_collection.object(env)) && m_cursor>o.m_cursor;
            }else return false;
        }
        bool operator<=(const const_iterator& o) const{
            if(JniEnvironment env{300}){
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                return env->IsSameObject(m_collection.object(env), o.m_collection.object(env)) && m_cursor<=o.m_cursor;
            }else return false;
        }
        bool operator>=(const const_iterator& o) const{
            if(JniEnvironment env{300}){
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                return env->IsSameObject(m_collection.object(env), o.m_collection.object(env)) && m_cursor>=o.m_cursor;
            }else return false;
        }
        bool operator==(const const_iterator& o) const{
            if(JniEnvironment env{300}){
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                return env->IsSameObject(m_collection.object(env), o.m_collection.object(env)) && m_cursor==o.m_cursor;
            }else return false;
        }
        inline bool operator!=(const const_iterator& o) const {
            return !(*this == o);
        }
    private:
        const_iterator(JNIEnv* env, const JObjectWrapper& collection, int cursor = 0)
            : m_collection(collection),
            m_size(QtJambiAPI::sizeOfJavaCollection(env, collection.object(env))),
            m_cursor(cursor),
            m_current(), m_isList(Java::Runtime::List::isInstanceOf(env, collection.object(env))) {}

        JObjectWrapper m_collection;
        int m_size;
        int m_cursor;
        mutable value_type m_current;
        bool m_isList;
        friend JavaSequence;
    };

    JavaSequence()
        : m_collection() {}

    JavaSequence(const JavaSequence& other)
        : m_canOverwrite(other.m_canOverwrite), m_collection(other.m_collection){}

    JavaSequence(JavaSequence&& other)
        : m_canOverwrite(other.m_canOverwrite), m_collection(std::move(other.m_collection)) {}

    JavaSequence(JNIEnv* env, jobject collection, bool canOverwrite = false)
        : m_canOverwrite(canOverwrite), m_collection(env, collection) {}

    JavaSequence& operator=(const JavaSequence& other){
        if(&other!=this){
            if(m_canOverwrite){
                if(JniEnvironment env{200}){
                    jobject _object = object(env);
                    jobject otherObject = other.m_collection.object(env);
                    if(!env->IsSameObject(_object, otherObject)){
                        QtJambiAPI::clearJavaCollection(env, _object);
                        if(otherObject){
                            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, otherObject);
                            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                                QtJambiAPI::addToJavaCollection(env, _object, QtJambiAPI::nextOfJavaIterator(env, iterator));
                            }
                        }
                    }
                }
            }else{
                m_collection = other.m_collection;
            }
        }
        return *this;
    }

    JavaSequence& operator=(std::initializer_list<value_type> args){
        if(JniEnvironment env{200}){
            jobject _object = object(env);
            QtJambiAPI::clearJavaCollection(env, _object);
            for(const QVariant& arg : args){
                QtJambiAPI::addToJavaCollection(env, _object, ::qtjambi_cast<jobject>(env, arg));
            }
        }
        return *this;
    }

    const_iterator begin() const{
        if(JniEnvironment env{128}){
            (void)object(env);
            return JavaSequence::const_iterator(env, m_collection, 0);
        }else return {};
    }

    const_iterator end() const{
        if(JniEnvironment env{128}){
            (void)object(env);
            return JavaSequence::const_iterator(env, m_collection, -1);
        }else return {};
    }

    inline const_iterator constBegin() const {return begin();}

    inline const_iterator constEnd() const {return end();}

    inline const_iterator cbegin() const {return begin();}

    inline const_iterator cend() const {return end();}

    void push_back(const value_type& value){
        if(JniEnvironment env{200}){
            jobject _object = object(env);
            QtJambiAPI::addToJavaCollection(env, _object, ::qtjambi_cast<jobject>(env, value));
        }
    }

    jobject object(JNIEnv* env) const{
        jobject object = m_collection.object(env);
        if(!Java::Runtime::Collection::isInstanceOf(env, object)){
            m_collection.assign(env, object = QtJambiAPI::newJavaArrayList(env, 0));
        }
        return object;
    }
    static void destroy(void* sequence){
        delete reinterpret_cast<JavaSequence*>(sequence);
    }
private:
    bool m_canOverwrite;
    mutable JObjectWrapper m_collection;
};

struct ThreadEngineStarterWrapperPrivate;

namespace QtConcurrent{
struct ThreadEngineStarterWrapper{
    ThreadEngineStarterWrapper();
    ThreadEngineStarterWrapper(const ThreadEngineStarterWrapper&);
    ~ThreadEngineStarterWrapper();
    ThreadEngineStarterWrapper& operator=(const ThreadEngineStarterWrapper&);
    ThreadEngineStarterWrapper& operator=(ThreadEngineStarterWrapper&&);
    inline bool operator==(const ThreadEngineStarterWrapper& other)const{
        return d==other.d;
    }
    template<typename T>
    JavaSequence<T>& sequence(JNIEnv* env, jobject collection, bool canOverwrite = true){
        JavaSequence<T>* sequence = new JavaSequence<T>(env, collection, canOverwrite);
        setSequence(sequence, JavaSequence<T>::destroy);
        return *sequence;
    }
    template<typename V>
    jobject convert(JNIEnv* env, const QtConcurrent::ThreadEngineStarter<V>& _starter){
        setStarter(_starter);
        return qtjambi_cast<jobject>(env, std::move(*this));
    }
    jobject startAsynchronously(JNIEnv* env);
    void reportException(const QException &e)const;
    void reportException(std::exception_ptr e)const;
    static void destroy(void *ptr, bool);
private:
    void setStarter(const QtConcurrent::ThreadEngineStarter<void>& starter);
    void setStarter(const QtConcurrent::ThreadEngineStarter<QVariant>& starter);
    void setStarter(const QtConcurrent::ThreadEngineStarter<JObjectWrapper>& starter);
    void setSequence(void* sequence, void(*deleter)(void*));
    friend size_t qHash(const ThreadEngineStarterWrapper &value, size_t seed);
    QExplicitlySharedDataPointer<ThreadEngineStarterWrapperPrivate> d;
};
}

#ifdef HANDLE_EXCEPTION
#define THREAD_ENGINE_STARTER_ARG(A) , A
#else
#define THREAD_ENGINE_STARTER_ARG(A)
#endif

#endif // UTILS_P_H
