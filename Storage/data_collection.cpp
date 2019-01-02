#include "data_collection.h"
#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QRegExp>

#include <QUuid>

DataCollection::JMeta::~JMeta() {}

bool DataCollection::isValid()
{
    return !_collection.isEmpty();
}

void DataCollection::close()
{
    _collection = QString();
    _meta.count = 0;
    _meta.storage_count = 0;
}

//Return the meta
DataCollection::JMeta DataCollection::meta()
{
    return _meta;
}

int DataCollection::storageCount()
{
    return _meta.storage_count.value();
}

int DataCollection::entryCount()
{
    return _meta.count.value();
}

//List all collections
QList<QString> DataCollection::listCollections( QString collection )
{
    QString base = QString("%1/%2").
                        arg( QStandardPaths::standardLocations( QStandardPaths::AppDataLocation ).value(0)).
                        arg(collection);

    //Create the collection
    QDir path(base);
    return path.entryList( QDir::Dirs );
}

bool DataCollection::loadCollection( QString collection, QString uid )
{
    //Setup the collection
    if ( uid.isEmpty() )
    {
        uid = QUuid::createUuid().toString(QUuid::WithoutBraces);
        return false;
    }

    //Get the collection filename
    QString base = QString("%1/%2").
                        arg( QStandardPaths::standardLocations( QStandardPaths::AppDataLocation ).value(0)).
                        arg(collection);
    _collection = QString("%1/%2").
                        arg( base ).
                        arg(uid);

    //Create the collection
    QDir col(base);
    if (!col.exists())
        return false;

    //Create the collection
    QDir path(_collection);
    if (!path.exists())
        return false;

    //Does the meta already exist?  Load it and we're done
    return readMeta( _meta );
}


void DataCollection::createCollection( QString collection, QString uid )
{
    //Setup the collection
    if ( uid.isEmpty() )
        uid = QUuid::createUuid().toString(QUuid::WithoutBraces);

    //Get the collection filename
    QString base = QString("%1/%2").
                        arg( QStandardPaths::standardLocations( QStandardPaths::AppDataLocation ).value(0)).
                        arg(collection);
    _collection = QString("%1/%2").
                        arg( base ).
                        arg(uid);

    //Create the collection
    QDir col(base);
    if (!col.exists())
        col.mkpath(base);

    //Create the collection
    QDir path(_collection);
    if (!path.exists())
        path.mkpath(_collection);

    //Does the meta already exist?  Load it and we're done
    if ( readMeta( _meta ) )
        return;

    //Write out the meta data
    _meta.count = 0;
    _meta.storage_count = 0;
    _meta.updated_on = QDateTime::currentDateTimeUtc();
    _meta.created_on = QDateTime::currentDateTimeUtc();
    writeMeta( _meta );
}

//Delete data stores
int DataCollection::deleteData( int offset, int count )
{
    if ( !isValid() )
        return false;

    QDir dir( _collection );

    QJsonArray ary;
    QRegExp rx("^data_");

    int found = 0;

    //Setup and write out the data
    int idx = 0;
    for ( auto entry : dir.entryList( QDir::Files, QDir::Name ))
    {
        if ( rx.indexIn(entry) < 0 )
            continue;

         //Deal with offsets and then limits
        if ( idx++ < offset )
            continue;

        QString filename = QString("%1/%2").arg(_collection).arg(entry);

        //Load up the file
        auto&& file = QFile( filename );
        if ( !file.open(QIODevice::ReadOnly))
        {
            qDebug("DataCollection couldn't read: %s", filename.toUtf8().data());
            continue;
        }

        //Load up the data, so we can figure out how many points are in it, before we delete it
        auto&& json = QJsonDocument::fromJson( file.readAll() );
        file.close();

        //Reduce the count before we delete this file
        if ( json.isArray() )
            _meta.count -= json.array().count();

        if ( file.remove() )
            found++;

        //Have we found enough entries?
        if ( found >= count )
            break;
    }

    //Update the meta
    if ( _meta.count < 0 )
        _meta.count = 0;
    writeMeta( _meta );

    return found;

}

bool DataCollection::deleteCollection()
{
    if ( !isValid() )
        return false;

    QDir path(_collection);
    _collection = QString();

    //Remove everything
    auto result = path.removeRecursively();

    //Close and return our result
    close();
    return result;
}

QJsonObject DataCollection::readHeader()
{
    if ( !isValid() )
        return QJsonObject();

    QString filename = QString("%1/header.js").arg(_collection);

    //Load up the file
    auto&& file = QFile( filename );
    if ( !file.open(QIODevice::ReadOnly))
    {
        qDebug("No %s file found", filename.toUtf8().data());
        return QJsonObject();
    }

    auto&& json = QJsonDocument::fromJson( file.readAll() );
    if ( !json.isObject() )
    {
        qDebug("Invalid data found in %s", filename.toUtf8().data());
        return QJsonObject();
    }

    return json.object();
}

bool DataCollection::writeHeader( QJsonObject&& obj )
{
    if ( !isValid() )
        return false;

    QJsonDocument doc(obj);
    if ( !doc.isObject() )
        return false;

    QString filename = QString("%1/header.js").arg(_collection);

    //Open files
    auto&& file = QFile( filename );
    if ( !file.open(QIODevice::WriteOnly))
    {
        qDebug("Couldn't save to %s", filename.toUtf8().data());
        return false;
    }

    //Write out the data
    auto&& data = doc.toJson( QJsonDocument::Compact );
    return file.write( data ) == data.length();
}

QJsonArray DataCollection::readData( int offset, int limit )
{
    if ( !isValid() )
        return QJsonArray();

    QDir dir( _collection );

    QJsonArray ary;
    QRegExp rx("^data_");

    //Setup and write out the data
    int idx = 0;
    for ( auto entry : dir.entryList( QDir::Files, QDir::Name ))
    {
        //Ensure this is a data file, not header/meta etc.
        if ( rx.indexIn(entry) < 0 )
            continue;

        //Deal with offsets and then limits
        if ( idx++ < offset )
            continue;

        QString filename = QString("%1/%2").arg(_collection).arg(entry);

        //Load up the file
        auto&& file = QFile( filename );
        if ( !file.open(QIODevice::ReadOnly))
        {
            qDebug("DataCollection couldn't read: %s", filename.toUtf8().data());
            continue;
        }

        auto&& json = QJsonDocument::fromJson( file.readAll() );
        if ( !json.isArray() )
        {
            qDebug("DataCollection's %s isn't a json array", filename.toUtf8().data());
            continue;
        }

        //Load up the data
        auto&& tmp = json.array();
        for ( auto idx = 0; idx < tmp.count(); idx++ )
            ary.append( tmp[idx] );

        //Are we done?
        if ( --limit <= 0 )
            break;
    }

    return ary;
}

void DataCollection::forEachData( std::function<bool (QJsonArray& ary)> callback )
{
    if ( !isValid() )
        return;

    QDir dir( _collection );
    QRegExp rx("^data_");

    //Setup and write out the data
    for ( auto entry : dir.entryList( QDir::Files, QDir::Name ))
    {
        //Ensure this is a data file, not header/meta etc.
        if ( rx.indexIn(entry) < 0 )
            continue;

        QString filename = QString("%1/%2").arg(_collection).arg(entry);

        //Load up the file
        auto&& file = QFile( filename );
        if ( !file.open(QIODevice::ReadOnly))
        {
            qDebug("DataCollection couldn't read: %s", filename.toUtf8().data());
            continue;
        }

        //Read and convert the data
        auto&& json = QJsonDocument::fromJson( file.readAll() );
        if ( !json.isArray() )
        {
            qDebug("DataCollection's %s isn't a json array", filename.toUtf8().data());
            continue;
        }
        file.close();

        //Callback to the user, if the user says we should stop, then we will =)
        auto&& tmp = json.array();
        if ( !callback( tmp ) )
            return;
    }
}


bool DataCollection::writeData( QJsonArray entries )
{
    if ( !isValid() )
        return false;

    //Load the document
    QJsonDocument doc(entries);
    if ( !doc.isArray() )
        return false;

    //Setup and write out the data
    QString filename = QString("%1/data_%2.js").arg(_collection).arg(_meta.storage_count.value(), 4, 10, QChar('0'));

    //Open files
    auto&& file = QFile( filename );
    if ( !file.open(QIODevice::WriteOnly))
    {
        qDebug("Couldn't save to %s in data collection %s", filename.toUtf8().data(), _collection.toUtf8().data());
        return false;
    }

    //Update the meta
    _meta.storage_count++;
    _meta.count += entries.count();
    _meta.updated_on = QDateTime::currentDateTimeUtc();
    writeMeta( _meta );

    //Write out the data to this data file
    auto&& data = doc.toJson( QJsonDocument::Compact );
    return file.write( data ) == data.length();
}

bool DataCollection::writeMeta( JMeta& meta )
{
     //Open files
    auto&& file = QFile( QString("%1/meta.js").arg(_collection) );
    if ( !file.open(QIODevice::WriteOnly))
    {
        qDebug("Couldn't save to meta.js in data collection %s", _collection.toUtf8().data());
        return false;
    }

    //Write the meta data
    auto&& meta_doc = QJsonDocument(meta.toJson());
    auto&& meta_data = meta_doc.toJson( QJsonDocument::Compact );
    file.write( meta_data );

    return true;
}

bool DataCollection::readMeta( JMeta& meta )
{
     //Open files
    auto&& file = QFile( QString("%1/meta.js").arg(_collection) );
    if ( !file.open(QIODevice::ReadOnly))
    {
        qDebug("Couldn't load to meta.js in data collection %s", _collection.toUtf8().data());
        return false;
    }

    //Write the meta data
    auto&& data = file.readAll();
    return meta.importJsonData( data );
}
