import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.phonon.*;
import java.util.*;


public class src_3rdparty_kdelibs_phonon_mediaobject extends QObject {
    private MediaObject media;
    public void fooBar() {
//! [0]

        media = new MediaObject(this);
        media.finished.connect(this, "slotFinished()");
        media.setCurrentSource(new MediaSource("/home/username/music/filename.ogg"));
        media.play();
//! [0]


//! [1]

        media.setCurrentSource(new MediaSource("classpath:/sounds/startsound.ogg"));
        media.enqueue(new MediaSource("/home/username/music/song.mp3"));
        media.enqueue(new MediaSource("classpath:/sounds/endsound.ogg"));
//! [1]


//! [2]

        media.setCurrentSource(new MediaSource("classpath:/sounds/startsound.ogg"));
        media.aboutToFinish.connect(this, "enqueueNextSource()");
    }

     public final void enqueueNextSource() {
         media.enqueue(new MediaSource("/home/username/music/song.mp3"));
     }
//! [2]


     public void fooBar2() {
         MediaObject producer = new MediaObject();

         {
//! [3]

         int x = 200;
         media.setTickInterval(x);
         assert(x == producer.tickInterval());
//! [3]
         }


         {
//! [4]

         int x = 200;
         media.setTickInterval(x);
         assert(x >= producer.tickInterval() &&
                x <= 2 * producer.tickInterval());
//! [4]
         }


//! [5]

         media.hasVideoChanged.connect(this, "hasVideoChanged(boolean)");
         media.setCurrentSource(new MediaSource("somevideo.avi"));
         media.hasVideo(); // returns false;
     }

     public final void hasVideoChanged(boolean b) {
         // b == true
         media.hasVideo(); // returns true;
     }
//! [5]
}

class Hack extends MediaObject {

     private MediaObject media;
     public void abc() {
//! [6]


         media.hasVideoChanged.connect(this, "hasVideoChanged(boolean)");
         media.setCurrentSource(new MediaSource("somevideo.avi"));
         media.hasVideo(); // returns false;
 }

     public final void hasVideoChanged(boolean b) {
         // b == true
         media.hasVideo(); // returns true;
     }
//! [6]

     public void setMetaArtist(List<String> arg) {}
     public void setMetaAlbum(List<String> arg) {}
     public void setMetaTitle(List<String> arg) {}
     public void setMetaDate(List<String> arg) {}
     public void setMetaGenre(List<String> arg) {}
     public void setMetaTrack(List<String> arg) {}
     public void setMetaComment(List<String> arg) {}

     public void fooBar() {
//! [7]

         setMetaArtist (media.metaData("ARTIST"     ));
         setMetaAlbum  (media.metaData("ALBUM"      ));
         setMetaTitle  (media.metaData("TITLE"      ));
         setMetaDate   (media.metaData("DATE"       ));
         setMetaGenre  (media.metaData("GENRE"      ));
         setMetaTrack  (media.metaData("TRACKNUMBER"));
         setMetaComment(media.metaData("DESCRIPTION"));
//! [7]

//! [8]

         QUrl url = new QUrl("http://www.example.com/music.ogg");
         media.setCurrentSource(new MediaSource(url));
//! [8]


         QProgressBar progressBar = new QProgressBar();
//! [9]

         progressBar.setRange(0, 100); // this is the default
         media.bufferStatus.connect(progressBar, "setValue(int)");
//! [9]


    }
}
