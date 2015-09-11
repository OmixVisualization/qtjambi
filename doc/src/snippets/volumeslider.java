
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import cin.trolltech.qt.gui.*;
import org.qtjambi.qt.phonon.*;

import java.util.*;

public class volumeslider
{

public static void main(String args[])
{
    QApplication.initialize(args);
    QApplication.setApplicationName("Volume slider tester");

    MediaObject mediaObject = new MediaObject();
    mediaObject.setCurrentSource(new MediaObject("/home/gvatteka/Music/Lumme-Badloop.ogg"));

//! [0]
    AudioOutput audioOutput = new AudioOutput(Phonon.Category.MusicCategory);
    Phonon.createPath(mediaObject, audioOutput);

    VolumeSlider volumeSlider = new VolumeSlider();
    volumeSlider.setAudioOutput(audioOutput);
//! [0]

    mediaObject.play();
    
    volumeSlider.show();

    QApplication.execStatic();
    QApplication.shutdown();
}

}

