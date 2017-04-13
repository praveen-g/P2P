#include <QDebug>

#include <unistd.h>
#include "net_socket.hh"

NetSocket::NetSocket()
{
	// Pick a range of four UDP ports to try to allocate by default,
	// computed based on my Unix user ID.
	// This makes it trivial for up to four P2Papp instances per user
	// to find each other on the same host,
	// barring UDP port conflicts with other applications
	// (which are quite possible).
	// We use the range from 32768 to 49151 for this purpose.
	myPortMin = 32768 + (getuid() % 4096)*4;
	myPortMax = myPortMin + 3;

	qsrand(time(0));
	peerID = QString::number(qrand());QString::number(qrand());
	msgID = 1;
	connect(this, SIGNAL(readyRead()), this, SLOT(receiveInput()), Qt::DirectConnection);

}

int NetSocket::serialize(QString data){

	qDebug()<<"Entering Serialize";

	//Serialization
	QMap<QString,QVariant> dataMap;
	dataMap.insert("ChatText", QVariant(data));
	dataMap.insert("Origin", QVariant(peerID));
	dataMap.insert("SeqNo", QVariant(msgID));
	msgID++;

	QByteArray dataArray;
	QDataStream * opStream = new QDataStream(&dataArray, QIODevice::WriteOnly);
	(*opStream) << dataMap;

	int sentData = 0;
	for(int port = myPortMin; port<myPortMax;port++){
		int currentData=0;
		if((currentData = writeDatagram(dataArray,QHostAddress::LocalHost,port))>0){
			qDebug()<<"Data sent to port"<<port;
			sentData+=currentData;
		}
	}
	
	if (sentData == 0) {
		qDebug() << "Error in Sending Data";
	} else {
		qDebug() << sentData << " Bytes Sent Successfuly";
	}
	return sentData;
}

// private
bool NetSocket::bind() {
	// Try to bind to each of the range myPortMin..myPortMax in turn.
	for (int p = myPortMin; p <= myPortMax; p++) {
		if (QUdpSocket::bind(p)) {
			if(p==myPortMin){
				neighbours[0]=NOT_DEFINED;
				neighbours[1]=p+1;
			}
			else if(p==myPortMax){
				neighbours[0]=p-1;
				neighbours[1]=NOT_DEFINED;
			}
			else{
				neighbours[0]=p-1;
				neighbours[1]=p+1;
			}
			qDebug() << "bound to UDP port " << p;
			qDebug() << "\n Neighbours: ";
			for(int i=0;i<NEIGHBOURS; i++){
				if (neighbours[i] != NOT_DEFINED ){
					qDebug() << neighbours[i] << " ";
				}
			}
			return true;
		}
	}

	qDebug() << "Oops, no ports in my default range " << myPortMin
		   << "-" << myPortMax << " available";
	return false;
}

int NetSocket::sendAck(QHostAddress sender, quint16 senderPort){

	//create want map
	QByteArray datagram;

	//send want map
	int ackSent = writeDatagram(datagram, sender, senderPort);
	return ackSent;
}

int NetSocket::processInput(QMap<QString, QVariant> dataMap,QHostAddress sender, quint16 senderPort){

	//received status message
	if(dataMap.contains("Want")){
		qDebug() << "Received Status Message";
		//handle status message
	}
	else{
		QString message = dataMap.value("ChatText").toString();
		QString seqNo = dataMap.value("SeqNo").toString();

		//complying with RFC for allowed message ID
		if(seqNo.contains(QChar('/')) || seqNo.contains(QChar(' ')) || seqNo.contains(QChar('\t')) ){
			qDebug() <<"Incorrect Message ID";
			return ERROR;
		}
		pendingDatagram += message;
		qDebug() << "data buffed: " << pendingDatagram.data();
		qDebug() << "sending signal";

		//send signal for displaying message
    	emit sendForDisplay(pendingDatagram);
	}

	//send acknowledgment
	int ackStatus;
	if ((ackStatus = sendAck(sender,senderPort))<0){
		qDebug() << "Error in Sending Acknowledgment";
	}
	return ackStatus;
}

int NetSocket::receiveInput() {

	qDebug() << "data ready for receiving";
	int totalDataRead=0;

	while (hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;

		int dataRead=0;
		if((dataRead = readDatagram(datagram.data(), datagram.size(), &sender, &senderPort))>0){
			qDebug()<<"Error Reading Data from port"<<senderPort;
			QMap<QString, QVariant> dataMap;
			QDataStream inStream(&datagram, QIODevice::ReadOnly);
			inStream >> dataMap;

			if(processInput(dataMap, sender, senderPort) <0){
				return ERROR;
			}
			
			totalDataRead+=dataRead;
		}
  	}


    if (totalDataRead == 0) {
		qDebug() << "Error in Reading Data";
	} else {
		qDebug() << totalDataRead << " Bytes Read Successfuly";
	}
	return totalDataRead;
}