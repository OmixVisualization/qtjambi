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

#include "gameobject.h"

#include "gamescene.h"

GameObject::GameObject(GameScene *scene, const QString &name)
: m_name(name), m_direction(Game::NoDirection), m_size_in_depth(1.0), m_scene(scene),
  m_current_animation(Game::NoAnimation), m_movement_factor(0.05), m_old_factor(0.0)
{
    startTimer(10);
    m_time.start();

    setScene(scene);
}

GameObject::~GameObject()
{
}

void GameObject::walk(Game::WalkingDirection direction)
{
    m_direction = direction;

    switch (direction) {
    case Game::Right:
        setFlipped(true);
        setCurrentAnimation(Game::WalkingHorizontally, true);
        break;
    case Game::Left:
        setFlipped(false);
        setCurrentAnimation(Game::WalkingHorizontally, true);
        break;
    case Game::Up:
        setCurrentAnimation(Game::WalkingFromScreen, true);
        break;
    case Game::Down:
        setCurrentAnimation(Game::WalkingToScreen, true);
        break;
    default:
        setCurrentAnimation(Game::StandingStill, false);
        break;
    }
}


Game::WalkingDirection GameObject::direction() const
{
    return m_direction;
}

QString GameObject::name() const
{
    return m_name;
}

QStringList GameObject::otherNames() const
{
    return m_other_names;
}

Point3D GameObject::position() const
{
    return m_position;
}

void GameObject::addName(const QString &other_name)
{
    m_other_names.append(other_name);
    if (m_scene != 0)
        m_scene->addNameToGameObject(this, other_name);
}


QPainterPath GameObject::shape() const
{
    if (!m_shape.isEmpty()) {
        return m_shape;
    } else {
        QPainterPath path;
        path.addRect(boundingRect());
        return path;
    }
}

void GameObject::showDescription() const
{
    m_scene->message(m_description);
}

Game::ObjectFlags GameObject::objectFlags() const
{
    return m_flags;
}

bool GameObject::canMove(const Point3D &pos)
{
    bool returned = true;

    GameAnimation *a = animation(m_current_animation);
    int w = 0; int h = 0;
    if (a != 0) {
        QImage img = a->currentFrame();
        w = img.width(); h = img.height();
    }

    QPainterPath walkPath;
    if (m_direction != Game::Left && m_direction != Game::Right) {
        walkPath = QPainterPath(this->pos() + QPointF(-w / 2.0, h / 2.0));
        walkPath.lineTo(this->pos() + QPointF(w / 2.0, h / 2.0));
        walkPath.lineTo(QPointF(pos.x(), pos.y()) + QPointF(w / 2.0, h / 2.0));
        walkPath.lineTo(QPointF(pos.x(), pos.y()) + QPointF(-w / 2.0, h / 2.0));
        walkPath.closeSubpath();
    } else {
        if (m_direction == Game::Left) w *= -1;
        walkPath = QPainterPath(this->pos() + QPointF(w / 2.0, h / 2.0));
        walkPath.lineTo(this->pos() + QPointF(w / 2.0, h / 2.0 + 1.0));
        walkPath.lineTo(QPointF(pos.x(), pos.y()) + QPointF(w / 2.0, h / 2.0));
        walkPath.lineTo(QPointF(pos.x(), pos.y()) + QPointF(w / 2.0, h / 2.0 + 1.0));
        walkPath.closeSubpath();
    }

    QList<QGraphicsItem *> items = m_scene->scene()->items();
    foreach (QGraphicsItem *item, items) {
        AbstractGameObject *gameObject = static_cast<AbstractGameObject *>(item);
        if (gameObject->collidesWithPath(gameObject->mapFromScene(walkPath)) && (gameObject->objectFlags() & Game::Blocking)) {
            returned = false;
            break;
        }
    }

    return returned;
}

void GameObject::timerEvent(QTimerEvent *)
{
    GameAnimation *a = animation(m_current_animation);
    if (a != 0 && a->update())
        update();

    int elapsed = m_time.elapsed();
    if (m_scene != 0 && elapsed > 0 && direction() != Game::NoDirection) {
        Point3D pos = position();

        int anim_width = (a != 0 ? a->currentFrame().width() / 2 : 0);
        int anim_height = (a != 0 ? a->currentFrame().height() / 2 : 0);
        if (direction() == Game::Left) {
            pos.rx() -= elapsed * m_movement_factor;
            if (pos.x() < anim_width) pos.rx() = anim_width;
        } else if (direction() == Game::Right) {
            pos.rx() += elapsed * m_movement_factor;
            if (pos.x() + anim_width > m_scene->background().width())
                pos.rx() = m_scene->background().width() - anim_width;
        } else if (direction() == Game::Up) {
            pos.ry() -= elapsed * m_movement_factor;
            if (pos.y() < m_scene->horizon()) pos.ry() = m_scene->horizon();
        } else if (direction() == Game::Down) {
            pos.ry() += elapsed * m_movement_factor;
            if (pos.y() + anim_height > m_scene->background().height()) pos.ry() = m_scene->background().height() - anim_height;
        }
        qreal dist = m_scene->farthestZ() - m_scene->closestZ();
        pos.rz() = m_scene->farthestZ() - ((m_scene->height() - pos.y()) / m_scene->height()) * dist;
        if (canMove(pos))
            setPosition(pos);
    }

    if (elapsed > 0)
        m_time.restart();
}

void GameObject::setPosition(const Point3D &position)
{
    setPos(position.x(), position.y());
    setZValue(position.z());
    m_position = position;
}


bool GameObject::inProximityOfEgo() const
{
    return m_scene->inProximityOfEgo(this);
}

void GameObject::perform(Game::ActionType action, AbstractGameObject **args, int num_args)
{
    switch (action) {
    case Game::Use:
        if (num_args == 0) {
            emit used();
        } else {
            for (int i=0; i<num_args; ++i) {
                emit usedWith(args[i]);
            }
        }
        break ;
    case Game::Look:
        if (num_args == 0) {
            if (inProximityOfEgo() || m_scene->egoHasInInventory(this)) {
                showDescription();
            }
        } else {
            qWarning("GameObject::perform: LookAt action does not take any arguments");
        }
        break ;
    default:
        qWarning("GameObject::perform: Unhandled action '%d'", int(action));
        break;
    }
}

void GameObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    GameAnimation *a = animation(m_current_animation);
    if (a != 0) {
        a->update();

        QImage img = a->currentFrame();
        int w = img.width(); int h = img.height();
        painter->drawImage(QRect(-w / 2, -h / 2, w, h), img);
    }
}

QRectF GameObject::boundingRect() const
{
    if (!m_shape.isEmpty()) {
        return m_shape.boundingRect();
    } else {
        GameAnimation *a = animation(m_current_animation);
        if (a != 0) {
            QImage img = a->currentFrame();
            int w = img.width(); int h = img.height();
            return QRectF(-w / 2, -h / 2, w, h);
        }

        return QRectF();
    }
}
