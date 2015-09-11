
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class ResourceSnippets extends QWidget
{
    
    public ResourceSnippets()
    {
    }

    @Override
    protected void paintEvent(QPaintEvent event)
    {
//![0]
        QFile resourceFile = new QFile("classpath:images/star.png");
//![0]

//![1]
        QPixmap pixmap = new QPixmap("classpath:images#/star.png");
//![1]

//![2]
        QFile absoluteFilePath = new QFile("classpath:images#/star.png");
        QFile relativeFilePath = new QFile("classpath:images/star.png");
//![2]

//![3]
        QDir dir = new QDir("classpath:images");
        List<QFileInfo> images = dir.entryInfoList();
//![3]

    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new ResourceSnippets().show();
    }
}

