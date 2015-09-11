/*   Ported from: src.3rdparty.kdelibs.phonon.objectdescriptionmodel.h
<snip>
//! [0]

     * QComboBox *cb = new QComboBox(parentWidget);
     * ObjectDescriptionModel *model = new ObjectDescriptionModel(cb);
     * model->setModelData(BackendCapabilities::availableAudioOutputDevices());
     * cb->setModel(model);
     * cb->setCurrentIndex(0); // select first entry
     * //! [0]


//! [1]

     * int cbIndex = cb->currentIndex();
     * AudioOutputDevice selectedDevice = model->modelData(cbIndex);
     * //! [1]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_3rdparty_kdelibs_phonon_objectdescriptionmodel {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]

     * QComboBox b = new QComboBox(parentWidget);
     * ObjectDescriptionModel odel = new ObjectDescriptionModel(cb);
     * model.setModelData(BackendCapabilities.availableAudioOutputDevices());
     * cb.setModel(model);
     * cb.setCurrentIndex(0); // select first entry
     * //! [0]


//! [1]

     * int cbIndex = cb.currentIndex();
     * AudioOutputDevice selectedDevice = model.modelData(cbIndex);
     * //! [1]


    }
}
