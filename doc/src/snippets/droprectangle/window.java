#include <QtGui>

#include "window.h"

Window.Window(QWidget parent)
    : QWidget(parent)
{
    QLabel textLabel = new QLabel(tr("Data:"), this);
    textBrowser = new QTextBrowser(this);

    QLabel mimeTypeLabel = new QLabel(tr("MIME types:"), this);
    mimeTypeCombo = new QComboBox(this);

    dropFrame = new QFrame(this);
    dropFrame.setFrameStyle(QFrame.StyledPanel | QFrame.Sunken);
    QLabel dropLabel = new QLabel(tr("Drop items here"), dropFrame);
    dropLabel.setAlignment(Qt.AlignHCenter);

    QVBoxLayout dropFrameLayout = new QVBoxLayout(dropFrame);
    dropFrameLayout.addWidget(dropLabel);

    QHBoxLayout dropLayout = new QHBoxLayout;
    dropLayout.addStretch(0);
    dropLayout.addWidget(dropFrame);
    dropLayout.addStretch(0);

    QVBoxLayout mainLayout = new QVBoxLayout(this);
    mainLayout.addWidget(textLabel);
    mainLayout.addWidget(textBrowser);
    mainLayout.addWidget(mimeTypeLabel);
    mainLayout.addWidget(mimeTypeCombo);
    mainLayout.addSpacing(32);
    mainLayout.addLayout(dropLayout);

    setAcceptDrops(true);
    setWindowTitle(tr("Drop Rectangle"));
}

//! [0]
    protected void dragMoveEvent(QDragMoveEvent event)
    {
        if (event.mimeData().hasFormat("text/plain") &&
             event.answerRect().intersects(dropFrame.geometry()))

            event.acceptProposedAction();
    }
//! [0]

void Window.dropEvent(QDropEvent event)
{
    textBrowser.setPlainText(event.mimeData().text());
    mimeTypeCombo.clear();
    mimeTypeCombo.addItems(event.mimeData().formats());

    event.acceptProposedAction();
}
