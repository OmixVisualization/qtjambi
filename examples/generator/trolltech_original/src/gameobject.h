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

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "gamenamespace.h"
#include "point3d.h"
#include "gameanimation.h"
#include "abstractgameobject.h"
#include "gamescene.h"

#include <QtCore/QHash>
#include <QtCore/QTime>
#include <QtCore/QPointer>



class GameObject: public QObject, public AbstractGameObject
{
    Q_OBJECT
public:
    enum {
        Type = UserType + 1
    };

    GameObject(GameScene *scene, const QString &name = QString());
    ~GameObject();

    int type() const
    {
        return Type;
    }

    void setPosition(const Point3D &position);
    inline void setSizeInDepth(qreal size_in_depth) { m_size_in_depth = size_in_depth; }
    inline void setFlags(Game::ObjectFlags flags) { m_flags = flags; }
    inline void setScene(GameScene *scene) { m_scene = scene; }
    inline void setShape(const QPainterPath &path) { m_shape = path; }
    inline void setMovementFactor(qreal movement_factor) { m_movement_factor = movement_factor; }
    inline void setDescription(const QString &description) { m_description = description; }

    inline void setAnimation(GameAnimation *animation)
    {
        if (animation != 0 && animation->parent() == 0) {
            animation->setParent(this);
            m_animations[animation->type()] = animation;
        } else {
            qWarning("Attempted to add invalid animation to object '%s'", qPrintable(m_name));
        }
    }
    inline void setCurrentAnimation(Game::AnimationType type, bool looping = false)
    {
        m_current_animation = type;
        GameAnimation *a = animation(type);
        if (a != 0) {
            a->setLooping(looping);
            a->setCurrentFrame(0);
        }
    }
    void addName(const QString &other_name);

    inline void setFlipped(bool on)
    {
        if (on && !(m_flags & Game::Flipped)) {
            m_flags |= Game::Flipped;
            scale(-1.0, 1.0);
        } else if (!on && (m_flags & Game::Flipped)) {
            m_flags &= ~Game::Flipped;
            scale(-1.0, 1.0);
        }
    }



    inline Point3D &rposition() { return m_position; }

    inline QString description() const { return m_description; }
    inline qreal sizeInDepth() const { return m_size_in_depth; }
    inline GameAnimation *animation(Game::AnimationType type) const { return m_animations.value(type, 0); }
    inline GameScene *gameScene() const { return m_scene; }
    inline qreal movementFactor() const { return m_movement_factor; }
    inline Game::AnimationType currentAnimation() const { return m_current_animation; }
    bool inProximityOfEgo() const;

    virtual QPainterPath shape() const;

    bool canMove(const Point3D &pos);

    void showDescription() const;

    virtual QRectF boundingRect() const;
    virtual void perform(Game::ActionType action, AbstractGameObject **args, int num_args);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual Game::WalkingDirection direction() const;
    virtual QString name() const;
    virtual QStringList otherNames() const;
    virtual Point3D position() const;
    virtual void walk(Game::WalkingDirection direction);
    virtual Game::ObjectFlags objectFlags() const;

protected:
    virtual void timerEvent(QTimerEvent *);

signals:
    void usedWith(AbstractGameObject *other);
    void used();

private:
    QString m_name;
    QString m_description;
    Point3D m_position;
    Game::WalkingDirection m_direction;
    qreal m_size_in_depth;
    QPointer<GameScene> m_scene;
    Game::AnimationType m_current_animation;
    qreal m_movement_factor;
    qreal m_old_factor;
    QHash<Game::AnimationType, GameAnimation *> m_animations;
    QStringList m_other_names;
    Game::ObjectFlags m_flags;
    QTime m_time;
    QPainterPath m_shape;
};

#endif
