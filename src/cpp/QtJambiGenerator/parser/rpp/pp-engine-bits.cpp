/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <errno.h>

#include "rpp/pp-engine-bits.h"
#include "rpp/pp-cctype.h"

#include <QVector>
#include <QDebug>
#include <QStringList>
#include <QDir>

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

rpp::pp::pp(pp_environment &__env) :
        env(__env), expand_macro(env) {
    verbose = 0;
    iflevel = 0;
    _M_skipping[iflevel] = 0;
    _M_true_test[iflevel] = 0;
    m_M_skipFile = false;
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
    QString message = QStringLiteral(u"unknown directive '#%1' at %2:%3").arg(p_directive, env.current_file.absoluteFilePath(), QString::number(env.current_line));
    env.log(message.toStdString());
    return PP_UNKNOWN_DIRECTIVE;
}


QFileInfo rpp::pp::find_include_file(QString const &p_input_filename,
                                 INCLUDE_POLICY p_include_policy, bool p_skip_current_path)  {
    assert(!p_input_filename.isEmpty());
    QFileInfo result(p_input_filename);

    if(result.isFile())
        return result;

    if(p_include_policy == INCLUDE_LOCAL && !p_skip_current_path) {
        if(env.current_file.isDir()){
            result = QFileInfo(QDir(env.current_file.absoluteFilePath()).absoluteFilePath(p_input_filename));
            if(result.isFile())
                return result;
        }
        if(env.current_file.isFile()){
            result = QFileInfo(env.current_file.dir().absoluteFilePath(p_input_filename));
            if(result.isFile())
                return result;
        }
    }

    {
        QDir dir = env.current_file.isDir() ? QDir(env.current_file.absoluteFilePath()) : env.current_file.dir();
        if(dir.dirName()=="private"){
            dir.cdUp();
            dir.cdUp();
            dir.cdUp();
            std::string __tmp;
            result = QFileInfo(dir.absoluteFilePath(p_input_filename));
            if(result.isFile())
                return result;
        }
    }

    QDir curentPath = env.current_file.isDir() ? QDir(env.current_file.absoluteFilePath()) : env.current_file.dir();
    for(const QString& p : qAsConst(include_paths)){
        QDir path(p);
        if(p_skip_current_path && path==curentPath)
            continue;
#ifdef Q_OS_MAC
        /* On MacOSX for those not familiar with the platform, it can group a collection of things
         *  like libraries/header files as installable modules called a framework.  A framework has
         *  a well defined layout, so <OpenGL/gl.h> would be transformed into a path
         *  /.../OpenGL.framework/Headers/gl.h
         */
        QString string = p_input_filename;
        //QStringList list = string.split("/"); //could be used for error checks
        QString module = string.split("/")[0];
        if(!module.contains('.')) {
            QString _string = string;
            _string.replace(module + "/", module + ".framework/Headers/");
            _string = path.absolutePath() + _string;
            result = QFileInfo(path.absoluteFilePath(_string));
            if(!result.exists() || !result.isFile()) {
                _string = string;
                _string.replace(module + "/", "../lib/" + module + ".framework/Headers/");
                result = QFileInfo(path.absoluteFilePath(_string));
            }
            if(result.isFile())
                return result;
        }
#endif
        result = QFileInfo(path.absoluteFilePath(p_input_filename));
        if(result.isFile())
            return result;
    }
    return {};
}

long rpp::pp::eval_has_include(const QString& incl, bool next){
    bool chop = (incl.startsWith("<") && incl.endsWith(">"))
                || (incl.startsWith("\"") && incl.endsWith("\""));
    QFileInfo fileInfo = find_include_file(chop ? incl.mid(1, incl.length()-2) : incl, INCLUDE_GLOBAL, false);
    if (fileInfo.isFile()) {
        return 1;
    }
    static QStringList available{"<chrono>"};
    if(available.contains(incl))
        return 1;
    Q_UNUSED(next)
    /*
    QString message = QString(next ? "Unavailable: __has_include_next(%1)" : "Unavailable: __has_include(%1)").arg(incl);
    env.log(message.toStdString());
    */
    return 0;
}

long rpp::pp::eval_has_x(const QString& attr, bool isHasAttribute, bool isHasCppAttribute, bool isHasBuiltin, bool isHasFeature){
    Q_UNUSED(attr)
    Q_UNUSED(isHasAttribute)
    Q_UNUSED(isHasCppAttribute)
    Q_UNUSED(isHasBuiltin)
    Q_UNUSED(isHasFeature)
    //static QStringList available{};
    //if(available.contains(attr))
    //    return 1;
    /*
    if(isHasCppAttribute)
        env.log(QString("Unavailable: __has_cpp_attribute(%1)").arg(attr).toStdString());
    else if(isHasAttribute)
        env.log(QString("Unavailable: __has_attribute(%1)").arg(attr).toStdString());
    else if(isHasBuiltin)
        env.log(QString("Unavailable: __has_builtin(%1)").arg(attr).toStdString());
    else if(isHasFeature)
        env.log(QString("Unavailable: __has_feature(%1)").arg(attr).toStdString());
    */
    return 0;
}

QFileInfo rpp::pp::handle_include(bool skip_current_path, QString filename, int quote){
    QFileInfo fileInfo = find_include_file(filename, quote == '>' ?
                                                         INCLUDE_GLOBAL : INCLUDE_LOCAL, skip_current_path);
#if defined (PP_HOOK_ON_FILE_INCLUDED)
    PP_HOOK_ON_FILE_INCLUDED(env.current_file, fp ? filepath : filename, fp);
#endif

    if (fileInfo.isFile()) {
        return fileInfo;
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
            "QtCore/qjnitypes.h",
            "cassert"
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
        return {};
    }
}
