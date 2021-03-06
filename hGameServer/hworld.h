#pragma once
#include <QObject>
#include <QVariant>
#include <QMap>
#include <QSharedPointer>
#include <QReadWriteLock>

typedef QMap< QString, QVariant >	GObject;
typedef QMap< QString, GObject >	GObjects;
typedef QList< QString >			GNames;

enum GObjectType {
	None,
	Asteroid,
	Planet,
	LivePlanet,
	System,
	Server,
	Ship
};
Q_DECLARE_METATYPE( GObjectType );

class HWorld :	public QObject {
	Q_OBJECT

public:
	HWorld( QObject* parent = 0 );

private:
	QReadWriteLock	mutex;
	GObjects		objects;

public slots:
	void			save();
	void			load();
	void			reinit();

	void					set( const QString & objectName, const QString & propertyName, const QVariant& value );
	void					remove( const QString & objectName );
	const QVariant			get( const QString & objectName, const QString & propertyName );
	const GObject			get( const QString & objectName );
	const GNames			get();
};

typedef QSharedPointer< HWorld > HWorldPtr;


/*
#include <QSharedData>
#include <QSharedDataPointer>
class hWorldData : public QSharedData {
	public:
		hWorldData() { }
		//hWorldData( const hWorldData &other )
		//	: QSharedData( other ), objectProperties( other.objectProperties ) { }
		~hWorldData() { }

		QMap<QString, QMap<QString, QVariant>>	objectProperties;
};

QSharedDataPointer<hWorldData>	data;
data( new hWorldData() ), 
 : data( other.data )
*/