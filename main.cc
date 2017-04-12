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

	// Create a UDP network socket
	NetSocket sock;
	if (!sock.bind())
		exit(1);

	QObject::connect(&dialog, SIGNAL(sendData(QString)),
		&sock, SLOT(serialize(QString)));

	// Enter the Qt main loop; everything else is event driven
	return app.exec();
}