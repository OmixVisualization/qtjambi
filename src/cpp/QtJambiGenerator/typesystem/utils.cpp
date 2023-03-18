/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#include <QtXml>

#include <reporthandler.h>

#include "../abstractgenerator.h"

#include "generator.h"			/* for isTargetPlatformArmCpu */
#include "utils.h"
#include "qmltypesystemreader.h"
#include "typedatabase.h"


QString strings_Object = QLatin1String("Object");
QString strings_String = QLatin1String("String");
QString strings_Thread = QLatin1String("Thread");
QString strings_char = QLatin1String("char");
QString strings_java_lang = QLatin1String("java.lang");
QString strings_jchar = QLatin1String("jchar");
QString strings_jobject = QLatin1String("jobject");
QString strings_jobjectArray = QLatin1String("jobjectArray");

//static void addRemoveFunctionToTemplates(TypeDatabase *db);

namespace TS{

bool TypeDatabase::defined(QString name){
    return m_defined && m_defined(name);
}

void TypeDatabase::setDefined(TypeDatabase::DefinedPtr&& function){
    m_defined = function;
}

QString Include::toString() const {
    if (type == IncludePath)
        return "#include <" + name + '>';
    else if (type == LocalPath)
        return "#include \"" + name + "\"";
    else
        return "import " + name + ";";
}

QString formattedCodeHelper(QTextStream &s, Indentor &indentor, QStringList &lines) {
    bool multilineComment = false;
    bool lastEmpty = true;
    QString lastLine;
    while (!lines.isEmpty()) {
        /*const*/ QString line = lines.takeFirst().trimmed();
// the former arm-qreal solution was nonsense because it leads to binary incompatibility
// the new solution overrides the qreal typedef during qtjambi wrapper generation.
        if (line.isEmpty()) {
            if (!lastEmpty)
                s << Qt::endl;
            lastEmpty = true;
            continue;
        } else {
            lastEmpty = false;
        }
        if (line.startsWith("/*"))
            multilineComment = true;

        if (multilineComment) {
            s << indentor;
            if (line.startsWith("*"))
                s << " ";
            s << line << Qt::endl;
            if (line.endsWith("*/"))
                multilineComment = false;
        } else if (line.startsWith("}")) {
            return line;
        } else if (line.endsWith("}")) {
            s << indentor << line << Qt::endl;
            return nullptr;
        } else if (line.endsWith("{")) {
            s << indentor << line << Qt::endl;
            QString tmp;
            {
                Indentation indent(indentor);
                tmp = formattedCodeHelper(s, indentor, lines);
            }
            if (!tmp.isNull()) {
                s << indentor << tmp << Qt::endl;
            }
            lastLine = tmp;
            continue;
        } else {
            s << indentor;
            if (!lastLine.isEmpty() &&
                    !lastLine.endsWith(";") &&
                    !line.startsWith("@") &&
                    !line.startsWith("//") &&
                    !lastLine.startsWith("//") &&
                    !lastLine.endsWith("}") &&
                    !line.startsWith("{"))
                s << "    ";
            s << line << Qt::endl;
        }
        lastLine = line;
    }
    return nullptr;
}


QTextStream &CodeSnip::formattedCode(QTextStream &s, Indentor &indentor) const {
    QStringList lst(code().split("\n"));
    while (!lst.isEmpty()) {
        QString tmp = formattedCodeHelper(s, indentor, lst);
        if (!tmp.isNull()) {
            s << indentor << tmp << Qt::endl;
        }
    }
    s.flush();
    return s;
}

QString TemplateInstance::expandCode() const {
    TemplateEntry *templateEntry = TypeDatabase::instance()->findTemplate(m_name);
    if (templateEntry) {
        QString code = templateEntry->code();
        for(const QString& key : replaceRules.keys()) {
            code.replace(key, replaceRules[key]);
        }
        QStringList lines = code.split("\n");
        while(!lines.isEmpty()){
            if(lines.last().trimmed().isEmpty()){
                lines.takeLast();
            }else{
                break;
            }
        }
        while(!lines.isEmpty()){
            if(lines.first().trimmed().isEmpty()){
                lines.takeFirst();
            }else{
                break;
            }
        }
        int sp = -1;
        QString spaces;
        QString cleanedCode;
        QTextStream s(&cleanedCode);
        s << m_indent << "// TEMPLATE - " << m_name << " - START" << Qt::endl;
        for(QString line : lines) {
            if(!line.isEmpty() && line[0]==QLatin1Char('\r')){
                line = line.mid(1);
            }
            if(sp<0 && line.isEmpty()){
                continue;
            }
            if(sp<0 && !QString(line).trimmed().isEmpty()){
                for(sp=0; sp<line.length(); ++sp){
                    if(line[sp]!=QLatin1Char(' ')){
                        break;
                    }
                }
                if(sp==0){
                    sp = 0;
                    for(; sp<lines[0].length(); ++sp){
                        if(lines[0][sp]!=QLatin1Char('\t')){
                            break;
                        }
                    }
                    spaces.fill(QLatin1Char('\t'), sp);
                }else{
                    spaces.fill(QLatin1Char(' '), sp);
                }
            }
            if(!QString(line).trimmed().isEmpty() || sp>=0){
                if(line.startsWith(spaces))
                    line = line.mid(sp);
                s << m_indent << line << Qt::endl;
            }
        }
        s << m_indent << "// TEMPLATE - " << m_name << " - END";
        return cleanedCode;
    } else {
        ReportHandler::warning("insert-template referring to non-existing template '" + m_name + "'");
    }
    return QString();
}

bool TemplateInstance::hasCode() const {
    TemplateEntry *templateEntry = TypeDatabase::instance()->findTemplate(m_name);
    if (templateEntry) {
        return templateEntry->hasCode();
    }
    return false;
}


QString FunctionModification::toString() const {
    QString str = signature + QLatin1String("->");
    if (modifiers & AccessModifierMask) {
        switch (modifiers & AccessModifierMask) {
            case Private:
                str += QLatin1String("private");
                break;
            case Protected:
                str += QLatin1String("protected");
                break;
            case Public:
                str += QLatin1String("public");
                break;
            case Friendly:
                str += QLatin1String("friendly");
                break;
        }
    }

    if (modifiers & Final) str += QLatin1String("final");
    if (modifiers & NonFinal) str += QLatin1String("non-final");
    if (modifiers & NativeDeclFinal) str += QLatin1String("declared-final");

    if (modifiers & Readable) str += QLatin1String("readable");
    if (modifiers & Writable) str += QLatin1String("writable");

    if (modifiers & CodeInjection) {
        for(const CodeSnip& s : snips) {
            str += QLatin1String("\n//code injection:\n");
            str += s.code();
        }
    }

    if (modifiers & Rename) str += QLatin1String("renamed:") + renamedToName;

    if (modifiers & Deprecated) str += QLatin1String("deprecate");

    if (modifiers & ReplaceExpression) str += QLatin1String("replace-expression");

    return str;
}

//static functions

/*
static void removeFunction(ComplexTypeEntry *e, const char *signature) {
    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.removal = TS::All;

    e->addFunctionModification(mod);
}




static void injectCode(ComplexTypeEntry *e,
                       const char *signature,
                       const QByteArray &code,
                       const ArgumentMap &args) {
    CodeSnip snip;
    snip.language = TS::NativeCode;
    snip.position = CodeSnip::Beginning;
    snip.addCode(QString::fromLatin1(code));
    snip.argumentMap = args;

    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.snips << snip;
    mod.modifiers = Modification::CodeInjection;
    e->addFunctionModification(mod);
}


static void addRemoveFunctionToTemplates(TypeDatabase *db)
{
    ContainerTypeEntry *qvector = db->findContainerType(QLatin1String("QVector"));
    removeFunction(qvector, "constData() const");
    removeFunction(qvector, "data() const");
    removeFunction(qvector, "data()");
    removeFunction(qvector, "first()");
    removeFunction(qvector, "last()");
    removeFunction(qvector, "operator[](int)");
    removeFunction(qvector, "operator[](int) const");
    removeFunction(qvector, "operator=(QVector<T>)");

    ContainerTypeEntry *qlist = db->findContainerType(QLatin1String("QList"));
    removeFunction(qlist, "constData() const");
    removeFunction(qlist, "data() const");
    removeFunction(qlist, "data()");
    removeFunction(qlist, "back()");
    removeFunction(qlist, "front()");
    removeFunction(qlist, "first()");
    removeFunction(qlist, "last()");
    removeFunction(qlist, "operator[](int)");
    removeFunction(qlist, "operator[](int) const");
    removeFunction(qlist, "operator=(QList<T>)");

    ContainerTypeEntry *qqueue = db->findContainerType(QLatin1String("QQueue"));
    removeFunction(qqueue, "head() const");


    ArgumentMap args1;
    args1[1] = QLatin1String("$1");
    ArgumentMap args2 = args1;
    args2[2] = QLatin1String("$2");

    QByteArray code =
        "\nif ($1 >= __qt_this->size() || $1 < 0) {"
        "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
        "\n                       QString::fromLatin1(\"Accessing container of size %3 at %4\")"
        "\n                       .arg(__qt_this->size()).arg($1).toLatin1() QTJAMBI_STACKTRACEINFO );"
        "\n   return;"
        "\n}";

    QByteArray code_with_return = QByteArray(code).replace("return;", "return $DEFAULT_VALUE_RETURN;");

    QByteArray code_index_length =
        "\nif ($1 < 0 || $2 < 0 || ($1 + $2) >= __qt_this->size()) {"
        "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
        "\n                       QString::fromLatin1(\"Accessing container of size %3 from %4 to %5\")"
        "\n                       .arg(__qt_this->size()).arg($1).arg($1+$2).toLatin1() QTJAMBI_STACKTRACEINFO );"
        "\n   return;"
        "\n}";

    QByteArray code_non_empty =
        "\nif (__qt_this->isEmpty()) {"
        "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
        "\n                       QString::fromLatin1(\"Accessing empty container...\").toLatin1() QTJAMBI_STACKTRACEINFO );"
        "\n   return;"
        "\n}";

    QByteArray code_two_indices =
        "\nif ($1 < 0 || $2 < 0 || $1 >= __qt_this->size() || $2 >= __qt_this->size()) {"
        "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
        "\n                       QString::fromLatin1(\"Accessing container of size %3 from %4 to %5\")"
        "\n                       .arg(__qt_this->size()).arg($1).arg($1+$2).toLatin1() QTJAMBI_STACKTRACEINFO );"
        "\n   return;"
        "\n}";

    { // QVector safty...
        injectCode(qvector, "at(int) const", code_with_return, args1);
        injectCode(qvector, "replace(int,T)", code, args1);
        injectCode(qvector, "remove(int)", code, args1);
        injectCode(qvector, "remove(int, int)", code_index_length, args2);
        injectCode(qvector, "pop_back()", code_non_empty, ArgumentMap());
        injectCode(qvector, "pop_front()", code_non_empty, ArgumentMap());
    }

    { // QList safty...
        injectCode(qlist, "at(int) const", code_with_return, args1);
        injectCode(qlist, "replace(int, T)", code, args1);
        injectCode(qlist, "pop_back()", code_non_empty, ArgumentMap());
        injectCode(qlist, "pop_front()", code_non_empty, ArgumentMap());
        injectCode(qlist, "swap(int, int)", code_two_indices, args2);
        injectCode(qlist, "move(int, int)", code_two_indices, args2);
        injectCode(qlist, "removeAt(int)", code, args1);
        injectCode(qlist, "takeAt(int)", code_with_return, args1);
    }

}*/

}

TypesystemException::TypesystemException(const QByteArray& message) : QException(), m_message(message)
{
}

TypesystemException *TypesystemException::clone() const{
    return new TypesystemException(this->m_message);
}

char const* TypesystemException::what() const noexcept{
    return m_message;
}

void TypesystemException::raise(const QString& message) {
    TypesystemException e(message.toUtf8());
    throw e;
}

void TypesystemException::raise() const {
    TypesystemException e(this->m_message);
    throw e;
}
