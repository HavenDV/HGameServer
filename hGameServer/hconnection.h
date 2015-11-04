#pragma once
#ifndef HCONNECTION_H
#define HCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QSharedPointer>
#include <QThread>
#include "hworld.h"

#define SOCKET_DEBUG false

typedef QSharedPointer<QTcpSocket>	QTcpSocketPtr;

class hConnection : public QObject {
	Q_OBJECT

public:
	hConnection( QTcpSocket* _socket, hWorldPtr _world );
	~hConnection();

private:
	QTcpSocketPtr	socket;
	qintptr			id;
	hWorldPtr		world;

private slots:
	void		bytesWritten( qint64 size );
	void		read();
	void		send();
	void		close();
	void		sendObjectPos( const QString & name );
signals:
	void		deleteConnection( qintptr id );
};

#endif // HCONNECTION_H
