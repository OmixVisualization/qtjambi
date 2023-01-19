#include <QtCore/QVariant>

template <class _Ty, size_t _Size>
constexpr size_t arraysize(const _Ty (&)[_Size]) noexcept {
    return _Size;
}

class QJSValue;

typedef QVariant(*Call)(QFunctionPointer fn, const QJSValue &);

static Call call[129];

template<int N>
struct FunctionInitializer{
    static void initialize(QFunctionPointer* functions){
        functions[N-1] = QFunctionPointer(f);
        FunctionInitializer<N-1>::initialize(functions);
    }

    static QVariant f(const QJSValue & v) {
        if(!call[N])reinterpret_cast<QFunctionPointer>(call[0])();
        return call[N](reinterpret_cast<QFunctionPointer>(&f), v);
    }
};

template<>
struct FunctionInitializer<0>{
    static void initialize(QFunctionPointer*){}
};

extern "C" Q_DECL_EXPORT Call* initialize(Call onNull, std::vector<QFunctionPointer>& functions){
    functions.resize(arraysize(call)-1);
    FunctionInitializer<arraysize(call)-1>::initialize(functions.data());
    call[0] = onNull;
    return call+1;
}

