import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class main
{
    public static void  main(String args[])
    {
        QApplication.initialize(args);

        MyPushButton button = new MyPushButton();
        button.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}


class MyPushButton extends QPushButton
{

    public MyPushButton(QWidget parent)
    {
        super(parent);
    }

//! [0]
    public void paintEvent(QPaintEvent event)
    {
        QStyleOptionButton optioni = new QStyleOptionButton();
        option.initFrom(this);
        option.state() = isDown() ? QStyle.State.State_Sunken :
                                    QStyle.State.State_Raised;
        if (isDefault())
            option.features().setFlag(QStyleOptionButton.ButtonFeature.DefaultButton);
        option.setText(text());
        option.setIcon(icon());

        QPainter painter = new QPainter(this);
        style().drawControl(QStyle.ControlElement.CE_PushButton, option, painter, this);
    }
//! [0]

}

class MyStyle extends QStyle
{

    //void drawPrimitive(PrimitiveElement element, QStyleOption option,
      //                 QPainter painter, QWidget widget);
//};

    public MyStyle()
    {
    //! [1]
        QStyleOptionFrame option = new QStyleOptionFrame();

        QStyleOptionFrame frameOption = null;
        if ((frameOption = (QStyleOptionFrame) option) != null) {
            QStyleOptionFrameV2 frameOptionV2 = new QStyleOptionFrameV2(frameOption);

            // draw the frame using frameOptionV2
        }
    //! [1]

    //! [2]
        QStyleOptionProgressBar progressBarOption = null;
        if ((progressBarOption = (QStyleOptionProgressBar) option) != null) {
            QStyleOptionProgressBarV2 progressBarV2 =
                new QStyleOptionProgressBarV2(progressBarOption);

            // draw the progress bar using progressBarV2
        }
    //! [2]

    //! [3]
        QStyleOptionTab tabOption = null;
        if ((tabOption = (QStyleOptionTab) option) != null) {
            QStyleOptionTabV2 tabV2 =
                    new QStyleOptionTabV2(tabOption);

            // draw the tab using tabV2
       }
    //! [3]
    }

    //! [4]
    void drawPrimitive(QStyle.PrimitiveElement element,
                       QStyleOption option,
                       QPainter painter,
                       QWidget widget)
    {
        if (element.equals(PrimitiveElement.PE_FrameFocusRect)) {
            QStyleOptionFocusRect focusRectOption =
                    (QStyleOptionFocusRect) option;
            if (focusRectOption != null) {
                // ...
            }
        }
        // ...
    }
    //! [4]
}
