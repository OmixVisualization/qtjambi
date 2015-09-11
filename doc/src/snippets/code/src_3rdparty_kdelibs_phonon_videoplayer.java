import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.phonon.*;


public class src_3rdparty_kdelibs_phonon_videoplayer {
    public void fooBar(QWidget parentWidget) {
        QUrl url = new QUrl();
//! [0]

        VideoPlayer player = new VideoPlayer(Phonon.Category.VideoCategory, parentWidget);
        player.finished.connect(player, "disposeLater()");
        player.play(new MediaSource(url));
//! [0]


        VideoPlayer audioPlayer = player;
//! [1]

        audioPlayer.load(new MediaSource(url));
        audioPlayer.play();
//! [1]


    }
}
