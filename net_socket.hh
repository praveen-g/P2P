#ifndef P2PAPP_NET_SOCKET_HH
#define P2PAPP_NET_SOCKET_HH
#define NEIGHBOURS 2
#define NOT_DEFINED -1
#define ERROR -1

#include <QUdpSocket>
#include <QApplication>

class NetSocket : public QUdpSocket
{
	Q_OBJECT

public:
	NetSocket();
	QString peerID;
	quint32 msgID;

  // Bind this socket to a P2Papp-specific default port.
  bool bind();
  int processInput(QMap<QString, QVariant> dataMap,QHostAddress sender, quint16 senderPort);
  int sendAck(QHostAddress sender, quint16 senderPort);
  
public slots:
	int serialize(QString data);

signals:
  void sendForDisplay(QString message);

private:
	int myPortMin, myPortMax;
  	QByteArray pendingDatagram;
  	int neighbours[NEIGHBOURS];

private slots:
  int receiveInput();
};

#endif