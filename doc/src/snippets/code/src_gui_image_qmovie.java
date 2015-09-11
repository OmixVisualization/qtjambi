import org.qtjambi.qt.gui.*;


public class src_gui_image_qmovie {
    public static void main(String args[]) {
        QApplication.initialize(args);
        {
//! [0]
        QLabel label = new QLabel();
        QMovie movie = new QMovie("animations/fire.gif");

        label.setMovie(movie);
        movie.start();
//! [0]
        }

        {
//! [1]
        QMovie movie = new QMovie("racecar.gif");
        movie.setSpeed(200); // 2x speed
//! [1]
        }


    }
}
