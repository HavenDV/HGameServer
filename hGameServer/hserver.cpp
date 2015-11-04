#include <QTimer>
#include "hserver.h"
#include <qnetworkinterface.h>

constexpr auto serverObjectName = "server";

inline auto	ipString( const QHostAddress & ip, quint16 port ) -> QString {
	return QString("%1:%2").arg( ip.toString() ).arg( port );
}

HServer::HServer( QObject *parent ) : 
	HThreadParent{ parent }, t{ this }, world{ new HWorld{ this } }, tcpServer{ this } {
	connect( &tcpServer, &QTcpServer::newConnection, this, &HServer::newConnect );
	connect( &tcpServer, &QTcpServer::acceptError, this, &HServer::error );
	qRegisterMetaType<GObjectType>();
	addThread( new HGameServer( world, serverObjectName ) );

	t.start( 5000 );
}

HServer::~HServer() {
	stop();
}

void	HServer::update() {
	world->save();
}
/*
void	HServer::incomingConnection( qintptr socketDescriptor ) {
	QTcpSocket* socket = new QTcpSocket();
	socket->setSocketDescriptor( socketDescriptor );
	addPendingConnection( socket );
	emit newConnection();
}
*/
void	HServer::start( QHostAddress ip, quint16 port ) {
	if ( !tcpServer.listen( ip, port ) ) {
        qWarning() << "Unable to start the server:" << tcpServer.errorString();
		return;
    }
	qDebug() << "Server is started. IP:" << ipString( tcpServer.serverAddress(), tcpServer.serverPort() );
	qDebug() << "Server IP's:";
	for ( auto &&i: QNetworkInterface::allAddresses() ) {
		if ( i.protocol() == QAbstractSocket::IPv4Protocol ) {
			qDebug() << "\t" << i.toString();
		}
	}
}

void	HServer::stop() {
	tcpServer.close();
	HThreadParent::stop();
	qDebug() << "Server is stopped.";
}

void	HServer::error( QAbstractSocket::SocketError socketError ) {
	qWarning() << "Socket error:" << socketError;
}

void	HServer::newConnect() {
    if ( !tcpServer.isListening() ) {
		return;
	}

	while ( tcpServer.hasPendingConnections() ) {
		auto socket = tcpServer.nextPendingConnection();	
		connect( socket, &QTcpSocket::destroyed, this, &HServer::socketDestroyed );
		auto conId = socket->socketDescriptor();
		connections[ conId ] = HConnectionPtr( new HConnection( socket, world ), &QObject::deleteLater );
		connect( connections[ conId ].data(), &HConnection::deleteConnection, this, &HServer::deleteConnection );
		if ( SOCKET_DEBUG ) {
			qDebug() << "New connection. ID:" << conId << "IP:" << ipString( socket->localAddress(), socket->localPort() );
		}
	}
}

void	HServer::deleteConnection( qintptr conId ) {
	connections.remove( conId );
}

void	HServer::socketDestroyed() {
	if ( SOCKET_DEBUG ) {
		qDebug() << "socketDestroyed";
	}
}