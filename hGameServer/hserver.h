#pragma once
#ifndef HSERVER_H
#define HSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QSharedPointer>
#include <QTimer>
#include "hconnection.h"
#include "hworld.h"
#include "hgameserver.h"
#include "hthreadparent.h"

typedef QSharedPointer<hConnection> hConnectionPtr;

class HServer : public HThreadParent {
	Q_OBJECT

public:
	HServer( QObject *parent );
	~HServer();

public slots:
	void		start( QHostAddress ip, quint16 port );
	void		update();
	void		stop();

private:
	QTimer							t;
	QMap<qintptr, hConnectionPtr>	connections;
	hWorldPtr						world;
	QTcpServer						tcpServer;

//protected:
	//virtual void	incomingConnection( qintptr socketDescriptor );

private slots:
	void		newConnect();
	void		error( QAbstractSocket::SocketError socketError );
	void		deleteConnection( qintptr conId );
	void		socketDestroyed();
};

#endif // HSERVER_H
