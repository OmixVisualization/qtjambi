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
package org.qtjambi.examples.svg;

import java.util.LinkedList;
import java.util.List;
import java.util.Random;

import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QCloseEvent;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QLinearGradient;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QPixmapCache;
import org.qtjambi.qt.svg.QGraphicsSvgItem;
import org.qtjambi.qt.svg.QSvgRenderer;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QFrame;
import org.qtjambi.qt.widgets.QGraphicsItem;
import org.qtjambi.qt.widgets.QGraphicsScene;
import org.qtjambi.qt.widgets.QGraphicsSceneMouseEvent;
import org.qtjambi.qt.widgets.QGraphicsView;
import org.qtjambi.qt.widgets.QStyleOptionGraphicsItem;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "SVG Card Deck",
                canInstantiate = "call-static-method:notWebstart")
//! [0]
public class SvgCards extends QGraphicsView {
//! [0]
    private boolean closing = false;

    private static String[] CARDS = {
        "black_joker",
        "red_joker",
        "back",
        "king_club",
        "king_diamond",
        "king_heart",
        "king_spade",
        "queen_club",
        "queen_diamond",
        "queen_heart",
        "queen_spade",
        "jack_club",
        "jack_diamond",
        "jack_heart",
        "jack_spade",
        "1_club",
        "1_diamond",
        "1_heart",
        "1_spade",
        "2_club",
        "2_diamond",
        "2_heart",
        "2_spade",
        "3_club",
        "3_diamond",
        "3_heart",
        "3_spade",
        "4_club",
        "4_diamond",
        "4_heart",
        "4_spade",
        "5_club",
        "5_diamond",
        "5_heart",
        "5_spade",
        "6_club",
        "6_diamond",
        "6_heart",
        "6_spade",
        "7_club",
        "7_diamond",
        "7_heart",
        "7_spade",
        "8_club",
        "8_diamond",
        "8_heart",
        "8_spade",
        "9_club",
        "9_diamond",
        "9_heart",
        "9_spade",
        "10_club",
        "10_diamond",
        "10_heart",
        "10_spade"
    };

    @Override
    protected void closeEvent(QCloseEvent e) {
        closing = true;
    }

//! [1]
    private static class Card extends QGraphicsSvgItem {
        private double opacity = 1.0;
        private CardManager manager;

        public Card(String card, QSvgRenderer renderer) {
//! [1] //! [2]
            super();
            setElementId(card);
            setSharedRenderer(renderer);
            setParent(renderer);
        }
//! [2]

        public void setManager(CardManager newManager) {
            manager = newManager;
        }

        public String cardName() {
            return elementId();
        }

        @Override
//! [3]
        public void mousePressEvent(QGraphicsSceneMouseEvent event) {
            setZValue(10);
//! [3]
            opacity = 0.7;
            manager.startedMove(this);
            super.mousePressEvent(event);
//! [4]
        }
//! [4]

        @Override
//! [5]
        public void mouseReleaseEvent(QGraphicsSceneMouseEvent event) {
//! [5]
            opacity = 1.0;
//! [6]
            setZValue(5);
//! [6]
            manager.stoppedMove(this);
            super.mouseReleaseEvent(event);
//! [7]
        }
//! [7]

        @Override
        public void paint(QPainter painter, QStyleOptionGraphicsItem option,
                          QWidget widget) {
			painter.setOpacity(opacity);
            super.paint(painter, option, widget);
        }
//! [8]
    }
//! [8]

    private static class CardBox extends QGraphicsItem {
        private QRectF rectangle;
        private String status = "Loading Cards...";

        public CardBox() {
            super();
            rectangle = new QRectF(0, 0, 200, 50);
        }

        @Override
        public QRectF boundingRect() {
            return rectangle;
        }

        @Override
        public void paint(QPainter painter, QStyleOptionGraphicsItem option,
                          QWidget widget) {

            painter.setRenderHint(QPainter.RenderHint.Antialiasing);
            painter.setPen(new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black)));
            QLinearGradient gradient = new QLinearGradient(rectangle.topLeft(),
                                                           rectangle.bottomLeft());
            gradient.setColorAt(0, new QColor(0, 0, 0, 127));
            gradient.setColorAt(1, new QColor(255, 255, 255, 127));
            painter.setBrush(new QBrush(gradient));
            painter.drawRoundRect(rectangle, (int) (25 * rectangle.height()
                                                    / rectangle.width()), 25);
            painter.save();

            QFont font = painter.font();
            font.setBold(true);
            font.setUnderline(true);
            painter.setFont(font);
            painter.drawText(new QPointF(rectangle.x() + 10, rectangle.y() + 20),
                             "Svg Cards");

            painter.restore();

            painter.drawText(new QPointF(rectangle.x() + 20, rectangle.y() + 40),
                             status);
        }

        void cardMoving(String card) {
            status = "Moving : " + card;
        }

        void cardStopped(String card) {
            status = "Dropped : " + card;
        }

        void cardInfo(String info) {
            status = info;
        }
    }

//! [9]
    private static class CardDeck extends QObject {
        private QSvgRenderer renderer;
        private String fileName;
        private List<Card> cards;

        public CardDeck(String file, QObject parent) {
            super(parent);

            fileName = file;
            renderer = new QSvgRenderer(fileName, parent);
            cards = new LinkedList<Card>();
            for (int i = 0; i < CARDS.length; ++i) {
                Card item = new Card(CARDS[i], renderer);
                cards.add(item);
            }

        }
//! [9]

        public List<Card> cards() {
            return cards;
        }
//! [10]
    }
//! [10]

    private static class CardManager extends QObject {
        private CardBox box;

        public void setBox(CardBox newBox) {
            box = newBox;
        }

        public void startedMove(Card card) {
            box.cardMoving(card.cardName());
            box.update(box.boundingRect());
        }

        public void stoppedMove(Card card) {
            box.cardStopped(card.cardName());
            box.update(box.boundingRect());
        }

        public void setOperation(String operation) {
            box.cardInfo(operation);
            box.update(box.boundingRect());
        }
    }

    private CardDeck deck;
    private CardManager manager;
    private Random random;
    private int cardsToLoad = 13;
    private int totalCards = cardsToLoad;
    private int x = 100;
    private int y = 100;
    private QGraphicsScene scene;

//! [11]
    public final void loadCards() {
        if (cardsToLoad != 0 && !closing) {
            addCard(random.nextInt(50));
            --cardsToLoad;
            if (cardsToLoad != 0) {
                double percent = (totalCards - cardsToLoad)
                                 / (double) totalCards * 100.0;
                manager.setOperation("Loading Cards : " + (int) percent + "% ");
            } else {
                manager.setOperation("Click on a Card");
            }
            viewport().update();
            QApplication.invokeLater(new Runnable() {
                public void run() { loadCards(); } });
        }
    }
//! [11]

//! [12]
    public SvgCards() {
        scene = new QGraphicsScene(this);
        setScene(scene);
//! [12]

//! [13]
        deck = new CardDeck("classpath:org/qtjambi/images/svg-cards.svg",
                            this);
//! [13]
        manager = new CardManager();
        random = new Random();

        CardBox box = new CardBox();
        box.setPos(sceneRect().width() - box.boundingRect().width() - 10,
                   sceneRect().height()- box.boundingRect().height() - 10);
        box.setZValue(99);
        scene.addItem(box);
        manager.setBox(box);

        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        setWindowTitle("SVG Cards Example");

        QLinearGradient grad = new QLinearGradient(0, 0, 800, 600);
        grad.setColorAt(0, QColor.fromRgbF(0.5, 0.5, 0.7));
        grad.setColorAt(1, QColor.fromRgbF(1, 1, 1));
        setRenderHint(QPainter.RenderHint.Antialiasing);
        setBackgroundBrush(new QBrush(grad));
        setFrameStyle(QFrame.Shape.NoFrame.value());

        QPixmapCache.setCacheLimit(5 * 1024);

//! [14]
        QApplication.invokeLater(new Runnable() {
                    public void run() { loadCards(); } });
//! [14] //! [15]
    }
//! [15]

    @Override
    public QSize sizeHint() {
        return new QSize(800, 600);
    }

//! [16]
    private final void addCard(int i) {
        Card item = deck.cards().get(i);
        while (item.scene() != null) {
            item = deck.cards().get(random.nextInt(50));
        }
//! [16]

        item.setRotation(0 + 180.0 * random.nextDouble());
        double scaleF = 0.5 + 0.9 * random.nextDouble();
        item.setScale(scaleF);
        x += 80;
        if (x >= 650) {
            x = 100;
            y += 100;
        }
//! [17]
        item.setPos(x, y);
        item.setFlag(QGraphicsItem.GraphicsItemFlag.ItemIsMovable, true);
//! [17]
        item.setManager(manager);
//! [18]
        scene().addItem(item);
    }
//! [18]

    public static void main(String args[]) {
        QApplication.initialize(args);

        SvgCards view = new SvgCards();
        view.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

    public static boolean notWebstart() {
        return System.getProperty("org.qtjambi.launcher.webstart") == null;
    }
}
