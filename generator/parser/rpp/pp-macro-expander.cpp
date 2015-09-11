
#include "rpp/pp-macro-expander.h"

std::string const *rpp::pp_macro_expander::resolve_formal(pp_fast_string const *name) {
    assert(name != 0);

    if (! frame)
        return 0;

    assert(frame->expanding_macro != 0);

    std::vector<pp_fast_string const *> const formals = frame->expanding_macro->formals;
    for (std::size_t index = 0; index < formals.size(); ++index) {
        pp_fast_string const *formal = formals[index];

        if (*formal != *name)
            continue;

        else if (frame->actuals && index < frame->actuals->size())
            return &(*frame->actuals)[index];

        else
            assert(0);  // internal error?
    }

    return 0;
}
