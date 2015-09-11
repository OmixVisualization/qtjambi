import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;

import java.util.*;

public class main
{
    public static void main(String args[])
    {
        if (args.length 2) {
            System.out.println("Usage: java main <filename>");;
            System.exit(0);
        }

        QFile file = new QFile(args[1]);
    //! [0]
        QXmlSimpleReader xmlReader = new QXmlSimpleReader();
        QXmlInputSource source = new QXmlInputSource(file);
    //! [0]

    //! [1]
        Handler handler = new Handler();
        xmlReader.setContentHandler(handler);
        xmlReader.setErrorHandler(handler);
    //! [1]

    //! [2]
        boolean ok = xmlReader.parse(source);

        if (!ok)
            System.err.println("Parsing failed.");
    //! [2]
        else {
            List<String> names = handler.names();
            List<Integer> indentations = handler.indentations();

            int items = names.size();

            for (int i = 0; i < items; ++i) {
                for (int j = 0; j < indentations.at(i); ++j)
                    System.out.print(" ");
                System.out.println(names.at(i));
            }
        }
    }
}
