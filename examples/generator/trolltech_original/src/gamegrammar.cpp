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

#include "gamegrammar.h"

#include "useaction.h"
#include "lookaction.h"
#include "pickupaction.h"
#include "gamescene.h"
#include "abstractgameobject.h"

GameGrammar::GameGrammar(GameScene *scene) : m_scene(scene)
{
    addVerb("use", new UseAction());
    addVerb("look", new LookAction());
    addVerb("take", new PickUpAction());
    addVerb("pick", new PickUpAction());
}

GameGrammar::~GameGrammar()
{
}

void GameGrammar::parse(const QString &c)
{
    m_current_command = c.toLower().split(' ');

    GameAction *action = command();
    if (action == 0) {
        m_scene->message("You can't do that");
    } else {
        action->perform(m_scene);
    }
}

void GameGrammar::registerGameObject(AbstractGameObject *gameObject)
{
    if (gameObject != 0) {
        m_objects[gameObject->name()] = gameObject;

        QStringList names = gameObject->otherNames();
        foreach (QString name, names) {
            m_objects[name] = gameObject;
        }
    } else {
        qWarning("GameGrammar::registerGameObject: Tried to add null object to grammar");
    }
}

void GameGrammar::addNameToGameObject(AbstractGameObject *gameObject, const QString &other_name)
{
    if (gameObject != 0) {
        m_objects[other_name] = gameObject;
    } else {
        qWarning("GameGrammar::addNameToGameObject: Tried to add null object to grammar");
    }
}

QString GameGrammar::currentToken(int i) const
{
    QString returned;
    if (m_current_command.size() < i)
        return QString();

    for (int j=0; j<i; ++j) {
        if (j > 0) returned += " ";
        returned += m_current_command.at(j);
    }

    return returned;
}

void GameGrammar::nextToken()
{
    m_current_command.pop_front();
}

GameAction *GameGrammar::command()
{
    GameAction *action = verb();
    if (action == 0)
        return 0;

    while (filler()) ;

    while (AbstractGameObject *s = object()) action->addSubject(s);

    while (filler()) ;

    while (AbstractGameObject *o = object()) action->addObject(o);

    return m_current_command.isEmpty() ? action : 0;
}

GameAction *GameGrammar::verb()
{
    GameAction *a = m_actions.value(currentToken(), 0);
    if (a != 0) {
        nextToken();
        return a->clone();
    } else {
        return 0;
    }
}

AbstractGameObject *GameGrammar::object()
{
    AbstractGameObject *gameObject = 0;
    int i=1;
    while (gameObject == 0 && !currentToken(i).isEmpty())
    {
        gameObject = m_objects.value(currentToken(i++), 0);
    }

    if (gameObject != 0 && gameObject->isVisible()
        && (m_scene->egoHasInInventory(gameObject) || m_scene->inProximityOfEgo(gameObject))) {
        while (--i) nextToken();
        while (and_token()) ;
        return gameObject;
    } else {
        return 0;
    }
}

bool GameGrammar::and_token()
{
    if (currentToken() == "and"
        || currentToken() == ",") {
        nextToken();
        return true;
    } else {
        return false;
    }
}

bool GameGrammar::filler()
{
    if (currentToken() == "to"
        || currentToken() == "with"
        || currentToken() == "in"
        || currentToken() == "at"
        || currentToken() == "up") {
        nextToken();
        return true;
    } else {
        return false;
    }
}
