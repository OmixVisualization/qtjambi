#include <QtCore/QtGlobal>
#include <vector>

template <class _Ty, size_t _Size>
constexpr size_t arraysize(const _Ty (&)[_Size]) noexcept {
    return _Size;
}

typedef qintptr(*Call)(QFunctionPointer fn, qintptr p1, qintptr p2, qintptr p3, qintptr p4, qintptr p5, qintptr p6, qintptr p7, qintptr p8, qintptr p9, qintptr p10);
static Call call[129];

template<int N>
struct FunctionInitializer{
    static void initialize(QFunctionPointer* functions){
        functions[N-1] = QFunctionPointer(f);
        FunctionInitializer<N-1>::initialize(functions);
    }

    static qintptr f(qintptr p1, qintptr p2, qintptr p3, qintptr p4, qintptr p5, qintptr p6, qintptr p7, qintptr p8, qintptr p9, qintptr p10) {
        if(!call[N])reinterpret_cast<QFunctionPointer>(call[0])();
        return call[N](reinterpret_cast<QFunctionPointer>(&f), p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
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
