import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class buttonwidget extends QWidget
{
    QSignalMapper signalMapper;

//! [0]
    public buttonwidget(List<String> texts)
    {
        signalMapper = new QSignalMapper(this);

        QGridLayout gridLayout = new QGridLayout();
        for (int i = 0; i < texts.size(); ++i) {
            QPushButton button = new QPushButton(texts.get(i));
            button.clicked.connect(signalMapper, "map()");
            //! [0] //! [1]
            signalMapper.setMapping(button, texts.get(i));
            gridLayout.addWidget(button, i / 3, i % 3);
        }

        signalMapper.mappedString.connect(
    //! [1] //! [2]
                this, "clicked(String)");

        setLayout(gridLayout);
    }
    //! [2]
}
