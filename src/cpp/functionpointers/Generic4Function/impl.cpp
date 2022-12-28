#include <QtCore/QtGlobal>
#include <QtCore/QVector>

template <class _Ty, size_t _Size>
constexpr size_t arraysize(const _Ty (&)[_Size]) noexcept {
    return _Size;
}

typedef qintptr(*Call)(QFunctionPointer fn, qintptr p1, qintptr p2, qintptr p3, qintptr p4);

static Call call[129];

template<int N>
struct FunctionInitializer{
    static void initialize(QFunctionPointer* functions){
        functions[N-1] = QFunctionPointer(f);
        FunctionInitializer<N-1>::initialize(functions);
    }

    static qintptr f(qintptr p1, qintptr p2, qintptr p3, qintptr p4) {
        if(!call[N])reinterpret_cast<QFunctionPointer>(call[0])();
        return call[N](reinterpret_cast<QFunctionPointer>(&f), p1, p2, p3, p4);
    }
};

template<>
struct FunctionInitializer<0>{
    static void initialize(QFunctionPointer*){}
};

extern "C" Q_DECL_EXPORT Call* initialize(Call onNull, QVector<QFunctionPointer>& functions){
    functions.resize(arraysize(call)-1);
    FunctionInitializer<arraysize(call)-1>::initialize(functions.data());
    call[0] = onNull;
    return call+1;
}
