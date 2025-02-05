/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
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

#ifndef DOCINDEXREADER_H
#define DOCINDEXREADER_H

#include <QtCore>
#include <QtXml>
#include "docmodel.h"

class DocIndexReader
{
public:
    DocIndexReader();
    const DocModel* readDocIndexes(const QDir& docDirectory, QThread* targetThread);
private:
    void analyzeNamespace(const QDir& subdir, DocModel* model, const QString& url, const QDomElement& element);
    void analyzeClass(const QDir& subdir, DocModel* model, const QString& url, const QDomElement& element);
    void analyzeEnum(const QDir& subdir, DocModel* model, const QString& url, const QDomElement& element);
    template<class DocFunctionOwner>
    void analyzeFunction(const QDir& subdir, DocFunctionOwner* owner, const QString& url, const QDomElement& element);
    void analyzeVariable(const QDir& subdir, DocClass* cls, const QString& url, const QDomElement& element);
    void analyzeTypeDef(const QDir& subdir, DocClass* cls, const QString& url, const QDomElement& element);
    void analyzeProperty(const QDir& subdir, DocClass* cls, const QString& url, const QDomElement& element);
};

#endif // DOCINDEXREADER_H
