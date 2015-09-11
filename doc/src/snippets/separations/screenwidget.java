import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class screenwidget extends QFrame
{
    boolean inverted;
    QColor paintColor;
    QImage newImage;
    QImage originalImage;
    QLabel imageLabel;
    QLabel nameLabel;
    QPushButton colorButton;
    QPushButton invertButton;
    Separation maskColor;

    public ScreenWidget(QWidget parent, QColor initialColor,
                               String name, Separation mask,
                               QSize labelSize)
    {
        super(parent);

        paintColor = initialColor;
        maskColor = mask;
        inverted = false;

        imageLabel = new QLabel;
        imageLabel.setFrameShadow(QFrame.Sunken);
        imageLabel.setFrameShape(QFrame.StyledPanel);
        imageLabel.setMinimumSize(labelSize);

        nameLabel = new QLabel(name);
        colorButton = new QPushButton(tr("Modify..."));
        colorButton.setBackgroundRole(QPalette.Button);
        colorButton.setMinimumSize(32, 32);

        QPalette palette(colorButton.palette());
        palette.setColor(QPalette.Button, initialColor);
        colorButton.setPalette(palette);

        invertButton = new QPushButton(tr("Invert"));
        //invertButton.setToggleButton(true);
        //invertButton.setOn(inverted);
        invertButton.setEnabled(false);

        connect(colorButton, SIGNAL(clicked()), this, SLOT(setColor()));
        connect(invertButton, SIGNAL(clicked()), this, SLOT(invertImage()));

        QGridLayout gridLayout = new QGridLayout();
        gridLayout.addWidget(imageLabel, 0, 0, 1, 2);
        gridLayout.addWidget(nameLabel, 1, 0);
        gridLayout.addWidget(colorButton, 1, 1);
        gridLayout.addWidget(invertButton, 2, 1, 1, 1);
        setLayout(gridLayout);
    }

    public void createImage()
    {
        newImage = originalImage.copy();

        // Create CMY components for the ink being used.
        float cyanInk = (255 - paintColor.red())/255.0;
        float magentaInk = (255 - paintColor.green())/255.0;
        float yellowInk = (255 - paintColor.blue())/255.0;

        int (convert)(QRgb);

        switch (maskColor) {
            case Cyan:
                convert = qRed;
                break;
            case Magenta:
                convert = qGreen;
                break;
            case Yellow:
                convert = qBlue;
                break;
        }

        for (int y = 0; y < newImage.height(); ++y) {
            for (int x = 0; x < newImage.width(); ++x) {
                QRgb p(originalImage.pixel(x, y));

                // Separate the source pixel into its cyan component.
                int amount;

                if (inverted)
                    amount = convert(p);
                else
                    amount = 255 - convert(p);

                QColor newColor(
                    255 - qMin(int(amount  cyanInk), 255),
                    255 - qMin(int(amount  magentaInk), 255),
                    255 - qMin(int(amount  yellowInk), 255));

                newImage.setPixel(x, y, newColor.rgb());
            }
        }

        imageLabel.setPixmap(QPixmap.fromImage(newImage));
    }


    public QImage image()
    {
        return newImage;
    }


    public void invertImage()
    {
        //inverted = invertButton.isOn();
        inverted = !inverted;
        createImage();
        emit imageChanged();
    }


    public void setColor()
    {
        QColor newColor = QColorDialog.getColor(paintColor);

        if (newColor.isValid()) {
            paintColor = newColor;
            QPalette palette(colorButton.palette());
            palette.setColor(QPalette.Button, paintColor);
            colorButton.setPalette(palette);
            createImage();
            emit imageChanged();
        }
    }


    public void setImage(QImage image)
    {
        originalImage = image;
        createImage();
        invertButton.setEnabled(true);
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new ScreenWidget().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
