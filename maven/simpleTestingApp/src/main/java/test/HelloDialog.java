/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package test;

import org.qtjambi.qt.gui.QDialog;
import test.ui.Ui_HelloDialog;

/**
 *
 * @author admin
 */
public class HelloDialog extends QDialog {
    private final Ui_HelloDialog ui;

    public HelloDialog() {
        this.ui = new Ui_HelloDialog();
        ui.setupUi(this);
    }




}
