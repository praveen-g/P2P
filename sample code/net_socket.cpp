#include "net_socket.hh"

NetSocket::NetSocket(QUuid uuid) : uuid(uuid) {
	// Pick a range of four UDP ports to try to allocate by default,
	// computed based on my Unix user ID.
	// This makes it trivial for up to four P2Papp instances per user
	// to find each other on the same host,
	// barring UDP port conflicts with other applications
	// (which are quite possible).
	// We use the range from 32768 to 49151 for this purpose.
	myPortMin = 32768 + (getuid() % 4096)*4;
	myPortMax = myPortMin + 3;
  bindPort();
  connect(this, SIGNAL(readyRead()), this, SLOT(handleReadyRead()), Qt::DirectConnection);
  connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(&antitimer, SIGNAL(timeout()), this, SLOT(antientropy()));
  antitimer.start(10*1000);
}

void NetSocket::rumor(const Message& msg) {
  timeoutMessage = msg;
  random_shuffle(neighbors.begin(), neighbors.end());
  qDebug() << "rumoring message: <" << msg << "> to port " << neighbors.front();
  this->writeDatagram(msg.serialize(), QHostAddress::LocalHost, neighbors.front());
  timer.start(1000);
}

const Message& NetSocket::getNextMessage() const {
  return pendingMessage;
}

// private

bool NetSocket::bindPort() {
	// Try to bind to each of the range myPortMin..myPortMax in turn.
	for (int p = myPortMin; p <= myPortMax; p++) {
		if (this->bind(QHostAddress::LocalHost, p)) {
			qDebug() << "bound to UDP port " << p;
      currentPort = p;
      if (currentPort == myPortMin) {
        neighbors.push_back(currentPort + 1);
      } else if (currentPort == myPortMax) {
        neighbors.push_back(currentPort - 1);
      } else {
        neighbors.push_back(currentPort - 1);
        neighbors.push_back(currentPort + 1);
      }
			return true;
		}
	}

	qDebug() << "Oops, no ports in my default range " << myPortMin
           << "-" << myPortMax << " available";
	return false;
}

void NetSocket::handleReadyRead() {
  QByteArray res;
  QHostAddress sender;
  quint16 senderPort;
	qDebug() << "data ready for receiving";
  while (this->hasPendingDatagrams()) {
    QByteArray datagram;
    datagram.resize(this->pendingDatagramSize());
    this->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    res += datagram;
  }
  pendingMessage = Message(res);
  if (pendingMessage.isRumor()) {
    qDebug() << "rumor received: " << pendingMessage;
    int check = updateStatus(pendingMessage);
    if (check > 0) {
      if (check == 1) messageReady();
      qDebug() << "new message received, sending back status";
      rumorStatusBack(senderPort);
      qDebug() << "new message received, rumoring";
      rumor(pendingMessage);
    } else {
      qDebug() << "message discarded";
    }
  } else {
    timer.start(1000);
    qDebug() << "status received: " << pendingMessage;
    compareStatus(pendingMessage, senderPort);
  }
}

void NetSocket::rumorBack(const Message& msg,
                          const quint16& senderPort) {
  this->writeDatagram(msg.serialize(), QHostAddress::LocalHost, senderPort);
  qDebug() << "rumor back message: <" << msg << "> to port " << senderPort;
}

void NetSocket::rumorStatusBack(const quint16& senderPort) {
  Message msg(getStatus());
  this->writeDatagram(msg.serialize(), QHostAddress::LocalHost, senderPort);
  qDebug() << "rumor back status: <" << msg << "> to port " << senderPort;
}

Message NetSocket::getStatus() {
  QMap<QString, QVariant> statusM;
  QMap<QString, vector<Message> >::iterator i;
  for (i = status.begin(); i != status.end(); ++i) {
    if (i.value().empty()) {
      statusM[i.key()] = 1;
    } else {
      statusM[i.key()] = i.value().back().getSeq() + 1;
    }
  }
  return Message(statusM);
}

void NetSocket::timeout() {
  qDebug() << "Timeout fired";
  if (qrand() < RAND_MAX / 2) {
    qDebug() << "stop rumoring";
    timer.stop();
    return;
  }
  qDebug() << "continue rumoring";
  rumor(timeoutMessage);
}

void NetSocket::antientropy() {
  qDebug() << "anti-entropy";
  random_shuffle(neighbors.begin(), neighbors.end());
  rumorStatusBack(neighbors.front());
}

int NetSocket::updateStatus(const Message& msg) {
  qDebug() << "updating status" << msg;
  quint32 seq = msg.getSeq();
  vector<Message> &tStatus = status[msg.printOrigin()];

  if (tStatus.empty()) {
    if (seq == 1) {
      tStatus.push_back(msg);
      qDebug() << "status for " << msg.printOrigin() << " is now:"
               << tStatus.back().getSeq();
      return 1; // for updating status
    }
    qDebug() << "not updating status";
    return 2; // for not updating status, seq # is too big
  }
  if (seq == tStatus.back().getSeq() + 1) {
    tStatus.push_back(msg);
    qDebug() << "status for " << msg.printOrigin() << " is now:"
             << tStatus.back().getSeq();
    return 1; // for updating status
  } else if (seq > tStatus.back().getSeq() + 1){
    qDebug() << "not updating status";
    return 2; // for not updating status, seq # is too big
  }
  return 0; // for message already received
}

void NetSocket::compareStatus(const Message& msg,
                              const quint16& senderPort) {
  QMap<QString, QVariant> incomingStatus = msg.getStatus();
  QMap<QString, QVariant> currStatus = getStatus().getStatus();
  QMap<QString, QVariant>::iterator i;

  // merge two status
  for (i = currStatus.begin(); i != currStatus.end(); i++) {
    if (!incomingStatus.contains(i.key())) {
      incomingStatus[i.key()] = 1;
    }
  }
  for (i = incomingStatus.begin(); i != incomingStatus.end(); i++) {
    if (!currStatus.contains(i.key())) {
      currStatus[i.key()] = 1;
    }
  }

  // first send what I have to you
  for (i = currStatus.begin(); i != currStatus.end(); i++) {
    if (currStatus[i.key()].toUInt() > incomingStatus[i.key()].toUInt()) {
      rumorBack(status[i.key()][incomingStatus[i.key()].toUInt() - 1], senderPort);
      return;
    }
  }

  // then request what you have
  for (i = currStatus.begin(); i != currStatus.end(); i++) {
    if (currStatus[i.key()].toUInt() < incomingStatus[i.key()].toUInt()) {
      rumorStatusBack(senderPort);
      return;
    }
  }
}
