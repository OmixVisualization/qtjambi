import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


abstract class MyLayout extends QLayoutItem {

    int cached_width = 1;
    int cached_hfw = 1;
    boolean cache_dirty = true;

    private int calculateHeightForWidth(int w) { return 1; }

//! [0]
    public int heightForWidth(int w)
    {
        if (cache_dirty || cached_width != w) {
            int h = calculateHeightForWidth(w);
            cached_hfw = h;
            return h;
        }
        return cached_hfw;
    }
//! [0]
}
