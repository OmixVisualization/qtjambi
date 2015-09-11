
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.phonon.*;

import java.util.*;

public class medianodesnippet
{

public static void main(String args[])
{
    QApplication.initialize(args);

//![0]
    MediaObject mediaObject = new MediaObject();
    AudioOutput audioOutput = new AudioOutput();
    VideoWidget videoWidget = new VideoWidget();

    Phonon.createPath(mediaObject, audioOutput);
    Phonon.createPath(mediaObject, videoWidget);

    List<Path> inputPaths =
        audioOutput.inputPaths();   // inputPaths = [ mediaObject ]
    List<Path> outputPaths =
        mediaObject.outputPaths(); // outputPaths = [ audioOutput, videoWidget ]
//![0]

    QApplication.execStatic();
    QApplication.shutdown();
}

}
