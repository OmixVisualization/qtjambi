/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package test;

import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.gui.QMessageBox;

/**
 *
 * @author admin
 */
public class Main {

    public static void main(String[] args) {

        QApplication.initialize(args);

        HelloDialog d = new HelloDialog();
        d.show();

        QApplication.execStatic();
        QApplication.shutdown();


    }

}
