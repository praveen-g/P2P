#include <iostream>
#include <QApplication>
#include "chat_dialog.hh"

QString uuid;

void msgHandler(QtMsgType type, const char* msg) {
  QString output = QString("%1: %2").arg(uuid).arg(msg);
  std::cerr << output.toStdString() << std::endl;
  if( type == QtFatalMsg ) abort();
}

int main(int argc, char **argv)
{
	// Initialize Qt toolkit
	QApplication app(argc,argv);

	// Create an initial chat dialog window
  ChatDialog dialog;
	dialog.show();
  uuid = dialog.getUuid();
  qInstallMsgHandler(msgHandler);
	// Enter the Qt main loop; everything else is event driven
	return app.exec();
}
