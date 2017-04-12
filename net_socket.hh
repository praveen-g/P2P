#include <QUdpSocket>

class NetSocket : public QUdpSocket
{
	Q_OBJECT

public:
	NetSocket();

	// Bind this socket to a P2Papp-specific default port.
	bool bind();

public slots:
	int serialize(QString data);


private:
	int myPortMin, myPortMax;
};