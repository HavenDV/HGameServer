#include <QSettings>
#include "hworld.h"

#include <QPoint>
#include <QSize>
#include <QDebug>

const auto settingsFileName = "settings.ini";
const auto settingsFormat = QSettings::IniFormat;

HWorld::HWorld( QObject* parent ) : QObject{ parent } {
	load();
	reinit();
}

void	HWorld::save() {
	QSettings settings{ settingsFileName, settingsFormat };
	settings.clear();

	settings.beginWriteArray("objects");
	auto i = 0;
	for ( auto && objectName : objects.keys() ) {
		settings.setArrayIndex( i++ );
		settings.setValue( "name", objectName );
	}
	settings.endArray();

	for ( auto && objectName : objects.keys() ) {
		settings.beginGroup( objectName );
		settings.beginWriteArray( "properties" );
		auto i = 0;
		for ( auto &&propertyName : objects[ objectName ].keys() ) {
			settings.setArrayIndex( i++ );
			settings.setValue( "name", propertyName );
			settings.setValue( "value", objects[ objectName ][ propertyName ] );
		}
		settings.endArray();
		settings.endGroup();
	}
}

void	HWorld::load() {
	QSettings settings{ settingsFileName, settingsFormat };
	GNames objectNames;

	auto size = settings.beginReadArray( "objects" );
	for ( auto i = 0; i < size; i++ ) {
		settings.setArrayIndex( i );
		objectNames.append( settings.value( "name" ).toString() );
	}
	settings.endArray();

	for ( auto &&objectName : objectNames ) {
		settings.beginGroup( objectName );
		auto size = settings.beginReadArray( "properties" );
		for ( auto i = 0; i < size; i++ ) {
			settings.setArrayIndex( i );
			auto propertyName = settings.value( "name" ).toString();
			objects[ objectName ][ propertyName ] = settings.value( "value" );
		}
		settings.endArray();
		settings.endGroup();
	}
}

void	HWorld::reinit() {
	QWriteLocker locker{ &mutex };

	objects.clear();
	objects[ "server" ][ "type" ] = GObjectType::Server;
	objects[ "server" ][ "time" ] = qlonglong( 0 );

	objects[ "sunSystem" ][ "type" ] = GObjectType::System;
	objects[ "sunSystem" ][ "size" ] = QSizeF( 5000.0, 5000.0 );
	objects[ "sunSystem" ][ "pos" ] = QPointF( 2500.0, 2500.0 );

	objects[ "sun" ][ "type" ] = GObjectType::Planet;
	objects[ "sun" ][ "parent" ] = "sunSystem";
	objects[ "sun" ][ "size" ] = QSizeF( 526.0, 300.0 );
	objects[ "sun" ][ "pos" ] = QPointF( 0.0, 0.0 );
	objects[ "sun" ][ "mass" ] = qreal( 333000.0 );
	objects[ "sun" ][ "speed" ] = qreal( 0.0 );
	objects[ "sun" ][ "angle" ] = qreal( 0.0 );

	objects[ "earth" ][ "type" ] = GObjectType::Planet;
	objects[ "earth" ][ "parent" ] = "sun";
	objects[ "earth" ][ "size" ] = QSizeF( 200.0, 200.0 );
	objects[ "earth" ][ "pos" ] = QPointF( 600.0, 0.0 );
	objects[ "earth" ][ "mass" ] = qreal( 1.0 );
	objects[ "earth" ][ "speed" ] = qreal( 1.3 );
	objects[ "earth" ][ "angle" ] = qreal( -90.0 );

	objects[ "moon" ][ "type" ] = GObjectType::Planet;
	objects[ "moon" ][ "parent" ] = "earth";
	objects[ "moon" ][ "size" ] = QSizeF( 20.0, 20.0 );
	objects[ "moon" ][ "pos" ] = QPointF( 1.1, 0.0 );
	objects[ "moon" ][ "mass" ] = qreal( 0.01 );
	objects[ "moon" ][ "speed" ] = qreal( 0.03 );
	objects[ "moon" ][ "angle" ] = qreal( -90.0 );
}

void	HWorld::set( const QString & objectName, const QString & propertyName, const QVariant & value ) {
	QWriteLocker locker{ &mutex };
	objects[ objectName ][ propertyName ] = value;
}

const QVariant	HWorld::get( const QString & objectName, const QString & propertyName ) {
	QReadLocker locker{ &mutex };
	return objects[ objectName ][ propertyName ];
}

const GObject	HWorld::get( const QString & objectName ) {
	QReadLocker locker{ &mutex };
	return objects[ objectName ];
}

const GNames	HWorld::get() {
	QReadLocker locker{ &mutex };
	return objects.keys();
}

void			HWorld::remove( const QString & objectName ) {
	QWriteLocker locker( &mutex );
	objects.remove( objectName );
}