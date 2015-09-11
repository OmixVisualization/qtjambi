/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef GENERATOR_SET_JAVA_H
#define GENERATOR_SET_JAVA_H

#include "generatorset.h"
#include "metajavabuilder.h"

class GeneratorSetJava : public GeneratorSet {
        Q_OBJECT

    public:
        GeneratorSetJava();

        QString usage();
        bool readParameters(const QMap<QString, QString> args);

        void buildModel(const QString pp_file);
        void dumpObjectTree();

        QString generate();

    private:

        bool no_java;
        bool no_cpp_h;
        bool no_cpp_impl;
        bool no_metainfo;
        bool build_class_list;
        bool build_qdoc_japi;
        bool docs_enabled;
        bool do_ui_convert;
        bool native_jump_table;
        bool qtjambi_debug_tools;

        QString doc_dir;
        QString ui_file_name;
        QString custom_widgets;

        MetaJavaBuilder builder;

};

#endif // GENERATOR_SET_JAVA_H
