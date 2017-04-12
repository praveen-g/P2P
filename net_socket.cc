#include <QApplication>
#include <QDebug>

#include <unistd.h>
#include "net_socket.hh"

int NetSocket::serialize(QString data){

	qDebug()<<"Entering Serialize";

	//Serialization
    QMap<QString,QVariant> dataMap;
    dataMap.insert("Text", data);
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
}

bool NetSocket::bind()
{
	// Try to bind to each of the range myPortMin..myPortMax in turn.
	for (int p = myPortMin; p <= myPortMax; p++) {
		if (QUdpSocket::bind(p)) {
			qDebug() << "bound to UDP port " << p;
			return true;
		}
	}

	qDebug() << "Oops, no ports in my default range " << myPortMin
		<< "-" << myPortMax << " available";
	return false;
}
