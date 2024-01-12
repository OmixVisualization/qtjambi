/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of QtJambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/QString>
#include <iostream>
//#define DEBUG_DEFUNDEF

#include "pp-environment.h"

void rpp::pp_environment::bind(pp_fast_string const *__name, pp_macro const &__macro) {
    _M_featureRegistry(std::string(__name->begin()), __macro.definition ? std::string(__macro.definition->begin()) : std::string(), current_file, true);
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
    std::cerr << "** DEFINE " << x__name.assign(__name->begin(), __name->size()) << " " << x__macro << " ** in " << qPrintable(current_file.absoluteFilePath()) << std::endl;
#endif
}

bool rpp::pp_environment::unbind(pp_fast_string const *__name) {
    bool result{false};
    if (pp_macro *m = resolve(__name)){
        m->hidden = true;
        _M_featureRegistry(std::string(__name->begin()), m->definition ? std::string(m->definition->begin()) : std::string(), current_file, false);
        result = true;
    }
#ifdef DEBUG_DEFUNDEF
    std::string x__name = std::string((const char *)__name);
    std::cerr << "** UNDEF " << x__name.assign(__name->begin(), __name->size()) << " ** in " << qPrintable(current_file.absoluteFilePath()) << std::endl;
#endif
    return result;
}

bool rpp::pp_environment::unbind(char const *__s, std::size_t __size) {
    rpp::pp_fast_string __tmp(__s, __size);
    return unbind(&__tmp);
}

rpp::pp_macro *rpp::pp_environment::resolve(pp_fast_string const *p_name) const {
    std::size_t h = hash_code(*p_name) % _M_hash_size;
    pp_macro *it = _M_base [h];

    while (it && it->name && it->hash_code == h && (*it->name != *p_name || it->hidden))
        it = it->next;

    return it;
}

bool rpp::pp_environment::pragmaOnce(){
    bool result = _M_pragmaOnceFiles.contains(current_file.absoluteFilePath());
    if(!result){
        _M_pragmaOnceFiles.insert(current_file.absoluteFilePath());
    }
    return result;
}

rpp::pp_macro *rpp::pp_environment::resolve(char const *__data, std::size_t __size) const {
    pp_fast_string const __tmp(__data, __size);
    return resolve(&__tmp);
}

std::size_t rpp::pp_environment::hash_code(pp_fast_string const &s) const {
    std::size_t hash_value = 0;

    for (std::size_t i = 0; i < s.size(); ++i)
        hash_value = (hash_value << 5) - hash_value + std::size_t(s.at(i));

    return hash_value;
}

void rpp::pp_environment::rehash() {
    delete[] _M_base;

    _M_hash_size <<= 1;

    _M_base = reinterpret_cast<pp_macro **>(memset(new pp_macro* [_M_hash_size], 0, _M_hash_size * sizeof(pp_macro*)));
    for (std::size_t index = 0; index < _M_macros.size(); ++index) {
        pp_macro *elt = _M_macros [index];
        std::size_t h = hash_code(*elt->name) % _M_hash_size;
        elt->next = _M_base [h];
        elt->hash_code = h;
        _M_base [h] = elt;
    }
}

MsgHandler rpp::MessageUtil::_m_messageHandler = [](const std::string &str){std::cerr << str;};

void rpp::MessageUtil::installMessageHandler(MsgHandler handler) {
    _m_messageHandler = handler;
}

void rpp::MessageUtil::message(const std::string &message){
    if(_m_messageHandler){
        _m_messageHandler(message);
    }
}

void rpp::pp_environment::log(const std::string &message) const{
    QString _message = QString("%1 (in %2:%3)")
            .arg(message.c_str())
            .arg(current_file.absoluteFilePath(), QString::number(current_line));
    rpp::MessageUtil::message(_message.toStdString());
}
