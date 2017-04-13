#ifndef P2PAPP_NET_SOCKET_HH
#define P2PAPP_NET_SOCKET_HH

#include <unistd.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <QUdpSocket>
#include <QByteArray>
#include <QTimer>
#include <QUuid>
#include "message.hh"

using namespace std;

class NetSocket : public QUdpSocket {
	Q_OBJECT

public:
	NetSocket(QUuid uuid);

  void rumor(const Message &);
  const Message &getNextMessage() const;
  int updateStatus(const Message& msg);

signals:
  void messageReady();

private:
  QUuid uuid;
	int myPortMin;
  int myPortMax;
  int currentPort;
  vector<int> neighbors;
  Message pendingMessage;
  Message timeoutMessage;
  QTimer timer;
  QTimer antitimer;
  QMap<QString, vector<Message> > status;

	// Bind this socket to a P2Papp-specific default port.
	bool bindPort();
  void flip();
  Message getStatus();
  void rumorStatusBack(const quint16& senderPort);
  void rumorBack(const Message& msg, const quint16& senderPort);
  void compareStatus(const Message& msg,
                     const quint16& senderPort);

private slots:
  void handleReadyRead();
  void timeout();
  void antientropy();
};

#endif
