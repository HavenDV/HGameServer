#pragma once
#ifndef HGAMESERVER_H
#define HGAMESERVER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QVariant>
#include "hobject.h"
#include "hworld.h"

class hGameServer Q_DECL_FINAL : public hObject {
	Q_OBJECT

public:
	hGameServer( hWorldPtr _world, QString _name );
	~hGameServer();
	void		init() Q_DECL_FINAL;

private:
	qint64		time;

private slots:
	void		update() Q_DECL_FINAL;
	void		initObject( QString name );

};

#endif // HGAMESERVER_H
