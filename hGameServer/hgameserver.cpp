#include <QTimer>
#include <QThread>
#include "hgameserver.h"
#include "hthread.h"
#include <QDebug>

hGameServer::hGameServer( hWorldPtr _world, QString _name )
	: hObject( _world, _name ) {  
	connect( this, &hGameServer::set, world.data(), &hWorld::set );
	connect( this, &hGameServer::remove, world.data(), &hWorld::remove );
}

hGameServer::~hGameServer() { 
}

void	hGameServer::init() {
	for ( auto&& objectName : world->get() )
		if ( objectName != name )
			initObject( objectName );

	get( "time", time );

	t.start( 10 );
}

void hGameServer::update() {
	send( "time", time++ );
}

void	hGameServer::initObject( QString objectName ) {
	auto object = new hObject( world, objectName );

	addThread( object );

	connect( object, &hObject::set, this, &hGameServer::set );
	connect( object, &hObject::remove, this, &hGameServer::remove );
}