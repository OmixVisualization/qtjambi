


public class main
{

//! [0]
    // This snippet is not relevant for Qt Jambi
    public static void main(String args[])
    {
        QApplication.initialize(args);

        if (args.length != 2) {
            System.err.println("Error - expected window id as argument");
            System.exit(1);
        }

        String windowId = app.arguments()[1];
        EmbedWidget window = new EmbedWidget();
        window.embedInto(windowId.toULong());
        window.show();

        QApplication.embedInto();
    }
//! [0]

}
