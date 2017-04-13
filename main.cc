#include <QApplication>

#include "chat_dialog.hh"
#include "net_socket.hh"

int main(int argc, char **argv)
{
	// Initialize Qt toolkit
	QApplication app(argc,argv);

	// Create an initial chat dialog window
	ChatDialog dialog;
	dialog.show();

	// // Create a UDP network socket
	NetSocket sock;
	if (!sock.bind())
		exit(1);

	dialog.peerID = &sock.peerID;
	dialog.msgID = &sock.msgID;

	//signal-slot used to send input data to neighbours
	QObject::connect(&dialog, SIGNAL(sendData(QString)),&sock, SLOT(serialize(QString)));
	
	//signal-slot used to display received data on screen
	QObject::connect(&sock, SIGNAL(sendForDisplay(QString)),&dialog, SLOT(displayMessage(QString)));

	// Enter the Qt main loop; everything else is event driven
	return app.exec();
}