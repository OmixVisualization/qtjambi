/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


package org.qtjambi.examples.phonon;

import java.util.List;
import java.util.Map;
import java.util.Vector;

import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.qt.core.QTime;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.widgets.QAbstractItemView;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QLCDNumber;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QSizePolicy;
import org.qtjambi.qt.widgets.QTableWidget;
import org.qtjambi.qt.widgets.QTableWidgetItem;
import org.qtjambi.qt.widgets.QToolBar;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Music Player")
public class MusicPlayer extends QMainWindow
{

//![0]
    public MusicPlayer()
    {
        audioOutput = new AudioOutput(Phonon.Category.MusicCategory);
        mediaObject = new MediaObject(this);
        metaInformationResolver = new MediaObject(this);

        Phonon.createPath(mediaObject, audioOutput);

        mediaObject.setTickInterval(1000);
//![0]

//![1]
        mediaObject.tick.connect(this, "tick(long)");
        mediaObject.stateChanged.connect(this, "stateChanged(Phonon$State,Phonon$State)");
        metaInformationResolver.stateChanged.
                connect(this, "metaStateChanged(Phonon$State,Phonon$State)");
        mediaObject.currentSourceChanged.connect(this, "sourceChanged(MediaSource)");
        mediaObject.aboutToFinish.connect(this, "aboutToFinish()");
//![1]

//![2]
        setupActions();
        setupMenus();
        setupUi();
        timeLcd.display("00:00"); 
    }
//![2]

//![6]
    private void addFiles()
    {
        List<String> files = QFileDialog.getOpenFileNames(this,
                            tr("Select Music Files"), ".");

        if (files.isEmpty())
            return;

        int index = sources.size();
        for (String string : files) {
            MediaSource source = new MediaSource(string);
        
            sources.add(source);
        } 
        if (!sources.isEmpty())
            metaInformationResolver.setCurrentSource(sources.get(index));
    }
//![6]

    private void about()
    {
        QMessageBox.information(this, tr("About Music Player"),
            tr("The Music Player example shows how to use Phonon - the multimedia" +
               " framework that comes with Qt - to create a simple music player."));
    }

//![9]
    private void stateChanged(Phonon.State newState, Phonon.State oldState)
    {
        switch (newState) {
            case ErrorState:
                if (mediaObject.errorType().equals(Phonon.ErrorType.FatalError)) {
                    QMessageBox.warning(this, tr("Fatal Error"),
                    mediaObject.errorString());
                } else {
                    QMessageBox.warning(this, tr("Error"),
                    mediaObject.errorString());
                }
                break;
//![9]
//![10]
            case PlayingState:
                playAction.setEnabled(false);
                pauseAction.setEnabled(true);
                stopAction.setEnabled(true);
                break;
            case StoppedState:
                stopAction.setEnabled(false);
                playAction.setEnabled(true);
                pauseAction.setEnabled(false);
                timeLcd.display("00:00");
                break;
            case PausedState:
                pauseAction.setEnabled(false);
                stopAction.setEnabled(true);
                playAction.setEnabled(true);
                break;
//![10]
            case BufferingState:
                break;
        }
    }

//![11]
    private void tick(long time)
    {
        QTime displayTime = new QTime(0, (int) (time / 60000) % 60, (int) (time / 1000) % 60);

        timeLcd.display(displayTime.toString("mm:ss"));
    }
//![11]

//![12]
    private void tableClicked(int row, int column)
    {
        boolean wasPlaying = mediaObject.state().equals(Phonon.State.PlayingState);

        mediaObject.stop();
        mediaObject.clearQueue();

        mediaObject.setCurrentSource(sources.get(row));

        if (wasPlaying) 
            mediaObject.play();
        else
            mediaObject.stop();
    }
//![12]

//![13]
    private void sourceChanged(MediaSource source)
    {
        musicTable.selectRow(sources.indexOf(source));

        timeLcd.display("00:00");
    }
//![13]

//![14]
    private void metaStateChanged(Phonon.State newState, Phonon.State oldState)
    {
        if (newState.equals(Phonon.State.ErrorState)) {
            QMessageBox.warning(this, tr("Error opening files"),
                metaInformationResolver.errorString());
            while (!sources.isEmpty() &&
                   !(sources.remove(sources.size() - 1).equals(metaInformationResolver.currentSource())));
            return;
        }

        if (!newState.equals(Phonon.State.StoppedState))
            return;

        if (metaInformationResolver.currentSource().type().equals(MediaSource.Type.Invalid))
                return;

        Map<String, List<String>> metaData = metaInformationResolver.metaData();


        String title = "";
        if (metaData.get("TITLE") != null)
            title = metaData.get("TITLE").get(0);

        if (title.equals(""))
            title = metaInformationResolver.currentSource().fileName();

        String artist = "";
        if (metaData.get("ARTIST") != null)
            artist = metaData.get("ARTIST").get(0);

        String album = "";
        if (metaData.get("ALBUM") != null)
            album = metaData.get("ALBUM").get(0);

        String year = "";
        if (metaData.get("DATE") != null)
            year = metaData.get("DATE").get(0);

        QTableWidgetItem titleItem = new QTableWidgetItem(title);
        QTableWidgetItem artistItem = new QTableWidgetItem(artist);
        QTableWidgetItem albumItem = new QTableWidgetItem(album);
        QTableWidgetItem yearItem = new QTableWidgetItem(year);
//![14]

        int currentRow = musicTable.rowCount();
        musicTable.insertRow(currentRow);
        musicTable.setItem(currentRow, 0, titleItem);
        musicTable.setItem(currentRow, 1, artistItem);
        musicTable.setItem(currentRow, 2, albumItem);
        musicTable.setItem(currentRow, 3, yearItem);

//![15]
        if (musicTable.selectedItems().isEmpty()) {
            musicTable.selectRow(0);
            mediaObject.setCurrentSource(metaInformationResolver.currentSource());
        }

        MediaSource source = metaInformationResolver.currentSource();
        int index = sources.indexOf(metaInformationResolver.currentSource()) + 1;
        if (sources.size() > index) {
            metaInformationResolver.setCurrentSource(sources.get(index));
        }
        else {
            musicTable.resizeColumnsToContents();
            if (musicTable.columnWidth(0) > 300)
                musicTable.setColumnWidth(0, 300);
        }
    }
//![15]

//![16]
    private void aboutToFinish()
    {
        int index = sources.indexOf(mediaObject.currentSource()) + 1;
        if (sources.size() > index) {
            mediaObject.enqueue(sources.get(index));
            musicTable.selectRow(index);
        }
    }
//![16]

    private void setupActions()
    {
        playAction = new QAction(new QIcon("classpath:org/qtjambi/examples/images/play.png"), tr("Play"), this);
        playAction.setShortcut(tr("Crl+P"));
        playAction.setDisabled(true);
        pauseAction = new QAction(new QIcon("classpath:org/qtjambi/examples/images/pause.png"), tr("Pause"), this);
        pauseAction.setShortcut(tr("Ctrl+A"));
        pauseAction.setDisabled(true);
        stopAction = new QAction(new QIcon("classpath:org/qtjambi/examples/images/stop.png"), tr("Stop"), this);
        stopAction.setShortcut(tr("Ctrl+S"));
        stopAction.setDisabled(true);
        addFilesAction = new QAction(tr("Add &Files"), this);
        addFilesAction.setShortcut(tr("Ctrl+F"));
        exitAction = new QAction(tr("E&xit"), this);
        exitAction.setShortcut(tr("Ctrl+X"));
        aboutAction = new QAction(tr("A&bout"), this);
        aboutAction.setShortcut(tr("Ctrl+B"));
        aboutQtJambiAction = new QAction(tr("About &Qt Jambi"), this);
        aboutQtJambiAction.setShortcut(tr("Ctrl+Q"));
        aboutQtAction = new QAction(tr("About Q&t"), this);

//![5]
        playAction.triggered.connect(mediaObject, "play()");
        pauseAction.triggered.connect(mediaObject, "pause()");
        stopAction.triggered.connect(mediaObject, "stop()");
//![5]
        addFilesAction.triggered.connect(this, "addFiles()");
        exitAction.triggered.connect(this, "close()");
        aboutAction.triggered.connect(this, "about()");
        aboutQtAction.triggered.connect(QApplication.instance(), "aboutQt()");
        aboutQtJambiAction.triggered.connect(QApplication.instance(), "aboutQtJambi()");
    }

    private void setupMenus()
    {
        QMenu fileMenu = menuBar().addMenu(tr("&File"));
        fileMenu.addAction(addFilesAction);
        fileMenu.addSeparator();
        fileMenu.addAction(exitAction);

        QMenu aboutMenu = menuBar().addMenu(tr("&Help"));
        aboutMenu.addAction(aboutAction);
        aboutMenu.addAction(aboutQtJambiAction);
        aboutMenu.addAction(aboutQtAction);        
    }

//![3]
    private void setupUi()
    {
//![3]
        QToolBar bar = new QToolBar();

        bar.addAction(playAction);
        bar.addAction(pauseAction);
        bar.addAction(stopAction);
    
//![4]
        seekSlider = new SeekSlider(this);
        seekSlider.setMediaObject(mediaObject);

        volumeSlider = new VolumeSlider(this);
        volumeSlider.setAudioOutput(audioOutput);
//![4]
        volumeSlider.setSizePolicy(QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Maximum);

        QLabel volumeLabel = new QLabel();
        volumeLabel.setPixmap(new QPixmap("images/volume.png"));

        QPalette palette = new QPalette();
        palette.setBrush(QPalette.ColorRole.Light, new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.darkGray)));

        timeLcd = new QLCDNumber();
        timeLcd.setPalette(palette);

        List<String> headers = new Vector<String>();
        headers.add(tr("Title"));
        headers.add(tr("Artist"));
        headers.add(tr("Album"));
        headers.add(tr("Year"));

        musicTable = new QTableWidget(0, 4);
        musicTable.setHorizontalHeaderLabels(headers);
        musicTable.setSelectionMode(QAbstractItemView.SelectionMode.SingleSelection);
        musicTable.setSelectionBehavior(QAbstractItemView.SelectionBehavior.SelectRows);
        musicTable.cellPressed.connect(this, "tableClicked(int,int)");

        QHBoxLayout seekerLayout = new QHBoxLayout();
        seekerLayout.addWidget(seekSlider);
        seekerLayout.addWidget(timeLcd);

        QHBoxLayout playbackLayout = new QHBoxLayout();
        playbackLayout.addWidget(bar);
        playbackLayout.addStretch();
        playbackLayout.addWidget(volumeLabel);
        playbackLayout.addWidget(volumeSlider);

        QVBoxLayout mainLayout = new QVBoxLayout();
        mainLayout.addWidget(musicTable);
        mainLayout.addLayout(seekerLayout);
        mainLayout.addLayout(playbackLayout);

        QWidget widget = new QWidget();
        widget.setLayout(mainLayout);

        setCentralWidget(widget);
        setWindowTitle("Phonon Music Player");
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }

//![variables]
    private SeekSlider seekSlider;
    private MediaObject mediaObject;
    private MediaObject metaInformationResolver;
    private AudioOutput audioOutput;
    private VolumeSlider volumeSlider;
    private List<MediaSource> sources =
        new Vector<MediaSource>();
//![variables]

    private QAction playAction;
    private QAction pauseAction;
    private QAction stopAction;
    private QAction addFilesAction;
    private QAction exitAction;
    private QAction aboutAction;
    private QAction aboutQtAction;
    private QAction aboutQtJambiAction;
    private QLCDNumber timeLcd;
    private QTableWidget musicTable;

//![main]
    public static void main(String args[])
    {
            QApplication.initialize(args);
            QApplication.setApplicationName("Music Player");

            new MusicPlayer().show();

            QApplication.execStatic();
            QApplication.shutdown();
    }
//![main]
}
