#include <QTimer>
#include <QThread>
#include "hgameserver.h"
#include "hthread.h"
#include <QDebug>

HGameServer::HGameServer( HWorldPtr _world, QString _name )
	: HObject( _world, _name ) {  
	connect( this, &HGameServer::set, world.data(), &HWorld::set );
	connect( this, &HGameServer::remove, world.data(), &HWorld::remove );
}

HGameServer::~HGameServer() { 
}

void	HGameServer::init() {
	for ( auto&& objectName : world->get() )
		if ( objectName != name )
			initObject( objectName );

	get( "time", time );

	t.start( 10 );
}

void	HGameServer::update() {
	send( "time", time++ );
}

void	HGameServer::initObject( QString objectName ) {
	auto object = new HObject( world, objectName );

	addThread( object );

	connect( object, &HObject::set, this, &HGameServer::set );
	connect( object, &HObject::remove, this, &HGameServer::remove );
}