import org.qtjambi.qt.core.*;
import org.qtjambi.qt.phonon.*;

public class src_3rdparty_kdelibs_phonon_mediasource {
    public static void main(String args[]) {
//! [0]
        MediaObject m = new MediaObject();
        String fileName = "/home/foo/bar.ogg";
        QUrl url = new QUrl("http://www.example.com/stream.mp3");
        QBuffer someBuffer = new QBuffer();
        m.setCurrentSource(new MediaSource(fileName));
        m.setCurrentSource(new MediaSource(url));
        m.setCurrentSource(new MediaSource(someBuffer));
        m.setCurrentSource(new MediaSource(Phonon.DiscType.Cd));
 //! [0]
    }
}
