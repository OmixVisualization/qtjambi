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

package org.qtjambi.examples;

import java.util.List;

import org.qtjambi.examples.generator.AbstractGameObjectInterface;
import org.qtjambi.examples.generator.Game;
import org.qtjambi.examples.generator.GameAction;
import org.qtjambi.examples.generator.GameAnimation;
import org.qtjambi.examples.generator.GameObject;
import org.qtjambi.examples.generator.GameScene;
import org.qtjambi.examples.generator.Point3D;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.gui.QGraphicsView;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QPainterPath;

class InventoryAction extends GameAction
{
    @Override
    public GameAction clone() {
        return new InventoryAction();
    }

    @Override
    public boolean perform(GameScene scene) {
        if (objects().isEmpty()) {
            List<AbstractGameObjectInterface> inventory = scene.egoInventory();

            if (inventory.isEmpty()) {
                scene.message("Your pockets are empty");
            } else {
                String msg = "You are carrying:\n";
                for (AbstractGameObjectInterface gameObject : inventory) {
                    if (gameObject instanceof GameObject) {
                        msg += ((GameObject) gameObject).description() + "\n";
                    }
                }

                scene.message(msg);
            }

            return true;
        } else {
            return false;
        }
    }

    public InventoryAction() {
        super(Game.ActionType.resolve(Game.ActionType.UserAction.value()));
    }
}

class ChickenObject extends GameObject {

    public ChickenObject(GameScene scene, String name) {
        super(scene, name);

        used.connect(this, "youCantUseTheChicken()");
    }

    private void youCantUseTheChicken() {
        gameScene().message("You can't use the rubber chicken for anything");
    }
}

//@QtJambiExample(name = "Generator Example")
public class GeneratorExample extends GameScene {

    private static final String resourcesLocation = "classpath:org/qtjambi/examples/generator/images/";

    public GeneratorExample()
    {
        int w = 0; int h = 0;

        // Set up scene
        {
            setCacheMode(QGraphicsView.CacheModeFlag.CacheBackground);
            setDescription("You are standing next to a restaurant and it is sunny outside.");

            QImage img = new QImage(resourcesLocation + "background.png");
            setBackground(img);

            setHorizon(250.0);
            w = img.width(); h = img.height();
            setSceneRect(new QRectF(0.0, 0.0, w, h));
        }

        // Player avatar
        setEgoObject(makeEgo(w, h));

        // Inventory command
        grammar().addVerb("inventory", new InventoryAction());
        grammar().addVerb("inv", new InventoryAction());


        // Make boundary
        {
            QPainterPath path = new QPainterPath(new QPointF(0, 305));
            path.lineTo(new QPointF(306, 305));
            path.lineTo(new QPointF(326, 293));
            path.lineTo(new QPointF(357, 293));
            path.lineTo(new QPointF(378, 305));
            path.lineTo(new QPointF(635, 305));
            path.lineTo(new QPointF(635, 200));
            path.lineTo(new QPointF(0, 200));
            path.closeSubpath();

            GameObject boundary = new GameObject(this, "boundary");
            boundary.setVisible(false);
            boundary.setShape(path);
            boundary.setFlags(Game.ObjectFlag.Blocking);


            addGameObject(boundary);
        }

        // Make chicken
        {
            GameObject chicken = new ChickenObject(this, "a rubber chicken with a pulley in the middle");

            chicken.setDescription("It's a rubber chicken with a pulley in the middle.");
            chicken.addName("rubber chicken");
            chicken.addName("chicken");
            chicken.addName("rubber chicken with a pulley in the middle");
            chicken.addName("rubber chicken with pulley");
            chicken.addName("rubber chicken with pulley in the middle");
            chicken.addName("rubber chicken with pulley in middle");
            chicken.addName("chicken with a pulley in the middle");
            chicken.addName("chicken with pulley");
            chicken.addName("chicken with pulley in the middle");
            chicken.addName("chicken with pulley in middle");
            chicken.setVisible(true);
            chicken.setPosition(new Point3D(100.0, 330.0, 0.0));
            chicken.setFlags(Game.ObjectFlag.Blocking, Game.ObjectFlag.CanPickUp);

            GameAnimation a = new GameAnimation(Game.AnimationType.NoAnimation);
            a.addFrame(new QImage(resourcesLocation + "chicken.png"));
            chicken.setAnimation(a);

            addGameObject(chicken);
        }

        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        setWindowTitle(tr("Generator Example"));

        message("Press any letter to write a command and enter when you are done. Use the arrow keys to move around."
                     +" Hit enter when you are done reading this message.");
    }

    private GameAnimation makeAnimation(Game.AnimationType type, String nameTemplate, int startIdx, int endIdx) {
        GameAnimation a = new GameAnimation(type);

        a.setSpeed(100);
        a.setLooping(true);

        for (int i=startIdx; i<=endIdx; ++i)
            a.addFrame(new QImage(resourcesLocation + nameTemplate.replace("#", new Integer(i).toString())));

        return a;
    }

    private GameObject makeEgo(int w, int h) {
        GameObject ego = new GameObject(this);

        ego.setPosition(new Point3D(w / 2.0, 350.0, 0.0));
        ego.setVisible(true);

        ego.setAnimation(makeAnimation(Game.AnimationType.WalkingHorizontally, "walk#.png", 1, 4));
        ego.setAnimation(makeAnimation(Game.AnimationType.StandingStill, "walk#.png", 2, 2));
        ego.setAnimation(makeAnimation(Game.AnimationType.WalkingFromScreen, "walkaway#.png", 1, 2));
        ego.setAnimation(makeAnimation(Game.AnimationType.WalkingToScreen, "walktowards#.png", 1, 2));

        return ego;
    }

    public static void main(String[] args) {
        QApplication.initialize(args);
        GeneratorExample ex = new GeneratorExample();
        ex.show();
        QApplication.execStatic();
        QApplication.shutdown();

    }

}
