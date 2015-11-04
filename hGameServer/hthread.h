#pragma once
#include <QObject>
#include <QTimer>

class HThread : public QObject {
	Q_OBJECT

public:

					HThread( QObject * parent = nullptr );
	virtual			~HThread();
	virtual void	init();
	virtual void	prepare();
	virtual void	stop();
	virtual void	update();

	auto	isPrepared() const -> const bool;
	auto	isStopped() const -> const bool;
    
protected:
    QTimer	t;
	bool	prepared,
			stopped;

signals: 
	void	threadPrepared();
    void	threadStopped();

};