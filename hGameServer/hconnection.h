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

class HConnection : public QObject {
	Q_OBJECT

public:
	HConnection( QTcpSocket* _socket, HWorldPtr _world );
	~HConnection();

private:
	QTcpSocketPtr	socket;
	HWorldPtr		world;
	qintptr			id;

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
