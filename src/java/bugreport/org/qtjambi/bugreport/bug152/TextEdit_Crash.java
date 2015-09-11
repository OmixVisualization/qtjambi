// mkdir build/bugreport
// javac -g -encoding utf8 -d build/bugreport -cp qtjambi-4.7.4.jar -source 1.5 -target 1.5  src/java/bugreport/org/qtjambi/bugreport/bug152/TextEdit_Crash.java
// jar -cvf $PWD/bug152.jar -C src/java/bugreport .
// jar -uvf $PWD/bug152.jar -C build/bugreport .
// java -Dorg.qtjambi.qt.debug=true -cp qtjambi-4.7.4.jar:build/qtjambi-linux64-gcc-4.7.4.jar:bug152.jar org/qtjambi/bugreport/bug152/TextEdit_Crash

// This bug was a crash when highlighting a long line of hebrew text on windows in Qy 4.7.1.
// I was not able to cause the crash on window with 4.7.4.

package org.qtjambi.bugreport.bug152;

import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.gui.QMainWindow;
import org.qtjambi.qt.gui.QTextEdit;

/**
 *
 * @author vienzlo
 */
public class TextEdit_Crash extends QMainWindow {

    QTextEdit edit;
    public TextEdit_Crash () {
        super();
        edit = new QTextEdit();
        edit.setText("פיתוח הגוף הוא ספורט פופולארי , בשנים האחרונות המגמה של פיתוח הגוף גוברת יותר ויותר. בדרך כלל אנו רגילים לקראות רק גברים המחטבים את הגוף ומנפחים את השרירים, אך בשנים האחרונות גם נשים החלו להשתתף בתחרויות של פיתוח הגוף והרבה ילדים ונערים. פיתוח הגוף הוא ספורט בריא. אימוני כוח ואימונים אירוביים גורמים לחיזוק השרירים, לחיזוק העצמות ולהפרשת הורמון הגדילה. פיתוח הגוף מסייע בחיזוק המערכת החיסונית וידוע גם כספורט העשוי לעכב תהליכי הזדקנות. פיתוח הגוף ואימוני הכושר גורמים לעלייה ברמות הטסטוסטרון שהוא הורמון חיוני לגברים אשר אחראי בין השאר על פוריות הגבר ובניית גוף שרירי וגברי. פיתוח הגוף צריך להיעשות במידה הנכונה , כאשר מפתחים את הגוף במידה הנכונה נגרמת לגוף תועלת רבה, אך כאשר המינון גבוה מדי ואינו נכון עלול להיגרם נזק.");
        
        setCentralWidget(edit);
        resize(300, 300);
        show();
    }
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        QApplication.initialize(args);
        TextEdit_Crash TextEditCrash = new TextEdit_Crash();
        QApplication.execStatic();
        QApplication.shutdown();
    }
}
