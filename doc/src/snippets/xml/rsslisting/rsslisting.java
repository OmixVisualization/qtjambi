
RSSListing.RSSListing(QWidget parent)
    : QWidget(parent)
{
    lineEdit = new QLineEdit(this);

    fetchButton = new QPushButton(tr("Fetch"), this);
    abortButton = new QPushButton(tr("Abort"), this);
    abortButton.setEnabled(false);

    treeWidget = new QTreeWidget(this);
    List<String> headerLabels;
    headerLabels << tr("Title") << tr("Link");
    treeWidget.setHeaderLabels(headerLabels);

    handler = 0;

    connect(http, SIGNAL(readyRead(QHttpResponseHeader )),
             this, SLOT(readData(QHttpResponseHeader )));

    connect(http, SIGNAL(requestFinished(int, bool)),
             this, SLOT(finished(int, bool)));

    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(fetch()));
    connect(fetchButton, SIGNAL(clicked()), this, SLOT(fetch()));
    connect(abortButton, SIGNAL(clicked()), http, SLOT(abort()));

    QVBoxLayout layout = new QVBoxLayout(this);

    QHBoxLayout hboxLayout = new QHBoxLayout;

    hboxLayout.addWidget(lineEdit);
    hboxLayout.addWidget(fetchButton);
    hboxLayout.addWidget(abortButton);

    layout.addLayout(hboxLayout);
    layout.addWidget(treeWidget);

    setWindowTitle(tr("RSS listing example"));
}


void RSSListing.fetch()
{
    lineEdit.setReadOnly(true);
    fetchButton.setEnabled(false);
    abortButton.setEnabled(true);
    treeWidget.clear();

    lastItemCreated = 0;

    newInformation = true;

    handler = new Handler;

//! [0]
    xmlReader.setContentHandler(handler);
    xmlReader.setErrorHandler(handler);
//! [0]

    connect(handler, SIGNAL(newItem(Strings, Strings)),
             this, SLOT(addItem(Strings, Strings)));

    QUrl url(lineEdit.text());

    http.setHost(url.host());
    connectionId = http.get(url.path());
}

//! [1]
void readData(QHttpResponseHeader resp)
{
    boolean sok;

    if (resp.statusCode() != 200)
        http.abort();
    else {
        xmlInput.setData(http.readAll());

        if (newInformation) {
            ok = xmlReader.parse(xmlInput, true);
            newInformation = false;
        }
        else
            ok = xmlReader.parseContinue();

        if (!ok)
            http.abort();
    }
}
//! [1]

void RSSListing.finished(int id, booleanserror)
{
    if (error) {
        qWarning("Received error during HTTP fetch.");
        lineEdit.setReadOnly(false);
        abortButton.setEnabled(false);
        fetchButton.setEnabled(true);
    }
    else if (id == connectionId) {

        booleansok = xmlReader.parseContinue();
        if (!ok)
            qWarning("Parse error at the end of input.");

        lineEdit.setReadOnly(false);
        abortButton.setEnabled(false);
        fetchButton.setEnabled(true);
    }
}

/
    Adds an item to the list view as it is reported by the handler.

    We keep a record of the last item created to ensure that the
    items are created in sequence.
/

void RSSListing.addItem(Stringstitle, Stringslink)
{
    QTreeWidgetItem item;

    item = new QTreeWidgetItem(treeWidget, lastItemCreated);
    item.setText(0, title);
    item.setText(1, link);

    lastItemCreated = item;
}

