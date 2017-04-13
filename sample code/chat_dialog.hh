#ifndef P2PAPP_CHAT_DIALOG_HH
#define P2PAPP_CHAT_DIALOG_HH

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include "net_socket.hh"
#include "message.hh"

class ChatDialog : public QDialog
{
	Q_OBJECT

public:
	ChatDialog();
	~ChatDialog();
  QString getUuid();

private slots:
	void gotReturnPressed();

private:
	QTextEdit *textview;
	QLineEdit *textline;
  NetSocket *socket;
  QUuid uuid;
  quint32 seqNum;

private slots:
  void handleReadyRead();
};

#endif
