/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright 2005 Roberto Raggi <roberto@kdevelop.org>
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef PP_ENGINE_BITS_H
#define PP_ENGINE_BITS_H

#include <string>
#include <vector>
#include <cstdio>

#include <QFile>
#include <QDebug>
#include <QString>
#include <QFileInfo>

#include <sys/stat.h>

#include "debuglog.h"
#include "pp-internal.h"
#include "pp-symbol.h"
#include "pp-cctype.h"
#include "pp-macro-expander.h"
#include "pp-environment.h"
#include "pp-scanner.h"

class QByteArray;

namespace rpp {

    enum PP_DIRECTIVE_TYPE {
        PP_UNKNOWN_DIRECTIVE,
        PP_DEFINE,
        PP_INCLUDE,
        PP_INCLUDE_NEXT,
        PP_ELIF,
        PP_ELSE,
        PP_ENDIF,
        PP_IF,
        PP_IFDEF,
        PP_IFNDEF,
        PP_UNDEF,
        PP_PRAGMA,
        PP_ERROR,
        PP_WARNING
    };

    enum INCLUDE_POLICY {
        INCLUDE_GLOBAL,
        INCLUDE_LOCAL
    };

    enum TOKEN_TYPE {
        TOKEN_NUMBER = 1000,
        TOKEN_UNUMBER,
        TOKEN_IDENTIFIER,
        TOKEN_DEFINED,
        TOKEN_LT_LT,
        TOKEN_LT_EQ,
        TOKEN_GT_GT,
        TOKEN_GT_EQ,
        TOKEN_EQ_EQ,
        TOKEN_NOT_EQ,
        TOKEN_OR_OR,
        TOKEN_AND_AND,
    };

    struct Value {
        enum Kind {
            Kind_Long,
            Kind_ULong,
        };

        Kind kind;

        union {
            long l;
            unsigned long ul;
        };

        bool is_ulong() const { return kind == Kind_ULong; }

        void set_ulong(unsigned long v) {
            ul = v;
            kind = Kind_ULong;
        }

        void set_long(long v) {
            l = v;
            kind = Kind_Long;
        }

        bool is_zero() const { return l == 0; }

#define PP_DEFINE_BIN_OP(name, op) \
  inline Value &name (const Value &other) \
  { \
    if (is_ulong () || other.is_ulong ()) \
      set_ulong (ul op other.ul); \
    else \
      set_long (l op other.l); \
    return *this; \
  }

        PP_DEFINE_BIN_OP(op_add, +)
        PP_DEFINE_BIN_OP(op_sub, -)
        PP_DEFINE_BIN_OP(op_mult, *)
        PP_DEFINE_BIN_OP(op_div, /)
        PP_DEFINE_BIN_OP(op_mod, %)
        PP_DEFINE_BIN_OP(op_lhs, <<)
        PP_DEFINE_BIN_OP(op_rhs, >>)
        PP_DEFINE_BIN_OP(op_lt, <)
        PP_DEFINE_BIN_OP(op_gt, >)
        PP_DEFINE_BIN_OP(op_le, <=)
        PP_DEFINE_BIN_OP(op_ge, >=)
        PP_DEFINE_BIN_OP(op_eq, ==)
        PP_DEFINE_BIN_OP(op_ne, !=)
        PP_DEFINE_BIN_OP(op_bit_and, &)
        PP_DEFINE_BIN_OP(op_bit_or, |)
        PP_DEFINE_BIN_OP(op_bit_xor, ^)
        PP_DEFINE_BIN_OP(op_and, &&)
        PP_DEFINE_BIN_OP(op_or, ||)

#undef PP_DEFINE_BIN_OP
    };

    class pp {

            union {
                long token_value;
                unsigned long token_uvalue;
                std::string *token_text;
            };

        public:

            pp(pp_environment &__env);


            /**
             * Loops though the given file and writes preprocessed
             * data to _OutputIterator result.
             */
            template <typename _InputIterator, typename _OutputIterator>
            void process(_InputIterator first, _InputIterator last, _OutputIterator result) {
#ifndef PP_NO_SMART_HEADER_PROTECTION
                std::string protection;
                protection.reserve(255);
                pp_fast_string tmp(protection.c_str(), protection.size());

                if (find_header_protection(first, last, &protection)
                        && env.resolve(&tmp) != nullptr) {
                    env.log(QString("** DEBUG found header protection:").arg(QString::fromStdString(protection)).toStdString());
                    return;
                }
#endif

                env.current_line = 1;
                char buffer[512];

                while (!m_M_skipFile) {
                    first = skip_blanks(first, last);
                    env.current_line += skip_blanks.lines;

                    if (first == last)
                        break;
                    else if (*first == '#') {
                        first = skip_blanks(++first, last);
                        env.current_line += skip_blanks.lines;

                        _InputIterator identifier_end = skip_identifier(first, last);
                        env.current_line += skip_identifier.lines;
                        std::size_t size = identifier_end - first;

                        assert(size < 512);
                        char *copy = buffer;
                        std::copy(first, identifier_end, copy);
                        copy[size] = '\0';

                        identifier_end = skip_blanks(identifier_end, last);
                        first = skip(identifier_end, last);

                        int was = env.current_line;
                        (void) handle_directive(buffer, size, identifier_end, first, result);

                        if (env.current_line != was) {
                            env.current_line = was;
                            _PP_internal::output_line(env.current_file, env.current_line, result);
                        }
                    } else if (*first == '\n') {
                        // ### compress the line
                        *result++ = *first++;
                        ++env.current_line;
                    } else if (skipping()) {
                        first = skip(first, last);
                    } else {
                        _PP_internal::output_line(env.current_file, env.current_line, result);
                        first = expand_macro(first, last, result);
                        env.current_line += expand_macro.lines;

                        if (expand_macro.generated_lines)
                            _PP_internal::output_line(env.current_file, env.current_line, result);
                    }
                }
            }

            /**
             * Opens given file and passes it to file(FILE*, _OutputIterator).
             */
            template <typename _OutputIterator>
            void file(QFileInfo const &fileInfo, _OutputIterator __result) {
                qDebug() << "Reading file: " << fileInfo.absoluteFilePath();
                QFile qfile(fileInfo.absoluteFilePath());
                if (qfile.open(QIODevice::ReadOnly)) {
                    bool skipFile = m_M_skipFile;
                    m_M_skipFile = false;
                    QFileInfo was = env.current_file;
                    env.current_file = fileInfo;
                    file(qfile, __result);
                    env.current_file = was;
                    m_M_skipFile = skipFile;
                } else {
                    env.log(QString("File '%1' not found!").arg(fileInfo.absoluteFilePath()).toStdString());
                }
            }

            /**
             * Reads contents of given file and passes contents of file
             * to operator(_InputIterator, _InputIterator, _OutputIterator).
             */
            template <typename _OutputIterator>
            void file(QFile& file, _OutputIterator result) {
                assert(file.isOpen());
                QByteArray data = file.readAll();
                if (data.isEmpty()) {
                    //std::cout << "pp-engine-bits.h[file(FILE*, _OutputIterator)]: Failed to read the file" << std::endl;
                    //exit(1);
                }
                file.close();
                if (!data.isEmpty()) {
                    process(data.constData(), (data.constData() + data.size()), result);
                }
            }

            template <typename _InputIterator>
            bool find_header_protection(_InputIterator __first, _InputIterator __last, std::string *__prot) {
                int was = env.current_line;

                while (__first != __last) {
                    if (pp_isspace(*__first)) {
                        if (*__first == '\n')
                            ++env.current_line;

                        ++__first;
                    } else if (_PP_internal::comment_p(__first, __last)) {
                        __first = skip_comment_or_divop(__first, __last);
                        env.current_line += skip_comment_or_divop.lines;
                    } else if (*__first == '#') {
                        __first = skip_blanks(++__first, __last);
                        env.current_line += skip_blanks.lines;

                        if (__first != __last && *__first == 'i') {
                            _InputIterator __begin = __first;
                            __first = skip_identifier(__begin, __last);
                            env.current_line += skip_identifier.lines;

                            std::string __directive(__begin, __first);

                            if (__directive == "ifndef") {
                                __first = skip_blanks(__first, __last);
                                env.current_line += skip_blanks.lines;

                                __begin = __first;
                                __first = skip_identifier(__first, __last);
                                env.current_line += skip_identifier.lines;

                                if (__begin != __first && __first != __last) {
                                    __prot->assign(__begin, __first);
                                    return true;
                                }
                            }
                        }
                        break;
                    } else
                        break;
                }

                env.current_line = was;
                return false;
            }


            QStringList include_paths;
            pp_skip_blanks skip_blanks;
            pp_skip_comment_or_divop skip_comment_or_divop;
            enum { MAX_LEVEL = 512 };
            int _M_skipping[MAX_LEVEL];
            int _M_true_test[MAX_LEVEL];
            int iflevel;
            int verbose;
            bool m_M_skipFile;

        private:
            pp_environment &env;

            pp_macro_expander expand_macro;
            pp_skip_identifier skip_identifier;
            pp_skip_number skip_number;
            std::string _M_current_text;

            std::string fix_file_path(std::string const &filename) const;

            bool is_absolute(std::string const &filename) const;

            bool file_isdir(std::string const &__filename) const;

            bool file_exists(std::string const &__filename) const;

            bool test_if_level();

            int skipping() const;

            /**
             * Returns preprocessor directive type, PP_DIRECTIVE_TYPE,
             * p_directive corresponds to.
             */
            PP_DIRECTIVE_TYPE find_directive(const char* p_directive, std::size_t p_size) const;

            /**
             * Finds correct include file from include paths or given data and returns
             * FILE pointer to that file.
             */
            QFileInfo find_include_file(QString const &__input_filename,
                                    INCLUDE_POLICY __include_policy, bool __skip_current_path);

            /**
             *
             */
            template <typename _InputIterator, typename _OutputIterator>
            _InputIterator handle_directive(char const *given_directive,
                                            std::size_t size,
                                            _InputIterator first,
                                            _InputIterator last,
                                            _OutputIterator result) {

                first = skip_blanks(first, last);

                PP_DIRECTIVE_TYPE directive = find_directive(given_directive, size);
                switch (directive) {
                    case PP_DEFINE:
                        if (! skipping())
                            return handle_define(first, last);
                        break;

                    case PP_INCLUDE:
                    case PP_INCLUDE_NEXT:
                        if (! skipping())
                            return handle_include(directive == PP_INCLUDE_NEXT, first, last, result);
                        break;

                    case PP_UNDEF:
                        if (! skipping())
                            return handle_undef(first, last);
                        break;

                    case PP_ELIF:
                        return handle_elif(first, last);

                    case PP_ELSE:
                        return handle_else(first, last);

                    case PP_ENDIF:
                        return handle_endif(first, last);

                    case PP_IF:
                        return handle_if(first, last);

                    case PP_IFDEF:
                        return handle_ifdef(false, first, last);

                    case PP_IFNDEF:
                        return handle_ifdef(true, first, last);
                    case PP_ERROR:
                        return handle_error(first, last);
                    case PP_PRAGMA:
                        if (! skipping())
                            return handle_pragma(first, last);
                        break;
                    default:
                        break;
                }

                return first;
            }

            template <typename _InputIterator, typename _OutputIterator>
            _InputIterator handle_include(bool skip_current_path, _InputIterator first, _InputIterator last,
                                          _OutputIterator result) {

                if (pp_isalpha(*first) || *first == '_') {
                    pp_macro_expander expand_include(env);
                    std::string name;
                    name.reserve(255);
                    expand_include(first, last, std::back_inserter(name));
                    std::string::iterator it = skip_blanks(name.begin(), name.end());
                    assert(it != name.end() && (*it == '<' || *it == '"'));
                    handle_include(skip_current_path, it, name.end(), result);
                    return first;
                }

                assert(*first == '<' || *first == '"');
                int quote = (*first == '"') ? '"' : '>';
                ++first;

                _InputIterator end_name = first;
                for (; end_name != last; ++end_name) {
                    assert(*end_name != '\n');

                    if (*end_name == quote)
                        break;
                }

                QString filename = QString::fromStdString(std::string(first, end_name));
                QFileInfo fileInfo = find_include_file(filename, quote == '>' ?
                                             INCLUDE_GLOBAL : INCLUDE_LOCAL, skip_current_path);

#if defined (PP_HOOK_ON_FILE_INCLUDED)
                PP_HOOK_ON_FILE_INCLUDED(env.current_file, fp ? filepath : filename, fp);
#endif

                if (fileInfo.isFile()) {
                    QFileInfo old_file = env.current_file;
                    env.current_file = fileInfo;
                    int __saved_lines = env.current_line;

                    env.current_line = 1;
                    //output_line (env.current_file, 1, __result);

                    file(fileInfo, result);

                    // restore the file name and the line position
                    env.current_file = old_file;
                    env.current_line = __saved_lines;

                    // sync the buffer
                    _PP_internal::output_line(env.current_file, env.current_line, result);
                } else {
#ifdef PP_OS_WIN
                    std::replace(filename.begin(), filename.end(), '\\', '/');
#endif
                    static QStringList ignoredFiles{
                            "type_traits",
                            "cstddef",
                            "utility",
                            "assert.h",
                            "stddef.h",
                            "algorithm",
                            "initializer_list",
                            "new",
                            "string.h",
                            "stdarg.h",
                            "string",
                            "string_view",
                            "iterator",
                            "list",
                            "stdlib.h",
                            "limits.h",
                            "vector",
                            "map",
                            "stdio.h",
                            "limits",
                            "exception",
                            "cmath",
                            "random",
                            "future",
                            "ctype.h",
                            "GLES2/gl2.h",
                            "AppKit/NSOpenGL.h",
                            "X11/Xlib.h",
                            "GL/glx.h",
                            "GL/gl.h",
                            "wingdi.h",
                            "float.h",
                            "set",
                            "numeric",
                            "functional",
                            "windows.h",
                            "Windows.h",
                            "xcb/xcb.h",
                            "qpa/qplatformscreen.h",
                            "CoreFoundation/CoreFoundation.h",
                            "qplatformdefs.h",
                            "QtAxContainer/QtAxContainer",
                            "QtActiveQt/QtActiveQtDepends",
                            "QtMacExtras/QtMacExtrasDepends",
                            "QtX11Extras/QtX11ExtrasDepends",
                            "QtWinExtras/QtWinExtrasDepends",
                            "ActiveQt/ActiveQtDepends",
                            "QtGui/qopengles2ext.h",
                            "time.h",
                            "inttypes.h",
                            "typeinfo",
                            "memory",
                            "mutex",
                            "atomic",
                            "array",
                            "filesystem",
                            "optional",
                            "variant",
                            "private/qlibrary_p.h",
                            "private/qobject_p.h",
                            "sys/types.h",
                            "unistd.h",
                            "GLES3/gl32.h",
                            "qopengl.h",
                            "cstring",
                            "tuple",
                            "vulkan/vulkan.h",
                            "QObject",
                            "qpromise.h",
                            "QScopedPointer",
                            "QtGlobal",
                            "QPointer",
                            "QAbstractListModel",
                            "QRectF",
                            "QLocale",
                            "QInputMethodEvent",
                            "QInputMethod",
                            "QVariant",
                            "QPointF",
                            "qanimationclipdata.h",
                            "qchannel.h",
                            "QStringList",
                            "cstdint",
                            "cstdlib",
                            "chrono",
                            "stdbool.h",
                            "climits",
                            "QtCore/qjnitypes.h"
                        };
                    if((verbose & DEBUGLOG_INCLUDE_ERRORS) != 0) {
                        QString current_file(env.current_file.absoluteFilePath());
                        if(!current_file.contains("private/")
                                && !current_file.endsWith("_p.h")
                                && !ignoredFiles.contains(filename)){
                            QString message = QString("No such file or directory: %1%2%3")
                                    .arg(quote != '>' ? '"' : '<')
                                    .arg(filename)
                                    .arg(quote != '>' ? '"' : '>');
                            env.log(message.toStdString());
                        }
                    }
                }

                return first;
            }

            template <typename _InputIterator>
            _InputIterator handle_error(_InputIterator __first, _InputIterator __last) {
                __first = skip_blanks(__first, __last);
                pp_skip_string_literal skip_string_literal;
                _InputIterator end_string_literal = skip_string_literal(__first, __last);
                pp_fast_string const *string_literal = pp_symbol::get(__first, end_string_literal);
                __first = end_string_literal;
                Q_UNUSED(string_literal)
                //std::cerr << "an error has occurred: " << std::string(string_literal->begin(), string_literal->end()) << std::endl;
                return __first;
            }

            template <typename _InputIterator>
            _InputIterator handle_define(_InputIterator __first, _InputIterator __last) {
                pp_macro macro;
#if defined (PP_WITH_MACRO_POSITION)
                macro.file = pp_symbol::get(env.current_file);
#endif
                std::string definition;

                __first = skip_blanks(__first, __last);
                _InputIterator end_macro_name = skip_identifier(__first, __last);
                pp_fast_string const *macro_name = pp_symbol::get(__first, end_macro_name);
                __first = end_macro_name;

                if (__first != __last && *__first == '(') {
                    macro.function_like = true;
                    macro.formals.reserve(5);

                    __first = skip_blanks(++__first, __last);    // skip '('
                    _InputIterator arg_end = skip_identifier(__first, __last);
                    if (__first != arg_end)
                        macro.formals.push_back(pp_symbol::get(__first, arg_end));

                    __first = skip_blanks(arg_end, __last);

                    if (*__first == '.') {
                        macro.variadics = true;
                        while (*__first == '.')
                            ++__first;
                    }

                    while (__first != __last && *__first == ',') {
                        __first = skip_blanks(++__first, __last);

                        arg_end = skip_identifier(__first, __last);
                        if (__first != arg_end)
                            macro.formals.push_back(pp_symbol::get(__first, arg_end));

                        __first = skip_blanks(arg_end, __last);

                        if (*__first == '.') {
                            macro.variadics = true;
                            while (*__first == '.')
                                ++__first;
                        }
                    }

                    assert(*__first == ')' || *__first == '\\');
                    ++__first;
                }

                __first = skip_blanks(__first, __last);

                while (__first != __last && *__first != '\n') {
                    if (*__first == '/') {
                        __first = skip_comment_or_divop(__first, __last);
                        env.current_line += skip_comment_or_divop.lines;
                        if(__first == __last || *__first == '\n')
                            break;
                    }

                    if (*__first == '\\') {
                        _InputIterator __begin = __first;
                        __begin = skip_blanks(++__begin, __last);

                        if (__begin != __last && *__begin == '\n') {
                            ++macro.lines;
                            __first = skip_blanks(++__begin, __last);
                            definition += '\n';
                            continue;
                        }
                    }

                    definition += *__first++;
                }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                QByteArray macroName(macro_name->begin(), int(macro_name->size()));
#else
                QByteArrayView macroName(macro_name->begin(), macro_name->size());
#endif
                if(macroName.startsWith("QT_FEATURE_")){
                    definition = "1";
                }else if(macroName.startsWith("QTJAMBI_FEATURE_")){
                    QByteArray feature("QT");
                    feature.append(macroName.mid(7));
                    pp_fast_string _macro_name(feature.constBegin(), feature.size());
                    if(rpp::pp_macro * macro = env.resolve(&_macro_name)){
                        env.unbind(macro->name);
                        macro_name = macro->name;
                    }
                }else if(macroName.startsWith("QT_DEPRECATED_VERSION_X")){
                    definition = "Q_DECL_DEPRECATED_X(text)";
                }else if(macroName.startsWith("QT_DEPRECATED_VERSION")){
                    definition = "Q_DECL_DEPRECATED";
                }else if(macroName=="QT_REQUIRE_CONFIG"){
                    return __first;
                }else if(macroName=="QT_DECL_METATYPE_EXTERN_TAGGED"
                         || macroName=="QT_IMPL_METATYPE_EXTERN_TAGGED"){
                    definition = "";
                }

                macro.definition = pp_symbol::get(definition);
                if((verbose & DEBUGLOG_DEFINE) != 0) {
                    std::cout << "#define " << std::string(macro_name->begin(), macro_name->end());
                    if(macro.function_like){
                        std::cout << "(";
                        bool b = false;
                        for(auto f : macro.formals){
                            if(b)
                                std::cout << ", ";
                            else
                                b = true;
                            std::cout << f->begin();
                        }
                        if(macro.variadics){
                            if(b)
                                std::cout << ", ";
                            else
                                b = true;
                            std::cout << "...";
                        }
                        std::cout << ")";
                    }
                    if(macro.definition->size()>0)
                        std::cout << " = " << std::string(macro.definition->begin(), macro.definition->end());
                    std::cout << std::endl;
                }
                //QT_DEPRECATED_VERSION_X
                env.bind(macro_name, macro);

                return __first;
            }

            /**
             * Invokes different skipping functions based the on first.
             */
            template <typename _InputIterator>
            _InputIterator skip(_InputIterator first, _InputIterator last) {
                pp_skip_string_literal skip_string_literal;
                pp_skip_char_literal skip_char_literal;

                while (first != last && *first != '\n') {
                    if (*first == '/') {
                        _InputIterator tmp = first;
                        first = skip_comment_or_divop(first, last);
                        if(tmp==first)
                            ++first;
                        env.current_line += skip_comment_or_divop.lines;
                    } else if (*first == '"') {
                        first = skip_string_literal(first, last);
                        env.current_line += skip_string_literal.lines;
                    } else if (*first == '\'') {
                        first = skip_char_literal(first, last);
                        env.current_line += skip_char_literal.lines;
                    } else if (*first == '\\') {
                        first = skip_blanks(++first, last);
                        env.current_line += skip_blanks.lines;

                        if (first != last && *first == '\n') {
                            ++first;
                            ++env.current_line;
                        }
                    } else
                        ++first;
                }

                return first;
            }


            template <typename _InputIterator>
            _InputIterator eval_primary(_InputIterator __first, _InputIterator __last, Value *result) {
                bool expect_paren = false;
                int token;
                __first = next_token(__first, __last, &token);
                bool isHasInclude = false;
                bool isHasCppAttribute = false;

                switch (token) {
                    case TOKEN_NUMBER:
                        result->set_long(token_value);
                        break;

                    case TOKEN_UNUMBER:
                        result->set_ulong(token_uvalue);
                        break;

                    case TOKEN_DEFINED:
                        __first = next_token(__first, __last, &token);

                        if (token == '(') {
                            expect_paren = true;
                            __first = next_token(__first, __last, &token);
                        }

                        if (token != TOKEN_IDENTIFIER) {
                            env.log(QString("Expected token INDENTIFIER, found token: %1").arg(token).toStdString());
                            result->set_long(0);
                            break;
                        }

                        result->set_long(env.resolve(token_text->c_str(), token_text->size()) != nullptr);

                        next_token(__first, __last, &token);    // skip '('

                        if (expect_paren) {
                            _InputIterator next = next_token(__first, __last, &token);
                            if (token != ')')
                                env.log("Expected \")\"");
                            else
                                __first = next;
                        }
                        break;

                    case '-':
                        __first = eval_primary(__first, __last, result);
                        result->set_long(- result->l);
                        return __first;

                    case '+':
                        __first = eval_primary(__first, __last, result);
                        return __first;

                    case '!':
                        __first = eval_primary(__first, __last, result);
                        result->set_long(result->is_zero());
                        return __first;

                    case TOKEN_IDENTIFIER: {
                        isHasInclude = _M_current_text==std::string("__has_include");
                        isHasCppAttribute = _M_current_text==std::string("__has_cpp_attribute");
                        result->set_long(0);
                        int _token;
                        _InputIterator ___first = next_token(__first, __last, &_token);
                        if(_token=='('){
                            token = _token;
                            __first = ___first;
                        }else if(_token==':'){
                            ___first = next_token(___first, __last, &_token);
                            if(_token==':'){
                                ___first = next_token(___first, __last, &_token);
                                if(_token==TOKEN_IDENTIFIER){
                                    token = _token;
                                    __first = ___first;
                                    break;
                                }
                            }
                        }else{
							if(rpp::pp_macro * macro = env.resolve(_M_current_text.c_str(), _M_current_text.size())){
                                if(macro->definition && !QLatin1String(macro->definition->begin(), macro->definition->size()).contains(QLatin1String(_M_current_text.c_str(), _M_current_text.size()))){
                                    std::string previous_text(_M_current_text);
                                    _M_current_text = std::string(macro->definition->begin(), macro->definition->end());
                                    eval_expression(macro->definition->begin(), macro->definition->end(), result);
                                    _M_current_text = previous_text;
                                }
                            }
                            break;
                        }
                    }
                    Q_FALLTHROUGH();
                    case '(':{
                        if(isHasInclude){
                            int _token;
                            _InputIterator ___first = next_token(__first, __last, &_token);
                            if(_token=='<'){
                                do{
                                    isHasInclude = false;
                                    ___first = next_token(___first, __last, &_token);
                                    if(_token=='>'){
                                        token = _token;
                                        __first = ___first;
                                        isHasInclude = true;
                                        break;
                                    }
                                }while(true);
                            }
                        }
                        if(isHasCppAttribute){
                            __first = eval_constant_expression(__first, __last, result);
                            next_token(__first, __last, &token);
                            result->set_long(1);
                        }else{
                            if(!isHasInclude)
                                __first = eval_constant_expression(__first, __last, result);
                            next_token(__first, __last, &token);

                            if(!isHasInclude){
                                while(token == ','){
                                    __first = next_token(__first, __last, &token);
                                    __first = eval_constant_expression(__first, __last, result);
                                    next_token(__first, __last, &token);
                                }
                            }
                        }

                        if (token != ')') {
                            pp_fast_string const *snippet = pp_symbol::get(__first, __last);
                            if(token<TOKEN_NUMBER)
                                env.log(QString("Expected \")\", found token \"%1\" in context \"%2\"").arg(QChar(token)).arg(QString::fromStdString(std::string(snippet->begin(), snippet->end()))).toStdString());
                            else
                                env.log(QString("Expected \")\", found token %1 in context \"%2\"").arg(token).arg(QString::fromStdString(std::string(snippet->begin(), snippet->end()))).toStdString());
                        } else {
                            __first = next_token(__first, __last, &token);
                        }
                        break;
                    }
                    default:
                        result->set_long(0);
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_multiplicative(_InputIterator __first, _InputIterator __last, Value *result) {
                __first = eval_primary(__first, __last, result);

                int token;
                _InputIterator next = next_token(__first, __last, &token);

                while (token == '*' || token == '/' || token == '%') {
                    Value value;
                    __first = eval_primary(next, __last, &value);

                    if (token == '*')
                        result->op_mult(value);
                    else if (token == '/') {
                        if (value.is_zero()) {
                            if(!QString::fromLatin1(next).startsWith(QLatin1String("QT_FEATURE_")))
                                env.log("division by zero");
                            result->set_long(0);
                        } else
                            result->op_div(value);
                    } else {
                        if (value.is_zero()) {
                            if(!QString::fromLatin1(next).startsWith(QLatin1String("QT_FEATURE_")))
                                env.log("modulo division by zero");
                            result->set_long(0);
                        } else
                            result->op_mod(value);
                    }
                    next = next_token(__first, __last, &token);
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_additive(_InputIterator __first, _InputIterator __last, Value *result) {
                __first = eval_multiplicative(__first, __last, result);

                int token;
                _InputIterator next = next_token(__first, __last, &token);

                while (token == '+' || token == '-') {
                    Value value;
                    __first = eval_multiplicative(next, __last, &value);

                    if (token == '+')
                        result->op_add(value);
                    else
                        result->op_sub(value);
                    next = next_token(__first, __last, &token);
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_shift(_InputIterator __first, _InputIterator __last, Value *result) {
                __first = eval_additive(__first, __last, result);

                int token;
                _InputIterator next = next_token(__first, __last, &token);

                while (token == TOKEN_LT_LT || token == TOKEN_GT_GT) {
                    Value value;
                    __first = eval_additive(next, __last, &value);

                    if (token == TOKEN_LT_LT)
                        result->op_lhs(value);
                    else
                        result->op_rhs(value);
                    next = next_token(__first, __last, &token);
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_relational(_InputIterator __first, _InputIterator __last, Value *result) {
                __first = eval_shift(__first, __last, result);

                int token;
                _InputIterator next = next_token(__first, __last, &token);

                while (token == '<'
                        || token == '>'
                        || token == TOKEN_LT_EQ
                        || token == TOKEN_GT_EQ) {
                    Value value;
                    __first = eval_shift(next, __last, &value);

                    switch (token) {
                        default:
                            assert(0);
                            break;

                        case '<':
                            result->op_lt(value);
                            break;

                        case '>':
                            result->op_gt(value);
                            break;

                        case TOKEN_LT_EQ:
                            result->op_le(value);
                            break;

                        case TOKEN_GT_EQ:
                            result->op_ge(value);
                            break;
                    }
                    next = next_token(__first, __last, &token);
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_equality(_InputIterator __first, _InputIterator __last, Value *result) {
                __first = eval_relational(__first, __last, result);

                int token;
                _InputIterator next = next_token(__first, __last, &token);

                while (token == TOKEN_EQ_EQ || token == TOKEN_NOT_EQ) {
                    Value value;
                    __first = eval_relational(next, __last, &value);

                    if (token == TOKEN_EQ_EQ)
                        result->op_eq(value);
                    else
                        result->op_ne(value);
                    next = next_token(__first, __last, &token);
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_and(_InputIterator __first, _InputIterator __last, Value *result) {
                __first = eval_equality(__first, __last, result);

                int token;
                _InputIterator next = next_token(__first, __last, &token);

                while (token == '&') {
                    Value value;
                    __first = eval_equality(next, __last, &value);
                    result->op_bit_and(value);
                    next = next_token(__first, __last, &token);
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_xor(_InputIterator __first, _InputIterator __last, Value *result) {
                __first = eval_and(__first, __last, result);

                int token;
                _InputIterator next = next_token(__first, __last, &token);

                while (token == '^') {
                    Value value;
                    __first = eval_and(next, __last, &value);
                    result->op_bit_xor(value);
                    next = next_token(__first, __last, &token);
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_or(_InputIterator __first, _InputIterator __last, Value *result) {
                __first = eval_xor(__first, __last, result);

                int token;
                _InputIterator next = next_token(__first, __last, &token);

                while (token == '|') {
                    Value value;
                    __first = eval_xor(next, __last, &value);
                    result->op_bit_or(value);
                    next = next_token(__first, __last, &token);
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_logical_and(_InputIterator __first, _InputIterator __last, Value *result) {
                __first = eval_or(__first, __last, result);

                int token;
                _InputIterator next = next_token(__first, __last, &token);

                while (token == TOKEN_AND_AND) {
                    Value value;
                    __first = eval_or(next, __last, &value);
                    result->op_and(value);
                    next = next_token(__first, __last, &token);
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_logical_or(_InputIterator __first, _InputIterator __last, Value *result) {
                __first = eval_logical_and(__first, __last, result);

                int token;
                _InputIterator next = next_token(__first, __last, &token);

                while (token == TOKEN_OR_OR) {
                    Value value;
                    __first = eval_logical_and(next, __last, &value);
                    result->op_or(value);
                    next = next_token(__first, __last, &token);
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_constant_expression(_InputIterator __first, _InputIterator __last, Value *result) {
                __first = eval_logical_or(__first, __last, result);

                int token;
                _InputIterator next = next_token(__first, __last, &token);

                if (token == '?') {
                    Value left_value;
                    __first = eval_constant_expression(next, __last, &left_value);
                    __first = skip_blanks(__first, __last);

                    __first = next_token(__first, __last, &token);
                    if (token == ':') {
                        Value right_value;
                        __first = eval_constant_expression(__first, __last, &right_value);

                        *result = !result->is_zero() ? left_value : right_value;
                    } else {
                        env.log(QString("Expected \":\", found token %1").arg(token).toStdString());
                        *result = left_value;
                    }
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator eval_expression(_InputIterator __first, _InputIterator __last, Value *result) {
                return __first = eval_constant_expression(skip_blanks(__first, __last), __last, result);
            }

            template <typename _InputIterator>
            _InputIterator handle_if(_InputIterator __first, _InputIterator __last) {
                if (test_if_level()) {
                    pp_macro_expander expand_condition(env);
                    std::string condition;
                    condition.reserve(255);
                    expand_condition(skip_blanks(__first, __last), __last, std::back_inserter(condition));

                    Value result;
                    result.set_long(0);
                    eval_expression(condition.c_str(), condition.c_str() + condition.size(), &result);

                    _M_true_test[iflevel] = !result.is_zero();
                    _M_skipping[iflevel] = result.is_zero();
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator handle_else(_InputIterator __first, _InputIterator /*__last*/) {
                if (iflevel == 0 && !skipping()) {
                    env.log("#else without #if");
                } else if (iflevel > 0 && _M_skipping[iflevel - 1]) {
                    _M_skipping[iflevel] = true;
                } else {
                    _M_skipping[iflevel] = _M_true_test[iflevel];
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator handle_elif(_InputIterator __first, _InputIterator __last) {
                assert(iflevel > 0);

                if (iflevel == 0 && !skipping()) {
                    env.log("#else without #if");
                } else if (!_M_true_test[iflevel] && !_M_skipping[iflevel - 1]) {
                    pp_macro_expander expand_condition(env);
                    std::string condition;
                    condition.reserve(255);
                    expand_condition(skip_blanks(__first, __last), __last, std::back_inserter(condition));

                    Value result;
                    result.set_long(0);
                    eval_expression(condition.c_str(), condition.c_str() + condition.size(), &result);

                    _M_true_test[iflevel] = !result.is_zero();
                    _M_skipping[iflevel] = result.is_zero();
                } else {
                    _M_skipping[iflevel] = true;
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator handle_endif(_InputIterator __first, _InputIterator /*__last*/) {
                if (iflevel == 0 && !skipping()) {
                    env.log("#endif without #if");
                } else {
                    _M_skipping[iflevel] = 0;
                    _M_true_test[iflevel] = 0;

                    --iflevel;
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator handle_ifdef(bool check_undefined, _InputIterator __first, _InputIterator __last) {
                if (test_if_level()) {
                    _InputIterator end_macro_name = skip_identifier(__first, __last);

                    std::size_t __size;
#if defined(__SUNPRO_CC)
                    std::distance(__first, end_macro_name, __size);
#else
                    __size = std::distance(__first, end_macro_name);
#endif
                    assert(__size < 256);

                    char __buffer [256];
                    std::copy(__first, end_macro_name, __buffer);

                    bool value = env.resolve(__buffer, __size) != nullptr;

                    __first = end_macro_name;

                    if (check_undefined)
                        value = !value;

                    _M_true_test[iflevel] = value;
                    _M_skipping[iflevel] = !value;
                }

                return __first;
            }

            template <typename _InputIterator>
            _InputIterator handle_pragma(_InputIterator __first, _InputIterator __last) {
                pp_macro_expander expand_condition(env);
                std::string condition;
                condition.reserve(255);
                expand_condition(skip_blanks(__first, __last), __last, std::back_inserter(condition));
                if(condition=="once"){
                    if(env.pragmaOnce()){
                        m_M_skipFile = true;
                    }
                }
                return __first;
            }

            template <typename _InputIterator>
            _InputIterator handle_undef(_InputIterator __first, _InputIterator __last) {
                __first = skip_blanks(__first, __last);
                _InputIterator end_macro_name = skip_identifier(__first, __last);
                assert(end_macro_name != __first);

                std::size_t __size;
#if defined(__SUNPRO_CC)
                std::distance(__first, end_macro_name, __size);
#else
                __size = std::distance(__first, end_macro_name);
#endif

                assert(__size < 256);

                char __buffer [256];
                std::copy(__first, end_macro_name, __buffer);

                pp_fast_string const __tmp(__buffer, __size);
                bool success = env.unbind(&__tmp);
                if((verbose & DEBUGLOG_UNDEF) != 0)
                    std::cout << "#undef " << std::string(__tmp.begin(), __tmp.end()) << (success ? "" : " failed!!!") << std::endl;

                __first = end_macro_name;

                return __first;
            }

            template <typename _InputIterator>
            char peek_char(_InputIterator __first, _InputIterator __last) {
                if (__first == __last)
                    return 0;

                return *++__first;
            }

            template <typename _InputIterator>
            _InputIterator next_token(_InputIterator __first, _InputIterator __last, int *kind) {
                __first = skip_blanks(__first, __last);

                if (__first == __last) {
                    *kind = 0;
                    return __first;
                }

                char ch = *__first;
                char ch2 = peek_char(__first, __last);

                switch (ch) {
                    case '/':
                        if (ch2 == '/' || ch2 == '*') {
                            __first = skip_comment_or_divop(__first, __last);
                            return next_token(__first, __last, kind);
                        }
                        ++__first;
                        *kind = '/';
                        break;

                    case '<':
                        ++__first;
                        if (ch2 == '<') {
                            ++__first;
                            *kind = TOKEN_LT_LT;
                        } else if (ch2 == '=') {
                            ++__first;
                            *kind = TOKEN_LT_EQ;
                        } else
                            *kind = '<';

                        return __first;

                    case '>':
                        ++__first;
                        if (ch2 == '>') {
                            ++__first;
                            *kind = TOKEN_GT_GT;
                        } else if (ch2 == '=') {
                            ++__first;
                            *kind = TOKEN_GT_EQ;
                        } else
                            *kind = '>';

                        return __first;

                    case '!':
                        ++__first;
                        if (ch2 == '=') {
                            ++__first;
                            *kind = TOKEN_NOT_EQ;
                        } else
                            *kind = '!';

                        return __first;

                    case '=':
                        ++__first;
                        if (ch2 == '=') {
                            ++__first;
                            *kind = TOKEN_EQ_EQ;
                        } else
                            *kind = '=';

                        return __first;

                    case '|':
                        ++__first;
                        if (ch2 == '|') {
                            ++__first;
                            *kind = TOKEN_OR_OR;
                        } else
                            *kind = '|';

                        return __first;

                    case '&':
                        ++__first;
                        if (ch2 == '&') {
                            ++__first;
                            *kind = TOKEN_AND_AND;
                        } else
                            *kind = '&';

                        return __first;

                    default:
                        if (pp_isalpha(ch) || ch == '_') {
                            _InputIterator end = skip_identifier(__first, __last);
                            _M_current_text.assign(__first, end);

                            token_text = &_M_current_text;
                            __first = end;

                            if (*token_text == "defined")
                                *kind = TOKEN_DEFINED;
                            else
                                *kind = TOKEN_IDENTIFIER;
                        } else if (pp_isdigit(ch)) {
                            _InputIterator end = skip_number(__first, __last);
                            std::string __str(__first, __last);
                            char ch = __str [__str.size() - 1];
                            if (ch == 'u' || ch == 'U') {
                                token_uvalue = strtoul(__str.c_str(), nullptr, 0);
                                *kind = TOKEN_UNUMBER;
                            } else {
                                token_value = strtol(__str.c_str(), nullptr, 0);
                                *kind = TOKEN_NUMBER;
                            }
                            __first = end;
                        } else
                            *kind = *__first++;
                }

                return __first;
            }

    };

} // namespace rpp

#endif // PP_ENGINE_BITS_H
