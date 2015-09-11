import org.qtjambi.qt.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

import java.util.*;

class main
{
    public static void main(String args[])
    {
        QApplication.initialize(args);
        MyStylePlugin plugin = new MyStylePlugin();
        QApplication.execStatic();
        QApplication.shutdown();
    }
}

class RocketStyle extends QCommonStyle
{

}

class StarBusterStyle extends QCommonStyle
{

}

class MyStylePlugin extends QStylePlugin
{

    //! [0]
    List<String> keys()
    {
        List<String> keyList = new Vector<String>();
        keyList.add("Rocket");
        keyList.add("StarBuster");

        return keyList;
    }
    //! [0]

    //! [1]
    QStyle create(String key)
    {
        String lcKey = key;

        if (lcKey.equals("rocket")) {
            return new RocketStyle();
        } else if (lcKey.equals("starbuster")) {
            return new StarBusterStyle();
        }
        return null;
    //! [1] //! [2]
    }
    //! [2]
}
