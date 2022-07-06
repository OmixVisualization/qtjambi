#include <QtCore/QMetaType>

typedef QtMetaTypePrivate::VariantData(*Call)(QFunctionPointer fn, const void * const *, int, uint);

static Call call = nullptr;

QtMetaTypePrivate::VariantData function_pointer(const void * const * ptr, int id, uint flg) {
	Q_ASSERT(call);
    return call(reinterpret_cast<QFunctionPointer>(&function_pointer), ptr, id, flg);
}

extern "C" Q_DECL_EXPORT QFunctionPointer initialize(Call _call){
    call = _call;
    return reinterpret_cast<QFunctionPointer>(&function_pointer);
}

