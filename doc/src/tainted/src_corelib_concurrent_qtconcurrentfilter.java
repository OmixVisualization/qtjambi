/*   Ported from: src.corelib.concurrent.qtconcurrentfilter.cpp
<snip>
//! [0]
    bool function(const T &t);
//! [0]


//! [1]
    bool allLowerCase(const QString &string)
    {
        return string.lowered() == string;
    }

    QStringList strings = ...;
    QFuture<QString> lowerCaseStrings = QtConcurrent::filtered(strings, allLowerCase);
//! [1]


//! [2]
    QStringList strings = ...;
    QFuture<void> future = QtConcurrent::filter(strings, allLowerCase);
//! [2]


//! [3]
    V function(T &result, const U &intermediate)
//! [3]


//! [4]
    void addToDictionary(QSet<QString> &dictionary, const QString &string)
    {
        dictionary.insert(string);
    }

    QStringList strings = ...;
    QFuture<QSet<QString> > dictionary = QtConcurrent::filteredReduced(strings, allLowerCase, addToDictionary);
//! [4]


//! [5]
    QStringList strings = ...;
    QFuture<QString> lowerCaseStrings = QtConcurrent::filtered(strings.constBegin(), strings.constEnd(), allLowerCase);

    // filter in-place only works on non-const iterators
    QFuture<void> future = QtConcurrent::filter(strings.begin(), strings.end(), allLowerCase);

    QFuture<QSet<QString> > dictionary = QtConcurrent::filteredReduced(strings.constBegin(), strings.constEnd(), allLowerCase, addToDictionary);
//! [5]


//! [6]
    QStringList strings = ...;

    // each call blocks until the entire operation is finished
    QStringList lowerCaseStrings = QtConcurrent::blockingFiltered(strings, allLowerCase);


    QtConcurrent::blockingFilter(strings, allLowerCase);

    QSet<QString> dictionary = QtConcurrent::blockingFilteredReduced(strings, allLowerCase, addToDictionary);
//! [6]


//! [7]
    // keep only images with an alpha channel
    QList<QImage> images = ...;
    QFuture<void> = QtConcurrent::filter(strings, &QImage::hasAlphaChannel());

    // keep only gray scale images
    QList<QImage> images = ...;
    QFuture<QImage> bgrImages = QtConcurrent::filtered(images, &QImage::isGrayscale());

    // create a set of all printable characters
    QList<QChar> characters = ...;
    QFuture<QSet<QChar> > set = QtConcurrent::filteredReduced(characters, &QChar::isPrint() , &QSet<QChar>::insert);
//! [7]


//! [8]
    // can mix normal functions and member functions with QtConcurrent::filteredReduced()

    // create a dictionary of all lower cased strings
    extern bool allLowerCase(const QString &string);
    QStringList strings = ...;
    QFuture<QSet<int> > averageWordLength = QtConcurrent::filteredReduced(strings, allLowerCase, QSet<QString>::insert);

    // create a collage of all gray scale images
    extern void addToCollage(QImage &collage, const QImage &grayscaleImage);
    QList<QImage> images = ...;
    QFuture<QImage> collage = QtConcurrent::filteredReduced(images, &QImage::isGrayscale, addToCollage);
//! [8]


//! [9]
    bool QString::contains(const QRegExp &regexp) const;
//! [9]


//! [10]
    boost::bind(&QString::contains, QRegExp("^\\S+$")); // matches strings without whitespace
//! [10]


//! [11]
    bool contains(const QString &string)
//! [11]


//! [12]
    QStringList strings = ...;
    QFuture<QString> strings = QtConcurrent::filtered(strings, boost::bind(&QString::contains, QRegExp("^\\S+$")));
//! [12]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_concurrent_qtconcurrentfilter {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
    booleansfunction(T );
//! [0]


//! [1]
    booleansallLowerCase(Stringstring)
    {
        return string.lowered() == string;
    }

    List<String> strings = ...;
    QFuture<QString> lowerCaseStrings = QtConcurrent.filtered(strings, allLowerCase);
//! [1]


//! [2]
    List<String> strings = ...;
    QFuture<void> future = QtConcurrent.filter(strings, allLowerCase);
//! [2]


//! [3]
    V function(T esult, U ntermediate)
//! [3]


//! [4]
    void addToDictionary(QSet<QString> ictionary, Stringstring)
    {
        dictionary.insert(string);
    }

    List<String> strings = ...;
    QFuture<QSet<QString> > dictionary = QtConcurrent.filteredReduced(strings, allLowerCase, addToDictionary);
//! [4]


//! [5]
    List<String> strings = ...;
    QFuture<QString> lowerCaseStrings = QtConcurrent.filtered(strings.constBegin(), strings.constEnd(), allLowerCase);

    // filter in-place only works on non-iterators
    QFuture<void> future = QtConcurrent.filter(strings.begin(), strings.end(), allLowerCase);

    QFuture<QSet<QString> > dictionary = QtConcurrent.filteredReduced(strings.constBegin(), strings.constEnd(), allLowerCase, addToDictionary);
//! [5]


//! [6]
    List<String> strings = ...;

    // each call blocks until the entire operation is finished
    List<String> lowerCaseStrings = QtConcurrent.blockingFiltered(strings, allLowerCase);


    QtConcurrent.blockingFilter(strings, allLowerCase);

    QSet<QString> dictionary = QtConcurrent.blockingFilteredReduced(strings, allLowerCase, addToDictionary);
//! [6]


//! [7]
    // keep only images with an alpha channel
    QList<QImage> images = ...;
    QFuture<void> = QtConcurrent.filter(strings, Image.hasAlphaChannel());

    // keep only gray scale images
    QList<QImage> images = ...;
    QFuture<QImage> bgrImages = QtConcurrent.filtered(images, Image.isGrayscale());

    // create a set of all printable characters
    QList<QChar> characters = ...;
    QFuture<QSet<QChar> > set = QtConcurrent.filteredReduced(characters, Char.isPrint() , Set<QChar>.insert);
//! [7]


//! [8]
    // can mix normal functions and member functions with QtConcurrent.filteredReduced()

    // create a dictionary of all lower cased strings
    extern booleansallLowerCase(Stringstring);
    List<String> strings = ...;
    QFuture<QSet<int> > averageWordLength = QtConcurrent.filteredReduced(strings, allLowerCase, QSet<QString>.insert);

    // create a collage of all gray scale images
    extern void addToCollage(QImage ollage, QImage rayscaleImage);
    QList<QImage> images = ...;
    QFuture<QImage> collage = QtConcurrent.filteredReduced(images, Image.isGrayscale, addToCollage);
//! [8]


//! [9]
    booleansQString.contains(QRegExp egexp);
//! [9]


//! [10]
    boost.bind(String.contains, QRegExp("^\\S+$")); // matches strings without whitespace
//! [10]


//! [11]
    booleanscontains(Stringstring)
//! [11]


//! [12]
    List<String> strings = ...;
    QFuture<QString> strings = QtConcurrent.filtered(strings, boost.bind(String.contains, QRegExp("^\\S+$")));
//! [12]


    }
}
