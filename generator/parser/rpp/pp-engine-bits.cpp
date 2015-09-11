/****************************************************************************
**
** Copyright (C) 2013-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** (in parts)
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#include <errno.h>

#include "rpp/pp-engine-bits.h"
#include "rpp/pp-cctype.h"

#include <QVector>
#include <QDebug>
#include <QStringList>
#include <QDir>

rpp::pp::pp(pp_environment &__env) :
        env(__env), expand_macro(env) {
    verbose = 0;
    iflevel = 0;
    _M_skipping[iflevel] = 0;
    _M_true_test[iflevel] = 0;
}

std::back_insert_iterator<std::vector<std::string> > rpp::pp::include_paths_inserter() {
    return std::back_inserter(rpp::pp::include_paths);
}

std::vector<std::string>::iterator rpp::pp::include_paths_begin() {
    return rpp::pp::include_paths.begin();
}

std::vector<std::string>::iterator rpp::pp::include_paths_end() {
    return rpp::pp::include_paths.end();
}

std::vector<std::string>::const_iterator rpp::pp::include_paths_begin() const {
    return rpp::pp::include_paths.begin();
}

std::vector<std::string>::const_iterator rpp::pp::include_paths_end() const {
    return rpp::pp::include_paths.end();
}

bool rpp::pp::is_absolute(std::string const &filename) const {
#if defined(PP_OS_WIN)
    return filename.length() >= 3
           && filename.at(1) == ':'
           && (filename.at(2) == '\\' || filename.at(2) == '/');
#else
    return filename.length() >= 1
           && filename.at(0) == '/';
#endif
}

bool rpp::pp::file_exists(std::string const &__filename) const {
    struct stat __st;
#if defined(PP_OS_WIN)
    return stat(__filename.c_str(), &__st) == 0;
#else
    return lstat(__filename.c_str(), &__st) == 0;
#endif
}

bool rpp::pp::file_isdir(std::string const &__filename) const {
    struct stat __st;
#if defined(PP_OS_WIN)
    if (stat(__filename.c_str(), &__st) == 0)
        return (__st.st_mode & _S_IFDIR) == _S_IFDIR;
    else
        return false;
#else
    if (lstat(__filename.c_str(), &__st) == 0)
        return (__st.st_mode & S_IFDIR) == S_IFDIR;
    else
        return false;
#endif
}

void rpp::pp::push_include_path(std::string const &path) {
    if (path.empty() || path [path.size() - 1] != PATH_SEPARATOR) {
        std::string tmp(path);
        tmp += PATH_SEPARATOR;
        if (find(rpp::pp::include_paths.begin(), rpp::pp::include_paths.end(), tmp) == rpp::pp::include_paths.end()) {
            rpp::pp::include_paths.push_back(tmp);
        }
    } else {
        if (find(rpp::pp::include_paths.begin(), rpp::pp::include_paths.end(), path) == rpp::pp::include_paths.end()) {
            rpp::pp::include_paths.push_back(path);
        }
    }
}

bool rpp::pp::test_if_level() {
    bool result = !rpp::pp::_M_skipping[rpp::pp::iflevel++];
    rpp::pp::_M_skipping[rpp::pp::iflevel] = rpp::pp::_M_skipping[rpp::pp::iflevel - 1];
    rpp::pp::_M_true_test[rpp::pp::iflevel] = false;
    return result;
}

int rpp::pp::skipping() const {
    return rpp::pp::_M_skipping[rpp::pp::iflevel];
}

rpp::PP_DIRECTIVE_TYPE rpp::pp::find_directive(char const *p_directive, std::size_t p_size) const {
    switch (p_size) {
        case 0: return PP_UNKNOWN_DIRECTIVE;
        case 2: //if
            if (p_directive[0] == 'i'
                    && p_directive[1] == 'f')
                return PP_IF;
            break;

        case 4: //elif, else
            if (p_directive[0] == 'e' && !strcmp(p_directive, "elif"))
                return PP_ELIF;
            else if (p_directive[0] == 'e' && !strcmp(p_directive, "else"))
                return PP_ELSE;
            break;

        case 5: //ifdef, undef, endif, error
            if (p_directive[0] == 'i' && !strcmp(p_directive, "ifdef"))
                return PP_IFDEF;
            else if (p_directive[0] == 'u' && !strcmp(p_directive, "undef"))
                return PP_UNDEF;
            else if (p_directive[0] == 'e') {
                if (!strcmp(p_directive, "endif"))
                    return PP_ENDIF;
                else if (!strcmp(p_directive, "error"))
                    return PP_ERROR;
            }
            break;

        case 6: //ifndef, define, pragma
            if (p_directive[0] == 'i' && !strcmp(p_directive, "ifndef"))
                return PP_IFNDEF;
            else if (p_directive[0] == 'd' && !strcmp(p_directive, "define"))
                return PP_DEFINE;
            else if (p_directive[0] == 'p' && !strcmp(p_directive, "pragma"))
                return PP_PRAGMA;
            break;

        case 7: //include, warning
            if (p_directive[0] == 'i' && !strcmp(p_directive, "include"))
                return PP_INCLUDE;
            else if (p_directive[0] == 'w' && !strcmp(p_directive, "warning"))
                return PP_WARNING;
            break;

        case 12: //include_next
            if (p_directive[0] == 'i' && !strcmp(p_directive, "include_next"))
                return PP_INCLUDE_NEXT;
            break;

        default:
            break;
    }
    std::cerr << "** WARNING unknown directive '#" << p_directive << "' at " << env.current_file << ":" << env.current_line << std::endl;
    return PP_UNKNOWN_DIRECTIVE;
}


FILE *rpp::pp::find_include_file(std::string const &p_input_filename, std::string *p_filepath,
                                 INCLUDE_POLICY p_include_policy, bool p_skip_current_path)  {
    assert(p_filepath != 0);
    assert(!p_input_filename.empty());

    p_filepath->assign(p_input_filename);

    if(is_absolute(*p_filepath))
        return std::fopen(p_filepath->c_str(), "r");

    if(!env.current_file.empty())
        _PP_internal::extract_file_path(env.current_file, p_filepath);

    if(p_include_policy == INCLUDE_LOCAL && !p_skip_current_path) {
        std::string __tmp(*p_filepath);
        __tmp += p_input_filename;

        if(file_exists(__tmp) && !file_isdir(__tmp)) {
            p_filepath->append(p_input_filename);
            if((verbose & DEBUGLOG_INCLUDE_DIRECTIVE) != 0)
                std::cout << "** INCLUDE local  " << *p_filepath << ": found" << std::endl;
            return std::fopen(p_filepath->c_str(), "r");
        }
    }

    std::vector<std::string>::const_iterator it = include_paths.begin();

    if(p_skip_current_path) {
        it = std::find(include_paths.begin(), include_paths.end(), *p_filepath);

        if(it != include_paths.end()) {
            ++it;
        } else {
            it = include_paths.begin();
        }
    }

    for(; it != include_paths.end(); ++it) {
        if(p_skip_current_path && it == include_paths.begin())
            continue;

        p_filepath->assign(*it);
        p_filepath->append(p_input_filename);

#ifdef Q_OS_MAC
        /* On MacOSX for those not familiar with the platform, it can group a collection of things
         *  like libraries/header files as installable modules called a framework.  A framework has
         *  a well defined layout, so <OpenGL/gl.h> would be transformed into a path
         *  /.../OpenGL.framework/Headers/gl.h
         */
        QString string = QString::fromStdString(p_input_filename);
        //QStringList list = string.split("/"); //could be used for error checks
        QString module = string.split("/")[0];
        if(!module.contains('.')) {
            string.replace(module + "/", module + ".framework/Headers/");
            string = QString::fromStdString(*it) + string;
            QFileInfo file = QFileInfo(string);
            if(file.exists() && file.isFile()) {
                QString path = QString::fromStdString(*it) + module + ".framework/Headers";
                push_include_path(path.toStdString());
                if((verbose & DEBUGLOG_INCLUDE_DIRECTIVE) != 0)
                    std::cout << "** INCLUDE system " << string.toStdString() << ": found" << std::endl;
                return std::fopen(string.toLatin1().data(), "r");
            }
        }
#endif
        if(file_exists(*p_filepath) && !file_isdir(*p_filepath)) {
            if((verbose & DEBUGLOG_INCLUDE_DIRECTIVE) != 0)
                std::cout << "** INCLUDE system " << *p_filepath << ": found" << std::endl;
            return std::fopen(p_filepath->c_str(), "r");
        }

        // Log all search attempts
        if((verbose & DEBUGLOG_INCLUDE_FULL) != 0)
            std::cout << "** INCLUDE system " << *p_filepath << ": " << strerror(errno) << std::endl;
    }

    return 0;
}
