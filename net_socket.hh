#ifndef P2PAPP_NET_SOCKET_HH
#define P2PAPP_NET_SOCKET_HH

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
  
public slots:
	int serialize(QString data);

signals:
  void sendForDisplay(QString message);

private:
	int myPortMin, myPortMax;
  	QByteArray pendingDatagram;

private slots:
  void deserialization();
};

#endif