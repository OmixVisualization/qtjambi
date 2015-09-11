
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.phonon.*;

import java.util.*;

public class phononeffectparameter
{

public static void main(String args[])
{
    QApplication.initialize(args);
    QApplication.setApplicationName("effectsnippets");

    List<EffectDescription> effects =
        BackendCapabilities.availableAudioEffects();

    Effect effect = new Effect(effects.get(3));
    
//![0]
    List<EffectParameter> parameters = effect.parameters();

    for (EffectParameter parameter : parameters) {
        // Do something with parameter
    }
//![0]

//![1]
    EffectWidget effectWidget = new EffectWidget(effect);
//![1]
    
    effectWidget.show();

    QApplication.execStatic();
    QApplication.shutdown();
}

}
