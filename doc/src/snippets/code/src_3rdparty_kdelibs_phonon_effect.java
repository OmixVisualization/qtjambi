import org.qtjambi.qt.core.*;
import org.qtjambi.qt.phonon.*;


public class src_3rdparty_kdelibs_phonon_effect extends QObject {
    public void fooBar() {
        MediaNodeInterface source = null;
        MediaNodeInterface sink = null;
        EffectDescription description = null;

//! [0]
     Path path = Phonon.createPath(source, sink);
     Effect effect = new Effect(description, this);
     path.insertEffect(effect);
//! [0]


    }
}
