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

import java.util.Random;

import org.qtjambi.qt.core.QBasicTimer;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTimerEvent;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QKeyEvent;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QFrame;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QLCDNumber;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QWidget;

//! [0] //! [1]
class TetrixBoard extends QFrame
{
//! [0]
    static final int redTable[] = new int[8];
    static final int greenTable[] = new int[8];
    static final int blueTable[] = new int[8];
    static {
        redTable[0] = 0x00;
        redTable[1] = 0xCC;
        redTable[2] = 0x66;
        redTable[3] = 0x66;
        redTable[4] = 0xCC;
        redTable[5] = 0xCC;
        redTable[6] = 0x66;
        redTable[7] = 0xDA;

        greenTable[0] = 0x00;
        greenTable[1] = 0x66;
        greenTable[2] = 0xCC;
        greenTable[3] = 0x66;
        greenTable[4] = 0xCC;
        greenTable[5] = 0x66;
        greenTable[6] = 0xCC;
        greenTable[7] = 0xAA;

        blueTable[0] = 0x00;
        blueTable[1] = 0x66;
        blueTable[2] = 0x66;
        blueTable[3] = 0xCC;
        blueTable[4] = 0x66;
        blueTable[5] = 0xCC;
        blueTable[6] = 0xCC;
        blueTable[7] = 0x00;

    };

    enum TetrixShape { NoShape, ZShape, SShape, LineShape, TShape, SquareShape,
                   LShape, MirroredLShape }
    private static final int BoardWidth = 10;
    private static final int BoardHeight = 22;
//! [1]

//! [2]
    private boolean isStarted = false;
    private boolean isPaused = false;
    private boolean isWaitingAfterLine = false;
    private int numLinesRemoved = 0;
    private int numPiecesDropped = 0;
    private int score = 0;
    private int level = 0;
    private int curX = 0;
    private int curY = 0;
    private QLabel nextPieceLabel = null;
    private QBasicTimer timer = new QBasicTimer();
//! [2] //! [3]
    private TetrixPiece curPiece = new TetrixPiece();
    private TetrixPiece nextPiece = new TetrixPiece();
    private TetrixShape board[] = new TetrixShape[BoardWidth * BoardHeight];
//! [3]

//! [4]
    public Signal1<Integer> scoreChanged = new Signal1<Integer>();
    public Signal1<Integer> levelChanged = new Signal1<Integer>();
    public Signal1<Integer> linesRemovedChanged = new Signal1<Integer>();
//! [4]

//! [5]
    public TetrixBoard(QWidget parent)
    {
        super(parent);

        setFrameStyle(QFrame.Shape.Panel.value() | QFrame.Shadow.Sunken.value());
        setFocusPolicy(Qt.FocusPolicy.StrongFocus);
        clearBoard();

        nextPiece.setRandomShape();
    }
//! [5]

    public void scoreChanged(int score)
    {
        scoreChanged.emit(score);
    }

    public void levelChanged(int level)
    {
        levelChanged.emit(level);
    }

    public void linesRemovedChanged(int numLines)
    {
        linesRemovedChanged.emit(numLines);
    }

//! [6]
    public void setNextPieceLabel(QLabel label)
    {
        nextPieceLabel = label;
    }
//! [6]

    TetrixShape shapeAt(int x, int y)
    {
        return board[x + y * BoardWidth];
    }

    void setShapeAt(int x, int y, TetrixShape shape)
    {
        board[x + y * BoardWidth] = shape;
    }

    int timeoutTime()
    {
        return 1000 / (1 + level);
    }

    int squareWidth() { return contentsRect().width() / BoardWidth; }

    int squareHeight() { return contentsRect().height() / BoardHeight; }

    @Override
//! [7]
    public QSize sizeHint()
    {
        return new QSize(BoardWidth * 15 + frameWidth() * 2, BoardHeight * 15 + frameWidth() * 2);
    }

    @Override
    public QSize minimumSizeHint()
//! [7] //! [8]
    {
        return new QSize(BoardWidth * 5 + frameWidth() * 2, BoardHeight * 5 + frameWidth() * 2);
    }
//! [8]

//! [9]
    public void start()
    {
        if (isPaused)
            return ;


        isStarted = true;
        isWaitingAfterLine = false;
        numLinesRemoved = 0;
        numPiecesDropped = 0;
        score = 0;
        level = 1;
        clearBoard();

        linesRemovedChanged(numLinesRemoved);
        scoreChanged(score);
        levelChanged(level);

        newPiece();

        timer.start(timeoutTime(), this);
    }
//! [9]

//! [10]
    public void pause()
    {
        if (!isStarted)
            return ;

        isPaused = !isPaused;
        if (isPaused) {
            timer.stop();
        } else {
            timer.start(timeoutTime(), this);
        }

        update();
//! [10] //! [11]
    }
//! [11]

    @Override
//! [12]
    protected void paintEvent(QPaintEvent e)
    {
        super.paintEvent(e);

        QPainter painter = new QPainter();
        painter.begin(this);
        QRect rect = contentsRect();
//! [12]

        if (isPaused) {
            painter.drawText(rect, Qt.AlignmentFlag.AlignCenter.value(), "Pause");
            painter.end();
            return ;
        }

//! [13]
        int boardTop = rect.bottom() - BoardHeight * squareHeight();

        for (int i=0; i<BoardHeight; ++i) {
            for (int j=0; j<BoardWidth; ++j) {
                TetrixShape shape = shapeAt(j, BoardHeight - i - 1);
                if (shape != TetrixShape.NoShape) {
                    drawSquare(painter, rect.left() + j * squareWidth(),
                        boardTop + i * squareHeight(), shape);
                }
//! [13] //! [14]
            }
//! [14]
        }

//! [15]
        if (curPiece.shape() != TetrixShape.NoShape) {
            for (int i=0; i<4; ++i) {
                int x = curX + curPiece.x(i);
                int y = curY - curPiece.y(i);

                drawSquare(painter, rect.left() + x * squareWidth(),
                    boardTop + (BoardHeight - y - 1) * squareHeight(),
                    curPiece.shape());
            }
//! [15] //! [16]
        }
//! [16] //! [17]

        painter.end();
    }
//! [17]

    @Override
//! [18]
    protected void keyPressEvent(QKeyEvent event)
    {
        if (!isStarted || isPaused || curPiece.shape() == TetrixShape.NoShape) {
            super.keyPressEvent(event);
            return ;
        }
//! [18]

//! [19]
        if (event.key() == Qt.Key.Key_Left.value())
            tryMove(curPiece, curX - 1, curY);
        else if (event.key() == Qt.Key.Key_Right.value())
            tryMove(curPiece, curX + 1, curY);
        else if (event.key() == Qt.Key.Key_Down.value())
            tryMove(curPiece.rotatedRight(), curX, curY);
        else if (event.key() == Qt.Key.Key_Up.value())
            tryMove(curPiece.rotatedLeft(), curX, curY);
        else if (event.key() == Qt.Key.Key_Space.value())
            dropDown();
        else if (event.key() == Qt.Key.Key_D.value())
            oneLineDown();
        else
            super.keyPressEvent(event);
    }
//! [19]

    @Override
//! [20]
    protected void timerEvent(QTimerEvent event)
    {
        if (event.timerId() == timer.timerId()) {
            if (isWaitingAfterLine) {
                isWaitingAfterLine = false;
                newPiece();
                timer.start(timeoutTime(), this);
            } else {
                oneLineDown();
            }
        } else {
            super.timerEvent(event);
//! [20] //! [21]
        }
//! [21] //! [22]
    }
//! [22]

//! [23]
    void clearBoard()
    {
        for (int i=0; i<BoardHeight * BoardWidth; ++i)
            board[i] = TetrixShape.NoShape;
    }
//! [23]

//! [24]
    void dropDown()
    {
        int dropHeight = 0;
        int newY = curY;
        while (newY > 0) {
            if (!tryMove(curPiece, curX, newY - 1))
                break ;
            --newY;
            ++dropHeight;
        }
        pieceDropped(dropHeight);
//! [24] //! [25]
    }
//! [25]

//! [26]
    void oneLineDown()
    {
        if (!tryMove(new TetrixPiece(curPiece), curX, curY - 1))
            pieceDropped(0);
    }
//! [26]

//! [27]
    void pieceDropped(int dropHeight)
    {
        for (int i=0; i<4; ++i) {
            int x = curX + curPiece.x(i);
            int y = curY - curPiece.y(i);
            setShapeAt(x, y, curPiece.shape());
        }

        ++numPiecesDropped;
        if (numPiecesDropped % 25 == 0) {
            ++level;
            timer.start(timeoutTime(), this);
            levelChanged(level);
        }

        score += dropHeight + 7;
        scoreChanged(score);

        removeFullLines();

        if (!isWaitingAfterLine)
            newPiece();
//! [27] //! [28]
    }
//! [28]

//! [29]
    void removeFullLines()
    {
        int numFullLines = 0;

        for (int i=BoardHeight - 1; i >= 0; --i) {
            boolean lineIsFull = true;

            for (int j=0; j<BoardWidth; ++j) {
                if (shapeAt(j, i) == TetrixShape.NoShape) {
                    lineIsFull = false;
                    break ;
                }
            }

            if (lineIsFull) {
//! [29] //! [30]
                ++numFullLines;
                for (int k=i; k<BoardHeight - 1; ++k) {
                    for (int j=0; j<BoardWidth; ++j)
                        setShapeAt(j, k, shapeAt(j, k + 1));
                }
//! [30] //! [31]
                for (int j=0; j<BoardWidth; ++j)
                    setShapeAt(j, BoardHeight - 1, TetrixShape.NoShape);
            }
//! [31] //! [32]
        }
//! [32]

//! [33]
        if (numFullLines > 0) {
            numLinesRemoved += numFullLines;
            score += 10 * numFullLines;
            linesRemovedChanged(numLinesRemoved);
            scoreChanged(score);

            timer.start(500, this);
            isWaitingAfterLine = true;
            curPiece.setShape(TetrixShape.NoShape);
            update();
        }
//! [33] //! [34]
    }
//! [34]

//! [35]
    void newPiece()
    {
        curPiece = new TetrixPiece(nextPiece);

        nextPiece.setRandomShape();
        showNextPiece();
        curX = BoardWidth / 2 + 1;
        curY = BoardHeight - 1 + curPiece.minY();

        if (!tryMove(curPiece, curX, curY)) {
            curPiece.setShape(TetrixShape.NoShape);
            timer.stop();
            isStarted = false;
        }
//! [35] //! [36]
    }
//! [36]

//! [37]
    void showNextPiece()
    {
        if (nextPieceLabel == null)
            return ;

        int dx = nextPiece.maxX() - nextPiece.minX() + 1;
        int dy = nextPiece.maxY() - nextPiece.minY() + 1;

        QPixmap pixmap = new QPixmap(dx * squareWidth(), dy * squareHeight());
        QPainter painter = new QPainter();
        painter.begin(pixmap);
        painter.fillRect(pixmap.rect(), nextPieceLabel.palette().window());

        for (int i=0; i<4; ++i) {
            int x = nextPiece.x(i) - nextPiece.minX();
            int y = nextPiece.y(i) - nextPiece.minY();
            drawSquare(painter, x * squareWidth(), y * squareHeight(), nextPiece.shape());
        }
        painter.end();

        nextPieceLabel.setPixmap(pixmap);
//! [37] //! [38]
    }
//! [38]

//! [39]
    boolean tryMove(TetrixPiece newPiece, int newX, int newY)
    {
        for (int i = 0; i < 4; ++i) {
            int x = newX + newPiece.x(i);
            int y = newY - newPiece.y(i);
            if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
                return false;
            if (shapeAt(x, y) != TetrixShape.NoShape)
                return false;
        }
//! [39]

//! [40]
        curPiece = new TetrixPiece(newPiece);
        curX = newX;
        curY = newY;
        update();
        return true;
    }
//! [40]

//! [41]
    void drawSquare(QPainter painter, int x, int y, TetrixShape shape)
    {
        QColor color = new QColor(redTable[shape.ordinal()], greenTable[shape.ordinal()], blueTable[shape.ordinal()]);
        painter.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2,
            new QBrush(color));

        painter.setPen(color.lighter());
        painter.drawLine(x, y + squareHeight() - 1, x, y);
        painter.drawLine(x, y, x + squareWidth() - 1, y);

        painter.setPen(color.darker());
        painter.drawLine(x + 1, y + squareHeight() - 1, x + squareWidth() - 1, y + squareHeight() - 1);
        painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1, x + squareWidth() - 1, y + 1);
    }
//! [41] //! [42]
}
//! [42]


//! [43]
class TetrixPiece
{
    static final int coordsTable[][][] =
    { { { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },  { 0, 0 },   { -1, 0 },  { -1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 1, 0 },   { 1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { 0, 2 } },
        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 0, 1 } },
        { { 0, 0 },   { 1, 0 },   { 0, 1 },   { 1, 1 } },
        { { -1, -1 }, { 0, -1 },  { 0, 0 },   { 0, 1 } },
        { { 1, -1 },  { 0, -1 },  { 0, 0 },   { 0, 1 } } };
//! [43]

//! [44]
    private TetrixBoard.TetrixShape pieceShape;
    private int coords[][] = new int[4][2];
//! [44]

//! [45]
    public TetrixPiece()
    {
        setShape(TetrixBoard.TetrixShape.NoShape);
    }
//! [45]

//! [46]
    public TetrixPiece(TetrixPiece copy)
    {
        pieceShape = copy.shape();
        for (int i=0; i<4; ++i) {
            setX(i, copy.x(i));
            setY(i, copy.y(i));
        }
//! [46] //! [47]
    }
//! [47]

//! [48]
    public void setRandomShape()
    {
        Random rand = new Random();
        int shapeint = rand.nextInt(7) + 1;

        TetrixBoard.TetrixShape shape = TetrixBoard.TetrixShape.NoShape;
        switch (shapeint) {
        case 1: shape = TetrixBoard.TetrixShape.ZShape; break ;
        case 2: shape = TetrixBoard.TetrixShape.SShape; break ;
        case 3: shape = TetrixBoard.TetrixShape.LineShape; break ;
        case 4: shape = TetrixBoard.TetrixShape.TShape; break ;
        case 5: shape = TetrixBoard.TetrixShape.SquareShape; break ;
        case 6: shape = TetrixBoard.TetrixShape.LShape; break ;
        case 7: shape = TetrixBoard.TetrixShape.MirroredLShape; break ;
        }

        setShape(shape);
//! [48] //! [49]
    }
//! [49]

//! [50]
    public TetrixBoard.TetrixShape shape()
    {
        return pieceShape;
    }
//! [50]

//! [51]
    public void setShape(TetrixBoard.TetrixShape shape)
    {
        for (int i=0; i<4; ++i) {
            for (int j=0; j<2; ++j)
                coords[i][j] = coordsTable[shape.ordinal()][i][j];
        }

        pieceShape = shape;
//! [51] //! [52]
    }
//! [52]

//! [53]
    public int minX()
    {
        int min = coords[0][0];
        for (int i=1; i<4; ++i)
            min = min < coords[i][0] ? min : coords[i][0];
        return min;
    }

    public int maxX()
//! [53] //! [54]
    {
        int max = coords[0][0];
        for (int i=1; i<4; ++i)
            max = max > coords[i][0] ? max : coords[i][0];
        return max;
    }
//! [54]

//! [55]
    public int minY()
    {
        int min = coords[0][1];
        for (int i=1; i<4; ++i)
            min = min < coords[i][1] ? min : coords[i][1];
        return min;
    }

    public int maxY()
//! [55] //! [56]
    {
        int max = coords[0][1];
        for (int i=1; i<4; ++i)
            max = max > coords[i][1] ? max : coords[i][1];
        return max;
    }
//! [56]

//! [57]
    public TetrixPiece rotatedLeft()
    {
        if (pieceShape == TetrixBoard.TetrixShape.SquareShape)
            return this;

        TetrixPiece result = new TetrixPiece();
        result.pieceShape = pieceShape;
        for (int i=0; i<4; ++i) {
            result.setX(i, y(i));
            result.setY(i, -x(i));
        }

        return result;
//! [57] //! [58]
    }
//! [58]

//! [59]
    public TetrixPiece rotatedRight()
    {
        if (pieceShape == TetrixBoard.TetrixShape.SquareShape)
            return this;

        TetrixPiece result = new TetrixPiece();
        result.pieceShape = pieceShape;
        for (int i=0; i<4; ++i) {
            result.setX(i, -y(i));
            result.setY(i, x(i));
        }

        return result;
//! [59] //! [60]
    }
//! [60]

//! [61]
    public int x(int index)
    {
        return coords[index][0];
    }

    public int y(int index)
//! [61] //! [62]
    {
        return coords[index][1];
    }
//! [62]

//! [63]
    private void setX(int index, int x)
    {
        coords[index][0] = x;
    }

    private void setY(int index, int y)
//! [63] //! [64]
    {
        coords[index][1] = y;
    }
//! [64] //! [65]
}
//! [65]

@QtJambiExample(name = "Tetrix")
public class Tetrix extends QWidget
{
//! [66]
    public Tetrix() {
        this(null);
    }

    public Tetrix(QWidget parent)
    {
        super(parent);

        board = new TetrixBoard(null);
//! [66]

        nextPieceLabel = new QLabel();
        nextPieceLabel.setFrameStyle(QFrame.Shape.Box.value() | QFrame.Shadow.Raised.value());
        nextPieceLabel.setAlignment(new Qt.Alignment(Qt.AlignmentFlag.AlignCenter));
        board.setNextPieceLabel(nextPieceLabel);

//! [67]
        scoreLcd = new QLCDNumber(5);
        scoreLcd.setSegmentStyle(QLCDNumber.SegmentStyle.Filled);
//! [67]

        levelLcd = new QLCDNumber(2);
        levelLcd.setSegmentStyle(QLCDNumber.SegmentStyle.Filled);

        linesLcd = new QLCDNumber(5);
        linesLcd.setSegmentStyle(QLCDNumber.SegmentStyle.Filled);

//! [68]
        startButton = new QPushButton("&Start");
        startButton.setFocusPolicy(Qt.FocusPolicy.NoFocus);
        quitButton = new QPushButton("&Quit");
        quitButton.setFocusPolicy(Qt.FocusPolicy.NoFocus);
        pauseButton = new QPushButton("&Pause");
//! [68] //! [69]
        pauseButton.setFocusPolicy(Qt.FocusPolicy.NoFocus);
//! [69] //! [70]

        startButton.clicked.connect(board, "start()");
//! [70] //! [71]
        quitButton.clicked.connect(this, "close()");
        pauseButton.clicked.connect(board, "pause()");
        board.scoreChanged.connect(scoreLcd, "display(int)");
        board.levelChanged.connect(levelLcd, "display(int)");
        board.linesRemovedChanged.connect(linesLcd, "display(int)");
//! [71]

//! [72]
        layout = new QGridLayout();
        layout.addWidget(createLabel("NEXT"), 0, 0);
        layout.addWidget(nextPieceLabel, 1, 0);
        layout.addWidget(createLabel("LEVEL"), 2, 0);
        layout.addWidget(levelLcd, 3, 0);
        layout.addWidget(startButton, 4, 0);
        layout.addWidget(board, 0, 1, 6, 1);
        layout.addWidget(createLabel("SCORE"), 0, 2);
        layout.addWidget(scoreLcd, 1, 2);
        layout.addWidget(createLabel("LINES REMOVED"), 2, 2);
        layout.addWidget(linesLcd, 3, 2);
        layout.addWidget(quitButton, 4, 2);
        layout.addWidget(pauseButton, 5, 2);

        setLayout(layout);

        setWindowTitle("Tetrix");
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        resize(550, 370);
    }
//! [72]

//! [73]
    private QLabel createLabel(String text)
    {
        QLabel lbl = new QLabel(text);
        lbl.setAlignment(new Qt.Alignment(Qt.AlignmentFlag.AlignBottom));
        return lbl;
    }
//! [73]

//! [74]
    private TetrixBoard board = null;
    private QPushButton startButton = null;
    private QPushButton quitButton = null;
    private QPushButton pauseButton = null;
    private QLabel nextPieceLabel = null;
    private QLCDNumber scoreLcd = null;
    private QLCDNumber levelLcd = null;
    private QLCDNumber linesLcd = null;
    private QGridLayout layout = null;
//! [74]


//! [75]
    public static void main(String args[])
    {
        QApplication.initialize(args);

        Tetrix window = new Tetrix();
        window.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [75] //! [76]
}
//! [76]
