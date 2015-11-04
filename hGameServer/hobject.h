#pragma once
#ifndef HOBJECT_H
#define HOBJECT_H

#include <QObject>
#include <QTimer>
#include <QPoint>
#include <QSize>
#include <QVariant>
#include <QMap>
#include <QThread>
#include "hworld.h"
#include "hthreadparent.h"

#include <QVector2D>
#include <QLine>

class HObject : public HThreadParent {
	Q_OBJECT

public:
	HObject( HWorldPtr _world, QString _name );
	virtual ~HObject();

protected:
	HWorldPtr				world;
	QString					name;
	GObjectType				type;

	QString					parent;
	QSizeF					size;
	QPointF					pos, gpos;
	qreal					mass, speed, angle;
	QVector2D				speedVec, otherSpeedVec;

	template<typename T> void	get( const QString & objectName, const QString & propertyName, T& dst ) {
		auto variant = world->get( objectName, propertyName );
		if ( variant.isValid() && variant.canConvert<T>() ) //&& !variant.isNull()
			dst = variant.value<T>();
	}

	template<typename T> void	get( const QString & propertyName, T& dst ) {
		get( name, propertyName, dst );
	}

public slots:
	virtual void			init();
	virtual void			stop();
	QVector2D				getParentAverage( const QString & name );
	void					updateParentProperties();

protected slots:
	virtual void			update();
	void					send( const QString & propertyName, const QVariant & value );

signals:
	void					remove( QString name );
	void					set( const QString & objectName, const QString & propertyName, const QVariant & value );
};

#endif // HOBJECT_H
