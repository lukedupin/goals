#ifndef DATA_COLLECTION_H
#define DATA_COLLECTION_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QDir>

#include <Json/json_struct.h>
#include <functional>

//Allows the user to storage arrays of json objects
class DataCollection
{
    QString _collection;

    public:
    //Meta information for a collection
    class JMeta : public JsonBase
    {
        J_BASE( JMeta )

        J( JInt, count );
        J( JInt, storage_count );
        J( JDateTime, updated_on );
        J( JDateTime, created_on );
    };

    private:
    JMeta _meta;

    public:
    //List all collections
    static QList<QString> listCollections( QString collection );

    //Set the collectio name
    bool isValid();

    //Close the capture
    void close();

    //Return the meta data
    JMeta meta();

    //Number of collections
    int storageCount();

    //Number of points
    int entryCount();

    //Set the collectio name
    bool loadCollection( QString collection, QString uid = QString() );

    //Set the collectio name
    void createCollection( QString collection, QString uid = QString() );

    //Delete data stores
    int deleteData( int offset, int count );

    //Delete the collection
    bool deleteCollection();

    //Read in the header of the collection
    QJsonObject readHeader();

    //Write out the header
    bool writeHeader( QJsonObject&& obj );

    //Write out data entries
    QJsonArray readData( int offset = 0, int limit = -1 );

    //Write out data entries
    void forEachData( std::function<bool (QJsonArray& ary)> callback );

    //Write out data entries
    bool writeData( QJsonArray entries );

    private:
    //write meta
    bool writeMeta( JMeta& meta );

    //write meta
    bool readMeta( JMeta& meta );
};

#endif
