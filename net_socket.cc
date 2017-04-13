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

	//initialize clock status with peerID and exepected messageID
	clockState.insert(peerID,msgID);

	connect(this, SIGNAL(readyRead()), this, SLOT(receiveInput()), Qt::DirectConnection);

}

int NetSocket::serialize(QString data){

	qDebug()<<"Entering Serialize";

	//Serialization
	QMap<QString,QVariant> dataMap;
	dataMap.insert("ChatText", QVariant(data));
	dataMap.insert("Origin", QVariant(peerID));
	dataMap.insert("SeqNo", QVariant(msgID));
	

	QByteArray dataArray;
	QDataStream * opStream = new QDataStream(&dataArray, QIODevice::WriteOnly);
	(*opStream) << dataMap;

	int sentData = NOT_DEFINED;

	//send next sequential message
	if(clockState.value(peerID)==msgID){
		msgID++;
		//update clockstate
		clockState.insert(peerID,msgID);
		qDebug() << "Updated clock status";

		for(int port = myPortMin; port<myPortMax;port++){
			int currentData=0;
			if((currentData = writeDatagram(dataArray,QHostAddress::LocalHost,port))>0){
				qDebug()<<"Data sent to port"<<port;
				sentData+=currentData;
			}
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
				neighbours.push_back(p+1);
			}
			else if(p==myPortMax){
				neighbours.push_back(p-1);
			}
			else{
				neighbours.push_back(p-1);
				neighbours.push_back(p+1);
			}
			qDebug() << "bound to UDP port " << p;
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

	int ackStatus;

	//received status message
	if(dataMap.contains("Want")){
		qDebug() << "Received Status Message";
		//handle status message
	}
	else{
		qDebug() << "Recevied rumour Message";
		QString message = dataMap.value("ChatText").toString();
		QString seqNo = dataMap.value("SeqNo").toString();
		QString peer = dataMap.value("Origin").toString();

		//complying with RFC for allowed message ID
		if(seqNo.contains(QChar('/')) || seqNo.contains(QChar(' ')) || seqNo.contains(QChar('\t')) ){
			qDebug() <<"Incorrect Message ID";
			return ERROR;
		}

		
		if(!clockState.contains(peer)){
			//add peer to system state
			clockState.insert(peer,1);
		}else{
			QString expctdSeqNo = clockState.value(peer).toString();
			//check if message arrived in sequence
			if(seqNo.compare(expctdSeqNo) == 0){
				
				//update clock status
				clockState.insert(peer, seqNo+1);
				
				qDebug() << "Correct Message Received";

				//send signal for displaying message
    			emit sendForDisplay(message);

    			//send acknowledgment
				
				if ((ackStatus = sendAck(sender,senderPort))<0){
					qDebug() << "Error in Sending Acknowledgment";
				}

				//continue rumouring
				//Send to random neighbour
			}
		}
		
	}

	return ackStatus;
}

int NetSocket::receiveInput() {

	qDebug() << "data ready for receiving";
	int totalDataRead = NOT_DEFINED;

	while (hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;

		int dataRead=0;
		if((dataRead = readDatagram(datagram.data(), datagram.size(), &sender, &senderPort))>0){
			
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