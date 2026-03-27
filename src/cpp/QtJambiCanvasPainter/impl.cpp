/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "hashes.h"
#include <QtCanvasPainter/private/qcanvasimage_p.h>
#include <QtCanvasPainter/private/qcanvascustombrush_p.h>

class QCanvasPathPrivate
{
public:
    static const QCanvasPathPrivate *get(const QCanvasPath *path) { return path->d_ptr; }
    QVarLengthArray<quint8> commands;
    QVarLengthArray<float> commandsData;
    qsizetype commandsCount = 0;
    qsizetype commandsDataCount = 0;
};

size_t qHash(const QCanvasImage &value, size_t seed){
    auto *d = QCanvasImagePrivate::get(&value);
    return qHashMulti(seed, d->id, d->width, d->height, d->sizeInBytes, d->type, d->tintColor);
}

size_t qHash(const QCanvasPath &value, size_t seed){
    auto *d = QCanvasPathPrivate::get(&value);
    seed = qHashMulti(seed, d->commandsCount, d->commandsDataCount);
    for (qsizetype i = 0; i < d->commandsCount; ++i) {
        seed = qHashMulti(seed, d->commands.at(i));
    }
    for (qsizetype i = 0; i < d->commandsDataCount; ++i) {
        seed = qHashMulti(seed, d->commandsData.at(i));
    }
    return seed;
}

size_t qHash(const QCanvasCustomBrush &value, size_t seed){
    auto *d = QCanvasCustomBrushPrivate::get(&value);
    return qHashMulti(seed, d->fragmentShader, d->vertexShader, d->timeRunning, d->time, d->data[0], d->data[1], d->data[2], d->data[3]);
}