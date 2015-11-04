#include <QCoreApplication>
#include <QTextCodec>
#include "hserver.h"

#include <QDebug>
#include <iostream>
constexpr auto debugLevel = 0;

void	myMessageOutput( QtMsgType type, const QMessageLogContext &context, const QString &msg ) {
	QByteArray localMsg = msg.toLocal8Bit();
	switch ( type ) {
		case QtDebugMsg:
			if ( debugLevel < 1 )
				std::cerr << "[D]: " << localMsg.toStdString() << std::endl;
			break;
		case QtWarningMsg:
			if ( debugLevel < 2 )
				std::cerr << "[W]: " << localMsg.toStdString() << std::endl;
			break;
		case QtCriticalMsg:
			if ( debugLevel < 3 )
				std::cerr << "[C]: " << localMsg.toStdString() << std::endl;
			break;
		case QtFatalMsg:
			std::cerr << "[F]: " << localMsg.toStdString() << std::endl;
			//fprintf(stderr, "[F]: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			abort();
	}
}

int	main( int argc, char *argv[] ) {
	//qInstallMessageHandler( myMessageOutput );
	QCoreApplication a{ argc, argv };

	HServer server{ &a };
	QObject::connect( &a, &QCoreApplication::aboutToQuit, &server, &HServer::stop );
	server.start( QHostAddress::Any, 80 );
	//QTimer::singleShot( 3000, &server, &HServer::stop );

	return a.exec();
}


//const char *s = "Hello, world!";
//printf("%c\n", 3[s]); // e
//printf("%c\n", 5["Hello, world!"]); //,