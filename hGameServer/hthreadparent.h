#pragma once
#include "hthread.h"

class HThreadParent : public HThread {
	Q_OBJECT

public:
					HThreadParent( QObject * parent = nullptr );
	virtual			~HThreadParent();

	virtual void	prepare();
	virtual void	stop();

	void	addThread( HThread * object );
	void	waitPreparedThreads();
	void	waitThreads();
	void	threadPrepared();
	void	threadStopped();

private:
	size_t	notPreparedThreadCount,
			threadCount;

signals:
	void	prepareAll();
	void	stopAll();

};
