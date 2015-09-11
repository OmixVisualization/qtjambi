
#include <iostream>
//#define DEBUG_DEFUNDEF

#include "pp-environment.h"

void rpp::pp_environment::bind(pp_fast_string const *__name, pp_macro const &__macro) {
    std::size_t h = hash_code(*__name) % _M_hash_size;
    pp_macro *m = new pp_macro(__macro);
    m->name = __name;
    m->next = _M_base [h];
    m->hash_code = h;
    _M_base [h] = m;

    _M_macros.push_back(m);

    if (_M_macros.size() == _M_hash_size)
        rehash();
#ifdef DEBUG_DEFUNDEF
    std::string x__name = std::string();
    std::string x__macro = std::string();
    if(__macro.definition)
        x__macro.assign(__macro.definition->begin(), __macro.definition->size());
    else
        x__macro = "<nul>";
    std::cerr << "** DEFINE " << x__name.assign(__name->begin(), __name->size()) << " " << x__macro << std::endl;
#endif
}

void rpp::pp_environment::unbind(pp_fast_string const *__name) {
    if (pp_macro *m = resolve(__name))
        m->hidden = true;
#ifdef DEBUG_DEFUNDEF
    std::string x__name = std::string((const char *)__name);
    std::cerr << "** UNDEF " << x__name.assign(__name->begin(), __name->size()) << std::endl;
#endif
}

void rpp::pp_environment::unbind(char const *__s, std::size_t __size) {
    rpp::pp_fast_string __tmp(__s, __size);
    unbind(&__tmp);
}

rpp::pp_macro *rpp::pp_environment::resolve(pp_fast_string const *p_name) const {
    std::size_t h = hash_code(*p_name) % _M_hash_size;
    pp_macro *it = _M_base [h];

    while (it && it->name && it->hash_code == h && (*it->name != *p_name || it->hidden))
        it = it->next;

    return it;
}

rpp::pp_macro *rpp::pp_environment::resolve(char const *__data, std::size_t __size) const {
    pp_fast_string const __tmp(__data, __size);
    return resolve(&__tmp);
}

std::size_t rpp::pp_environment::hash_code(pp_fast_string const &s) const {
    std::size_t hash_value = 0;

    for (std::size_t i = 0; i < s.size(); ++i)
        hash_value = (hash_value << 5) - hash_value + s.at(i);

    return hash_value;
}

void rpp::pp_environment::rehash() {
    delete[] _M_base;

    _M_hash_size <<= 1;

    _M_base = (pp_macro **) memset(new pp_macro* [_M_hash_size], 0, _M_hash_size * sizeof(pp_macro*));
    for (std::size_t index = 0; index < _M_macros.size(); ++index) {
        pp_macro *elt = _M_macros [index];
        std::size_t h = hash_code(*elt->name) % _M_hash_size;
        elt->next = _M_base [h];
        elt->hash_code = h;
        _M_base [h] = elt;
    }
}
