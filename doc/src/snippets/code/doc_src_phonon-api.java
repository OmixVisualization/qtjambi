//! [0]
/*PushStream::PushStream(QObject *parent)
  : AbstractMediaStream(parent), m_timer(new QTimer(this))
{
  setStreamSize(getMediaStreamSize());

  connect(m_timer, SIGNAL(timeout()), SLOT(moreData()));
  m_timer->setInterval(0);
}

void PushStream::moreData()
{
  const QByteArray data = getMediaData();
  if (data.isEmpty()) {
    endOfData();
  } else {
    writeData(data);
  }
}

void PushStream::needData()
{
  m_timer->start();
  moreData();
}

void PushStream::enoughData()
{
  m_timer->stop();
}*/
//! [0]


//! [1]
/*PullStream::PullStream(QObject *parent)
  : AbstractMediaStream(parent)
{
  setStreamSize(getMediaStreamSize());
}

void PullStream::needData()
{
  const QByteArray data = getMediaData();
  if (data.isEmpty()) {
    endOfData();
  } else {
    writeData(data);
  }
}*/
//! [1]


//! [2]
//seekStream(0);
//! [2]


//! [3]

// Note: Java does not have implicit constructors.

MediaObject m = new MediaObject();
String fileName = "/home/foo/bar.ogg";
QUrl url = new QUrl("http://www.example.com/stream.mp3");
QBuffer someBuffer = new QBuffer();
m.setCurrentSource(new MediaSource(fileName));
m.setCurrentSource(new MediaSource(url));
m.setCurrentSource(new MediaSource(someBuffer));
m.setCurrentSource(new MediaSource(Phonon.DiskType.Cd));
//! [3]


//! [4]
VideoPlayer player = new VideoPlayer(Phonon.Category.VideoCategory, parentWidget);
player.play(new MediaSource(url));
//! [4]


//! [5]
audioPlayer.load(new MediaSource(url));
audioPlayer.play();
//! [5]


//! [6]
media = new MediaObject(this);
connect(media, SIGNAL(finished()), SLOT(slotFinished());
media.finished.connect(this, "slotFinished()");
media.setCurrentSource(new MediaSource("/home/username/music/filename.ogg"));

...

media.play();
//! [6]


//! [7]
media.setCurrentSource(new MediaSource("classpath:sounds/startsound.ogg"));
media.enqueue(new MediaSource("/home/username/music/song.mp3"));
media.enqueue(new MediaSource("classpath:sounds/endsound.ogg"));
//! [7]


//! [8]
  media.setCurrentSource(new MediaSource("classpath:/sounds/startsound.ogg"));
  media.aboutToFinish.connect(this, "enqueueNextSource()");
}

private void enqueueNextSource()
{
  media.enqueue(new MediaSource("/home/username/music/badboys.mp3"));
}
//! [8]


//! [9]
int x = 200;
media.setTickInterval(x);
(x == producer.tickInterval());
//! [9]


//! [10]
int x = 200;
media.setTickInterval(x);
(x >= producer.tickInterval() &&
 x <= 2producer.tickInterval());
//! [10]


//! [11]
  media.hasVideoChanged.connect(this, "hasVideoChanged(Boolean)")
  media.setCurrentSource("returnofthejedi.avi");
  media.hasVideo(); // returns false;
}

public void hasVideoChanged(bool b)
{
  // b == true
  media.hasVideo(); // returns true;
}
//! [11]


//! [12]
  media.hasVideoChanged.connect(this, "hasVideoChanged(Boolean)");
  media.setCurrentSource("somevideo.avi");
  media.hasVideo(); // returns false;
}

public void hasVideoChanged(bool b)
{
  // b == true
  media.hasVideo(); // returns true;
}
//! [12]


//! [13]
setMetaArtist (media.metaData("ARTIST"     ));
setMetaAlbum  (media.metaData("ALBUM"      ));
setMetaTitle  (media.metaData("TITLE"      ));
setMetaDate   (media.metaData("DATE"       ));
setMetaGenre  (media.metaData("GENRE"      ));
setMetaTrack  (media.metaData("TRACKNUMBER"));
setMetaComment(media.metaData("DESCRIPTION"));
//! [13]


//! [14]
QUrl url = new QUrl("http://www.example.com/music.ogg");
media.setCurrentSource(url);
//! [14]


//! [15]
progressBar.setRange(0, 100); // this is the default
media.bufferStatus.connect(this, "setValue(Integer)");
//! [15]

// This is not yet exported to the Jambi API, needs checkup when exported
//! [16]
BackendCapabilities.notifier().capabilitiesChanged.connect(...
//! [16]


//! [17]
QComboBox cb = new QComboBox(parentWidget);
ObjectDescriptionModel model = new ObjectDescriptionModel(cb);
model.setModelData(BackendCapabilities.availableAudioOutputDevices());
cb.setModel(model);
cb.setCurrentIndex(0); // select first entry
//! [17]


//! [18]
int cbIndex = cb.currentIndex();
AudioOutputDevice selectedDevice = model.modelData(cbIndex);
//! [18]


//! [19]
Path path = Phonon.createPath(...);
Effect effect = new Effect(this);
path.insertEffect(effect);
//! [19]


//! [20]
MediaObject media = new MediaObject();
AudioOutput output = new AudioOutput(Phonon.Category.MusicCategory);
Path path = Phonon.createPath(media, output);
if (path.isValid()) { // for this simple case the path should always be
    ...               //valid - there are unit tests to ensure it
}
// insert an effect
List<EffectDescription> effectList = BackendCapabilities.availableAudioEffects();
if (!effectList.isEmpty()) {
    Effect *effect = path.insertEffect(effectList.get(0));
}
//! [20]


//! [21]
MediaObject media = new MediaObject(parent);
VideoWidget vwidget = new VideoWidget(parent);
Phonon.createPath(media, vwidget);
//! [21]
