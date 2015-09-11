/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "pickupaction.h"

#include "gameobject.h"
#include "gamescene.h"

bool PickUpAction::perform(GameScene *scene)
{
    if (objects.size() > 0) {
        return false;
    } else {
        QList<AbstractGameObject *> cant_pick_up;
        for (int i=0; i<subjects.size(); ++i) {
            if (!(subjects.at(i)->objectFlags() & Game::CanPickUp)) {
                cant_pick_up.append(subjects.at(i));
            } else {
                scene->addToEgoInventory(subjects.at(i));
            }
        }

        QString msg;
        for (int i=0; i<cant_pick_up.size(); ++i) {
            if (i == 0) {
                msg += "You can't pick up ";
            } else if (i == cant_pick_up.size() - 1) {
                msg += " and ";
            } else {
                msg += ", ";
            }

            msg += cant_pick_up.at(i)->name();
        }

        if (!msg.isEmpty())
            scene->message(msg);

        return cant_pick_up.size() != subjects.size();
    }
}

GameAction *PickUpAction::clone() const
{
    PickUpAction *action = new PickUpAction;
    action->subjects = subjects;
    action->objects = objects;

    return action;
}

