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

#ifndef CPP_HEADER_GENERATOR
#define CPP_HEADER_GENERATOR

#include "cppgenerator.h"
#include "metalang.h"

class CppHeaderGenerator : public CppGenerator {
    public:
        CppHeaderGenerator(PriGenerator *pri);
        QString fileNameForClass(const MetaClass *cls) const override;
        QString fileNameForFunctional(const MetaFunctional *cls) const override;

        void write(QTextStream &s, const MetaClass *java_class, int nesting_level = 0) override;
        void write(QTextStream &s, const MetaFunctional *java_functional, int nesting_level = 0) override;
        void writeFunction(QTextStream &s, const MetaFunction *java_function, Option options = NoOption);
        void writeFunctionOverride(QTextStream &s, const MetaFunction *java_function, const QString& prefix);
        void writeForwardDeclareSection(QTextStream &s, const MetaClass *java_class);
        void writeForwardDeclareSection(QTextStream &s, const MetaFunctional *java_class);
        void writeVariablesSection(QTextStream &s, const MetaClass *java_class, bool isInterface);
        void writeFieldAccessors(QTextStream &s, const MetaField *java_field);
        static void writeInjectedCode(QTextStream &s, const MetaClass *java_class, const QList<CodeSnip::Position>& positions);
        static void writeInjectedCode(QTextStream &s, const MetaFunctional *java_class, const QList<CodeSnip::Position>& positions);
        bool shouldGenerate(const MetaClass *java_class) const override;
        bool shouldGenerate(const MetaFunctional *functional) const override;
        static bool shouldGenerateHeaders(const MetaClass *java_class);
};

#endif // CPP_HEADER_GENERATOR
