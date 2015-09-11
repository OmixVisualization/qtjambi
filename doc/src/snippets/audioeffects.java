
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.phonon.*;

import java.util.*;


public class audioeffects
{

public static void  main(String args[])
{
    QApplication.initialize(args);
    QApplication.setApplicationName("Audio effect tester");

    MediaObject mediaObject = new MediaObject();
    mediaObject.setCurrentSource(new MediaSource("/home/gvatteka/Music/Lumme-Badloop.ogg"));

    AudioOutput audioOutput =
        new AudioOutput(Phonon.Category.MusicCategory);

//! [0]
    List<EffectDescription> effectDescriptions =
            BackendCapabilities.availableAudioEffects();
    EffectDescription effectDescription = effectDescriptions.get(4);
    
    Path path = Phonon.createPath(mediaObject, audioOutput);

//! [1]
    Effect effect = new Effect(effectDescription);
    path.insertEffect(effect);
//! [0]

    EffectWidget effectWidget = new EffectWidget(effect);
    effectWidget.show();
//! [1]

    mediaObject.play();
    
    effectWidget.setWindowTitle("Effect Name: " + effectDescription.name());

    QApplication.execStatic();
    QApplication.shutdown();
}

}
