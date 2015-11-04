#include <QCoreApplication>
#include <QThread>
#include "hthreadparent.h"

HThreadParent::HThreadParent( QObject * parent ) :
	threadCount{ 0 }, notPreparedThreadCount{ 0 }, HThread{ parent } {};

HThreadParent::~HThreadParent() {}

void	HThreadParent::prepare() {
	t.stop();
	waitPreparedThreads();
	HThread::prepare();
}

void	HThreadParent::stop() {
	t.stop();
	waitThreads();
	HThread::stop();
}

void	HThreadParent::threadPrepared() {
	--notPreparedThreadCount;
}

void	HThreadParent::threadStopped() {
	--threadCount;
}

void	HThreadParent::waitPreparedThreads() {
	emit prepareAll();
	while ( notPreparedThreadCount > 0 ) {
		QCoreApplication::processEvents();
	}
}

void	HThreadParent::waitThreads() {
	emit stopAll();
	while ( threadCount > 0 ) {
		QCoreApplication::processEvents();
	}
}

void	HThreadParent::addThread( HThread* object ) {
	auto thread = new QThread();
	object->moveToThread( thread );

	connect( thread, &QThread::started, object, &HThread::init );
	connect( this, &HThreadParent::prepareAll, object, &HThread::prepare );
	connect( this, &HThreadParent::stopAll, object, &HThread::stop );
	connect( object, &HThread::threadPrepared, this, &HThreadParent::threadPrepared );
	connect( object, &HThread::threadStopped, thread, &QThread::quit );
	connect( object, &HThread::threadStopped, object, &HThread::deleteLater );
	connect( thread, &QThread::finished, thread, &QThread::deleteLater );
	connect( thread, &QThread::finished, this, &HThreadParent::threadStopped );

	++threadCount;
	++notPreparedThreadCount;

	thread->start();
}
