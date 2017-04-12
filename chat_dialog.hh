#ifndef P2PAPP_CHAT_DIALOG_HH
#define P2PAPP_CHAT_DIALOG_HH

#include <QDialog>
#include <QApplication>
#include <QTextEdit>
#include <QLineEdit>

class ChatDialog : public QDialog
{
	Q_OBJECT

public:
	ChatDialog();

public slots:
	void gotReturnPressed();
	void displayMessage(QString);

signals:
	void sendData(QString);

private:
	QTextEdit *textview;
	QLineEdit *textline;
};

#endif