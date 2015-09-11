
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.phonon.*;

import java.util.*;

public class seekslider
{

public static void main(String args[])
{
    QApplication.initialize(args);

//![0]
    MediaObject moo = new MediaObject();
    AudioOutput device = new AudioOutput();
    Phonon.createPath(moo, device);
    moo.setCurrentSource(new MediaSource("/home/gvatteka/Music/Lumme-Badloop.ogg"));

    SeekSlider slider = new SeekSlider();
    slider.setMediaObject(moo);
    
    slider.show();
    moo.play();
//![0]

    QApplication.execStatic();
    QApplication.shutdown();
}

}
