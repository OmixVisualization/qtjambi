import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class DragDropListModel extends QStringListModel
{

    DragDropListModel.DragDropListModel(List<String> strings,
                                         QObject parent)
        : List<String>Model(strings, parent)
    {
    }

    //! [0]
    boolean DragDropListModel.dropMimeData(QMimeData data,
        Qt.DropAction action, int row, int column, QModelIndex parent)
    {
        if (action == Qt.IgnoreAction)
            return true;

        if (!data.hasFormat("application/vnd.text.list"))
            return false;

        if (column > 0)
    //! [0] //! [1]
            return false;
    //! [1]

    //! [2]
        int beginRow;

        if (row != -1)
            beginRow = row;
    //! [2] //! [3]
        else if (parent.isValid())
            beginRow = parent.row();
    //! [3] //! [4]
        else
            beginRow = rowCount(QModelIndex());
    //! [4]

    //! [5]
        QByteArray encodedData = data.data("application/vnd.text.list");
        QDataStream stream(encodedData, QIODevice.ReadOnly);
        List<String> newItems;
        int rows = 0;

        while (!stream.atEnd()) {
            String text;
            stream >> text;
            newItems << text;
            ++rows;
        }
    //! [5]

    //! [6]
        insertRows(beginRow, rows, QModelIndex());
        for (String text, newItems) {
            QModelIndex idx = index(beginRow, 0, QModelIndex());
            setData(idx, text);
            beginRow++;
        }

        return true;
    }
    //! [6]

    //! [7]
    Qt.ItemFlags DragDropListModel.flags(QModelIndex index)
    {
        Qt.ItemFlags defaultFlags = List<String>Model.flags(index);

        if (index.isValid())
            return Qt.ItemIsDragEnabled | Qt.ItemIsDropEnabled | defaultFlags;
        else
            return Qt.ItemIsDropEnabled | defaultFlags;
    }
    //! [7]

    //! [8]
    QMimeData DragDropListModel.mimeData(QModelIndexList indexes)
    {
        QMimeData mimeData = new QMimeData();
        QByteArray encodedData;

        QDataStream stream(encodedData, QIODevice.WriteOnly);

        for (QModelIndex index, indexes) {
            if (index.isValid()) {
                String text = data(index, Qt.DisplayRole).toString();
                stream << text;
            }
        }

        mimeData.setData("application/vnd.text.list", encodedData);
        return mimeData;
    }
    //! [8]

    //! [9]
    List<String> DragDropListModel.mimeTypes()
    {
        List<String> types;
        types << "application/vnd.text.list";
        return types;
    }
    //! [9]

    //! [10]
    Qt.DropActions DragDropListModel.supportedDropActions()
    {
        return Qt.CopyAction | Qt.MoveAction;
    }
    //! [10]

}
