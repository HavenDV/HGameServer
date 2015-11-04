#pragma once
#ifndef HGAMESERVER_H
#define HGAMESERVER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QVariant>
#include "hobject.h"
#include "hworld.h"

class HGameServer Q_DECL_FINAL : public HObject {
	Q_OBJECT

public:
	HGameServer( HWorldPtr _world, QString _name );
	~HGameServer();
	void		init() Q_DECL_FINAL;

private:
	qint64		time;

private slots:
	void		update() Q_DECL_FINAL;
	void		initObject( QString name );

};

#endif // HGAMESERVER_H
