#include <QDebug>
#include <QVBoxLayout>

#include "chat_dialog.hh"
#include "message.hh"

ChatDialog::ChatDialog() {
	setWindowTitle("P2Papp");
  uuid = QUuid::createUuid();
  socket = new NetSocket(uuid);
  seqNum = 1;
  connect(socket, SIGNAL(messageReady()), this, SLOT(handleReadyRead()), Qt::DirectConnection);

	// Read-only text box where we display messages from everyone.
	// This widget expands both horizontally and vertically.
	textview = new QTextEdit(this);
	textview->setReadOnly(true);

	// Small text-entry box the user can enter messages.
	// This widget normally expands only horizontally,
	// leaving extra vertical space for the textview widget.
	//
	// You might change this into a read/write QTextEdit,
	// so that the user can easily enter multi-line messages.
	textline = new QLineEdit(this);

	// Lay out the widgets to appear in the main window.
	// For Qt widget and layout concepts see:
	// http://doc.qt.nokia.com/4.7-snapshot/widgets-and-layouts.html
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(textview);
	layout->addWidget(textline);
	setLayout(layout);

	// Register a callback on the textline's returnPressed signal
	// so that we can send the message entered by the user.
	connect(textline, SIGNAL(returnPressed()),
		this, SLOT(gotReturnPressed()));
}

ChatDialog::~ChatDialog() {
  delete socket;
}

void ChatDialog::gotReturnPressed() {
	// Initially, just echo the string locally.
	// Insert some networking code here...
	textview->append(uuid.toString() + " says: " + textline->text());

  Message msg(textline->text().toUtf8().data(), uuid.toString(), seqNum++);
  socket->updateStatus(msg);
  socket->rumor(msg);
	// Clear the textline to get ready for the next input message.
	textline->clear();
}

QString ChatDialog::getUuid() {
  return uuid.toString();
}

//private

void ChatDialog::handleReadyRead() {
  Message msg = socket->getNextMessage();
	qDebug() << "getting data: " << msg;
  textview->append(msg.printOrigin() + " says: " + msg.printMessage());
}
