#ifndef JSON_BASE_H
#define JSON_BASE_H

#include "json_variable_base.h"

#include <QByteArray>
#include <QString>
#include <QList>
#include <QVariant>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJSValue>

  //Handy name for my base object
#define J_BASE( x ) \
  public: \
  x( QString key = QString(), JsonBase* base = nullptr ) : \
    JsonBase( key, base ) {} \
  x( const x& other ) : JsonBase() { if ( !importJson( other.toJson() )) qDebug("Couldn't import json: %s", #x); } \
  x( QJsonObject obj ) : JsonBase() { if ( !importJson( obj )) qDebug("Couldn't import json object:, %s", #x); } \
  x( QJSValue obj ) : JsonBase() { if ( !importQJSValue( obj )) qDebug("Couldn't import json object:, %s", #x); } \
  x( QByteArray data ) : JsonBase() { if ( !importJsonData( data )) qDebug("Couldn't import json bytes: %s", data.data()); } \
  x operator=( x data ) { if ( !importJson( data.toJson() )) qDebug("Couldn't import json object: %s", #x); return *this; } \
  virtual ~x();

  //Define the json base object which smoothly moves data between objects
class JsonBase : public JsonVariableBase
{
  template <typename T> friend class JsonVariable;

  private:
    //Holds all my entries of objects
  QList<JsonVariableBase*> _entries;

  public:
    //Allow testing of myself
  JsonBase( QString key = QString(), JsonBase* base = nullptr );

    //Ensure we deconstruct correctly
  virtual ~JsonBase() {}

    //Handy function for importing data from a raw byte array
  bool importJsonData( QByteArray& data );
    //Handy function for exporting data in byte array format
  bool exportJsonData( QByteArray& data ) const;

    //Load a json variable into my collection list
  bool importJson( QJsonObject obj );
    //Return a json object with all updated values
  QJsonObject toJson() const;

    //Load a json variable into my collection list
  bool importQJSValue( QJSValue obj );
    //Return a json object with all updated values
  QJSValue exportQJSValue() const;

   //*** Imp for variable base
    //Return my key
  QString key();
    //Load a json variable into my collection list
  bool importValue( QJsonValue obj );
    //Return a json object with all updated values
  QJsonValue exportValue() const;
    //Reset all my local registered values
  void reset();
    //Debug out the data
  void debugDump( int padding = 0 ) const;

    //Register an object me
  void registerObject( JsonVariableBase* obj );
};

#endif // JSONBASE_H
