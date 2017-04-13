#include <QVBoxLayout>
#include <QDebug>

#include "chat_dialog.hh"

ChatDialog::ChatDialog()
{
	setWindowTitle("P2Papp");

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

void ChatDialog::gotReturnPressed()
{
	qDebug() << "Origin: " << *peerID << " Seq Num: "<< QString::number(*msgID)<<" Text:" << textline->text();
	QString text = *peerID+"-"+QString::number(*msgID)+"-"+textline->text();

	textview->append(text);

	emit sendData(text);

	// Clear the textline to get ready for the next input message.
	textline->clear();
}

void ChatDialog::displayMessage(QString message) {
	qDebug() << "getting data: " << message;
	textview->append(message);
}