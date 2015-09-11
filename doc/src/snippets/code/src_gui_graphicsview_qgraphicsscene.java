import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_graphicsview_qgraphicsscene extends QGraphicsScene {
    public static void main(String args[]) {
        QApplication.initialize(args);
        }

    public void first() {
//! [0]
        QGraphicsScene scene = new QGraphicsScene();
        scene.addText("Hello, world!");

        QGraphicsView view = new QGraphicsView(scene);
        view.show();
//! [0]
    }

    public void second() {

//! [1]
        QGraphicsScene scene = new QGraphicsScene();
        scene.addRect(0d,0d,50d,50d);
        QPrinter printer = new QPrinter(QPrinter.PrinterMode.HighResolution);
        printer.setPageSize(QPrinter.PageSize.A4);

        QPainter painter = new QPainter(printer);
        scene.render(painter);
//! [1]

        int depth = 0;

//! [2]
        QSizeF segmentSize = sceneRect().size().divide(java.lang.Math.pow(2, depth - 1));
//! [2]
    }
    public void third() {

//! [3]
        QGraphicsScene scene = new QGraphicsScene();
        QGraphicsView view = new QGraphicsView(scene);
        view.show();

        // a blue background
        scene.setBackgroundBrush(new QBrush(QColor.blue));

        // a gradient background
        QRadialGradient gradient = new QRadialGradient(0, 0, 10);
        gradient.setSpread(QGradient.Spread.RepeatSpread);
        scene.setBackgroundBrush(new QBrush(gradient));
//! [3]
    }

    public void fourth() {

//! [4]
        QGraphicsScene scene = new QGraphicsScene();
        QGraphicsView view = new QGraphicsView(scene);
        view.show();

        // a white semi-transparent foreground
        scene.setForegroundBrush(new QBrush(new QColor(255, 255, 255, 127)));

        // a grid foreground
        scene.setForegroundBrush(new QBrush(QColor.lightGray, Qt.BrushStyle.CrossPattern));
//! [4]
    }

//! [5]
    class TileScene extends QGraphicsScene {

    private final int tileWidth = 10;
    private final int tileHeight = 10;
    private final int numTilesH = 10;
    private final int numTilesV = 10;
    private QPixmap[][] tiles;

    TileScene() {
        tiles = new QPixmap[numTilesH][numTilesV];
    }

      public QRectF rectForTile(int x, int y)
      {
          // Return the rectangle for the tile at position (x, y).
          return new QRectF(x * tileWidth,y * tileHeight,tileWidth,tileHeight);
      }

      public void setTile(int x, int y, QPixmap pixmap)
      {
          // Sets or replaces the tile at position (x, y) with pixmap.
          if (x >= 0 && x < numTilesH && y >= 0 && y < numTilesV) {
              tiles[y][x] = pixmap;
              invalidate(rectForTile(x, y), QGraphicsScene.SceneLayer.BackgroundLayer);
          }
      }

      protected void drawBackground(QPainter painter, QRectF exposed)
      {
          // Draws all tiles that intersect the exposed area.
          for (int y = 0; y < numTilesV; ++y) {
              for (int x = 0; x < numTilesH; ++x) {
                  QRectF rect = rectForTile(x, y);
                  if (exposed.intersects(rect))
                      painter.drawPixmap(rect.topLeft(), tiles[y][x]);
              }
          }
      }

    }

//! [5]
}
