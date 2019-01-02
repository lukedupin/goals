#include "session.h"

#include <QJsonDocument>
#include <QStandardPaths>
#include <QDir>

static const char* filename = "settings.json";

Session::Session()
{}

Session::~Session()
{}

QVariant Session::storage()
{
    return _storage.toJson();
}

//Setup the session object
bool Session::load()
{
    _loaded = false;

    QString path = QStandardPaths::standardLocations( QStandardPaths::AppDataLocation ).value(0);
    qDebug("Storage path: %s", path.toUtf8().data());

    //iOS happy
    QDir myDir(path);
    if (!myDir.exists()) {
        myDir.mkpath(path);
    }
    QDir::setCurrent(path);

    //Load up the file
    auto&& file = QFile( filename );
    if ( !file.open(QIODevice::ReadOnly))
    {
        qDebug("No %s file found", filename);
        return _loaded;
    }

    auto&& json = QJsonDocument::fromJson( file.readAll() );
    if ( !json.isObject() )
    {
        qDebug("Invalid data found in %s", filename);
        return _loaded;
    }

    //Load up data
    _loaded = _storage.importJson( json.object() );
    if ( !_loaded )
        qDebug("Couldn't map %s", filename);

    return _loaded;
}

bool Session::isLoaded()
{
    return _loaded;
}

bool Session::save( JStorage storage )
{
    QByteArray data;
    if ( !storage.exportJsonData( data ) )
        return false;

    //Save my storage
    _storage.goals.clear();
    for ( auto& goal: storage.goals.toList() )
        _storage.goals.append( goal );

    //Open files
    auto&& file = QFile( filename );
    if ( !file.open(QIODevice::WriteOnly))
    {
        qDebug("Couldn't save to %s", filename);
        return false;
    }

    //Write out the data
    _loaded = true;
    return file.write( data.data(), data.length() ) == data.length();
}
