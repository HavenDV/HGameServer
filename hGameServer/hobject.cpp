#include <QDateTime>
#include <qmath.h>
#include "hobject.h"
#include <QDebug>
#include <QSettings>

inline qreal	toRadian( qreal value ) {
	return 2.0 * M_PI * value / 360.0;
}

void operator += ( QPointF& pos, const QVector2D& vec ) {
	pos.rx() += vec.x();
	pos.ry() += vec.y();
}

HObject::HObject( HWorldPtr _world, QString _name )
	: world( _world ), name( _name ) {}

HObject::~HObject() {}

auto	HObject::getParentAverage( const QString & objectName ) -> QVector2D {
	if ( objectName.isNull() || objectName == name ) {
		return QVector2D( 0.0F, 0.0F );
	}

	QPointF objectPos;
	qreal objectMass;
	uint objectType;
	get( objectName, "pos", objectPos );
	get( objectName, "mass", objectMass );
	get( objectName, "type", objectType );

	if ( objectMass < 0.01 || objectPos.isNull() || objectType != GObjectType::Planet ) {
		return QVector2D( 0.0F, 0.0F );
	}

	QLineF toParent( gpos, objectPos );
	auto distance = toParent.length();
	auto F = 0.003 * objectMass / ( distance * distance );

	return -F * QVector2D( gpos - objectPos ).normalized();
}

void	HObject::updateParentProperties() {
	for ( auto && objectName : world->get() ) {
		otherSpeedVec += getParentAverage( objectName );
	}
}

void	HObject::init() {
	send( "init", true );

	get( "type", type );
	get( "parent", parent );
	get( "size", size );
	get( "pos", pos );
	get( "mass", mass );
	get( "speed", speed );
	get( "angle", angle );

	if ( !parent.isNull() ) {
		QPointF parentPos;
		get( parent, "pos", parentPos );
		gpos = pos + parentPos;
	}

	speedVec.setX( qCos( toRadian( angle ) ) * speed );
	speedVec.setY( -qSin( toRadian( angle ) ) * speed );

	if ( type != GObjectType::System ) {
		t.start( 10 );
	}
}

void	HObject::stop() {
	HThreadParent::stop();
	emit remove( name );
}

void	HObject::send( const QString & propertyName, const QVariant & value ) {
	emit set( name, propertyName, value );
}

void	HObject::update() {
	auto currentTime = QTime::currentTime();
	auto currentMsec = 1000 * currentTime.second() + currentTime.msec();

	updateParentProperties();

	pos += otherSpeedVec + speedVec;
	gpos = pos;
	if ( !parent.isNull() ) {
		QPointF parentPos;
		get( parent, "pos", parentPos );
		gpos += parentPos;
	}

	send( "pos", gpos );
	//qDebug() << gpos;
}