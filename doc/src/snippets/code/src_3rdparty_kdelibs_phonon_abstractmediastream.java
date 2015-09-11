import org.qtjambi.qt.core.*;
import org.qtjambi.qt.phonon.*;

abstract class PushStream extends AbstractMediaStream {

    private QTimer m_timer;

    private int getMediaStreamSize() { return 0; }
    private QByteArray getMediaData() { return new QByteArray("A MOVIE!"); }

//! [0]
    public PushStream(QObject parent) {
        super(parent);
        m_timer = new QTimer(this);

        setStreamSize(getMediaStreamSize());

        m_timer.timeout.connect(this, "moreData()");
        m_timer.setInterval(0);
   }

    public void moreData() {
        QByteArray data = getMediaData();
        if (data.isEmpty()) {
            endOfData();
        } else {
            writeData(data);
        }
    }

    @Override
    protected void needData() {
        m_timer.start();
        moreData();
    }

    @Override
    protected void enoughData() {
        m_timer.stop();
    }
//! [0]
}


abstract class PullStream extends AbstractMediaStream {

    private int getMediaStreamSize() { return 0; }
    private QByteArray getMediaData() { return new QByteArray("A SONG!"); }

//! [1]
    public PullStream(QObject parent) {
        super(parent);
        setStreamSize(getMediaStreamSize());
    }

    @Override
    protected void needData() {
        QByteArray data = getMediaData();
        if (data.isEmpty()) {
          endOfData();
        } else {
          writeData(data);
        }
    }
//! [1]


    void fooBar() {
//! [2]
         seekStream(0);
//! [2]
    }
}
