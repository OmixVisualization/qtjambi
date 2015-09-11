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

#include "gamescene.h"

#include "gamegrammar.h"
#include "gameobject.h"

#include <QtGui/QDialog>
#include <QtGui/QGraphicsView>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainterPath>

GameScene::GameScene(QWidget *parent)
    : QGraphicsView(parent), m_ego_object(0), m_horizon(100), m_farthest_z(3.0), m_closest_z(1.0)
{
    m_grammar = new GameGrammar(this);
    setScene(new QGraphicsScene());
    this->setCursor(QCursor(Qt::BlankCursor));
}

GameScene::~GameScene()
{
}

void GameScene::setEgoObject(AbstractGameObject *ego_object)
{
    m_ego_object = ego_object;
    ego_object->walk(Game::NoDirection);
    scene()->addItem(ego_object);
}

void GameScene::addGameObject(AbstractGameObject *gameObject)
{
    m_grammar->registerGameObject(gameObject);
    scene()->addItem(gameObject);
}

bool GameScene::inProximityOfEgo(const AbstractGameObject *o) const
{
    return ((o->position() - m_ego_object->position()).length() <= (m_ego_object->boundingRect().width()/2.0 + o->boundingRect().width()/2.0)*2);
}

void GameScene::message(const QString &msg)
{
    m_message.append(msg);
}

void GameScene::lookAround()
{
    message(m_description);

    QList<AbstractGameObject *> nearObjects;
    QList<QGraphicsItem *> items = scene()->items();
    foreach (QGraphicsItem *item, items) {
        AbstractGameObject *gameObject = static_cast<GameObject *>(item);

        if (gameObject->isVisible() && !gameObject->name().isEmpty()
            && inProximityOfEgo(gameObject)) {
            nearObjects.append(gameObject);
        }
    }

    QString objects_description;
    for (int i=0; i<nearObjects.size(); ++i) {
        if (i == 0) {
            objects_description = "You see ";
        } else if (i == nearObjects.size() - 1) {
            objects_description += " and ";
        } else {
            objects_description += ", ";
        }

        objects_description += nearObjects.at(i)->name();
    }

    if (!objects_description.isEmpty())
        message(objects_description);
}

void GameScene::keyPressEvent(QKeyEvent *e)
{
    QWidget *viewport = this->viewport();
    if (!m_message.isEmpty()) {
        if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
            m_message.pop_front();
            viewport->update();
        }
    } else {
        Game::WalkingDirection dir = Game::NoDirection;
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            m_grammar->parse(m_current_input);
            m_current_input = "";
            viewport->update();
            break;
        case Qt::Key_Left:
            dir = Game::Left;
            break;
        case Qt::Key_Right:
            dir = Game::Right;
            break;
        case Qt::Key_Up:
            dir = Game::Up;
            break;
        case Qt::Key_Down:
            dir = Game::Down;
            break;
        default:
            if ((e->key() >= Qt::Key_A && e->key() <= Qt::Key_Z)
                || (e->key() == Qt::Key_Space)) {
                m_current_input += e->text();
                viewport->update();
                } else if (e->key() == Qt::Key_Backspace) {
                    m_current_input.chop(e->count());
                    viewport->update();
                }
        }
        if (dir != Game::NoDirection && m_ego_object != 0) {
            if (m_ego_object->direction() == dir)
                m_ego_object->walk(Game::NoDirection);
            else
                m_ego_object->walk(dir);
        }

    }
}

void GameScene::drawForeground(QPainter *painter, const QRectF &)
{
    if (!m_message.isEmpty()) {
        QFontMetrics fm(font());

        int w = (int)(sceneRect().width() / 2.0);
        QRect brect = fm.boundingRect(0, 0, w, (int)sceneRect().height(), Qt::TextWordWrap, m_message.at(0));

        QRectF rect(sceneRect().width() / 2.0 - brect.width() / 2.0, sceneRect().height() / 2.0 - brect.height() / 2.0,
                    brect.width(), brect.height());
        painter->setPen(Qt::black);
        painter->setBrush(QColor(0xff, 0xff, 0xff, 0xe0));
        painter->drawRect(rect.adjusted(-10.0, -10.0, 10.0, 10.0));
        painter->drawText(rect, m_message.at(0));
    }

    if (!m_current_input.isEmpty()) {
        QFontMetrics fm(font());

        int height = fm.height() + 10;

        double wmargin = sceneRect().width() / 10.0;
        double hmargin = sceneRect().height() / 10.0;
        QRectF rect(wmargin, sceneRect().height() - hmargin - height, sceneRect().width() - wmargin * 2, height);

        painter->setPen(Qt::black);
        painter->setBrush(QColor(0xff, 0xff, 0xff, 0xe0));
        painter->drawRect(rect.adjusted(-10.0, -10.0, 10.0, 10.0));
        painter->drawText(rect, m_current_input);
    }
}

void GameScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, Qt::black);
    painter->drawImage(0, 0, m_current_background);
}

void GameScene::addToEgoInventory(AbstractGameObject *o)
{
    scene()->removeItem(o);
    m_ego_inventory.append(o);
}

