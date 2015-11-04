#include <QTimer>
#include <QPoint>
#include <QSize>
#include "hconnection.h"

const QByteArray http200Ok =	"HTTP/1.1 200 Ok\r\n"
								"Content-Type: text/html; charset=\"utf-8\"\r\n"
								"Connection: Keep-Alive\r\n"
								"\r\n";

QString		rus( const char* in ) {
	return QString::fromLocal8Bit( in );
	//return QStringLiteral( "" );
}

hConnection::hConnection( QTcpSocket* _socket, hWorldPtr _world ) : 
					socket( _socket, &QObject::deleteLater ),
					world( _world ) { 
	id = socket->socketDescriptor();
	connect( socket.data(), &QTcpSocket::readyRead, this, &hConnection::read );
	connect( socket.data(), &QTcpSocket::bytesWritten, this, &hConnection::bytesWritten );
}

void	hConnection::bytesWritten( qint64 size ) {
	if ( SOCKET_DEBUG ) qDebug() << "Socket bytes written: " << size << ". ID:" << id;
	close();
}

hConnection::~hConnection() {
	socket->close();
	if ( SOCKET_DEBUG ) qDebug() << "Socket closed. ID:" << id;
}

void	hConnection::read() {
	if ( socket->state() != QAbstractSocket::ConnectedState ) 
		return;
    auto socket = static_cast<QTcpSocket*>( sender() );

	if ( SOCKET_DEBUG ) qDebug() << "New data from socket ID:" << id;
	auto data = socket->readAll();
	auto splittedData = data.split(' ');
	if ( splittedData.size() < 2 ) return send();
	if ( SOCKET_DEBUG ) qDebug() << "Method: " << splittedData[0];
	auto list = splittedData[1].split('/');
	if ( list.size() < 3 ) return send();
	if ( SOCKET_DEBUG ) qDebug() << "Func: " << list[1];
	if ( SOCKET_DEBUG ) qDebug() << "Arg: " << list[2];
	if ( list[1] == "getObjectPos" )
		sendObjectPos( list[2] );
	else
		send();
}

void	hConnection::sendObjectPos( const QString & name ) {
	auto pos = world->get( name, "pos" ).toPointF();
	auto size = world->get( name, "size" ).toSizeF();
	auto time = world->get( "server", "time" ).toLongLong();
	QTextStream stream( socket.data() );
    stream.setAutoDetectUnicode( true );
    stream <<	http200Ok <<
				"<x>" << pos.x() - size.width()/2 << "</x>"
				"<y>" << pos.y() - size.height()/2 << "</y>"
				"<w>" << size.width() << "</w>"
				"<h>" << size.height() << "</h>"
				"<t>" << time << "</t>"
				"<s>" << time/2000 << "</s>";
	if ( SOCKET_DEBUG ) qDebug() << "Send object: " << name << " properties to socket ID:" << id;
	//qDebug() << QThread::currentThreadId();
}

void	hConnection::send() {
	QTextStream stream( socket.data() );
	stream.setCodec( "UTF-8" );
    stream << http200Ok <<
		  "<html><head>\n"
		  "<link rel='stylesheet' href='//game.havendv.ru/styles.css'>"
		  "<script src='//ajax.googleapis.com/ajax/libs/jquery/2.1.4/jquery.min.js'></script>"
		  "<script src='//game.havendv.ru/jquery.mousewheel.min'></script>"
		  "<script src='//cdn.jsdelivr.net/jquery.scrollto/2.1.0/jquery.scrollTo.min.js'></script>"
		  "<script src='//game.havendv.ru/js.js'></script>"
		  "</head><body>\n"
		  "<div class='ui'><h1>Nothing to see here</h1>\n"
		  "<h2>Server time: <span id='serverTime'></span>. Server step <span id='serverStep'></span></h2></div>\n"
		  "<div class='stars'>"
			"<div class='obj' id='earth'>"
				"<div class='info'>"
					"<h3 class='name'>Earth</h3>"
					"Size: <span class='size'>4242</span><br>"
					"Gen: <span class='gen'>4442</span><br>"
					+ rus( "Раса: <span class='race'>Люди</span><br>" ) +
				"</div>"
				"<img class='obj planet' src='//img.havendv.ru/game/earth.gif'>"
			"</div>"
			"<div class='obj' id='moon'>"
				"<img class='obj planet' src='//img.havendv.ru/game/earth.gif'>"
			"</div>"
			"<div class='obj' id='sun'>"
				"<img class='obj sun' src='//img.havendv.ru/game/sun.gif'>"
			"</div>"
		  "</div>"
		  "<div class='results'></div>"
		  "</body></html>";
	if ( SOCKET_DEBUG ) qDebug() << "Send data to socket ID:" << id;
}

void	hConnection::close() {
	emit deleteConnection( id );
}