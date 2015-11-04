#include "hthread.h"

HThread::HThread( QObject * parent ) :
	QObject{ parent }, t{ this }, prepared{ false }, stopped{ false } {
	connect( &t, &QTimer::timeout, this, &HThread::update );
}

HThread::~HThread() {}

void	HThread::init() {}

void	HThread::prepare() {
	t.stop();
	prepared = true;
	emit threadPrepared();
}

void	HThread::stop() {
	stopped = true;
	emit threadStopped();
}

void	HThread::update() {}

auto	HThread::isPrepared() const -> const bool {
	return prepared;
}

auto	HThread::isStopped() const -> const bool {
	return stopped;
}