#include "json_base.h"

#include <QJsonDocument>
#include <QJsonParseError>

JsonBase::JsonBase( QString key, JsonBase* parent ) : JsonVariableBase( key )
{
  if ( parent != nullptr )
    parent->registerObject( this );
}

bool JsonBase::importJsonData( QByteArray& data )
{
  QJsonParseError err;
  QJsonObject obj = QJsonDocument::fromJson( data, &err ).object();

    //Did this work?
  if ( err.error == QJsonParseError::NoError )
    return importJson( obj );

    //Dump the error
  qDebug("Json import error[%d]: %s",
         err.error, err.errorString().toUtf8().data());
  return false;
}

bool JsonBase::exportJsonData( QByteArray& data ) const
{
  QJsonDocument obj( toJson() );

    //Convert to byte array
  data = obj.toJson( QJsonDocument::Compact );
  return true;
}


bool JsonBase::importQJSValue(QJSValue js_value)
{
  bool result = true;

    //Rip through my entries, importing in values, missing values are left along
  for ( int i = 0; i < _entries.length(); i++ )
  {
    JsonVariableBase* obj = _entries[i];

      //If we don't have this key, reset the value and move on
    if ( !js_value.hasProperty( obj->key()))
    {
      obj->reset();
      continue;
    }

      //Load up the value
    if ( !obj->importQJSValue( js_value.property( obj->key() )))
    {
      qDebug("Failed to import key: %s", obj->key().toUtf8().data());
      result = false;
    }
  }

  return result;
}

QJSValue JsonBase::exportQJSValue() const
{
  QJSValue js_value;

      //Rip through my entries, importing in values
  for ( int i = 0; i < _entries.length(); i++ )
  {
    auto obj = _entries[i];
    js_value.setProperty( obj->key(), obj->exportQJSValue() );
  }

  return js_value;
}



bool JsonBase::importJson(QJsonObject json_obj)
{
  bool result = true;

    //Rip through my entries, importing in values, missing values are left along
  for ( int i = 0; i < _entries.length(); i++ )
  {
    JsonVariableBase* obj = _entries[i];

      //If we don't have this key, reset the value and move on
    if ( !json_obj.contains( obj->key()))
    {
      obj->reset();
      continue;
    }

      //Load up the value
    if ( !obj->importValue( json_obj[ obj->key()] ) )
    {
      qDebug("Failed to import key: %s", obj->key().toUtf8().data());
      result = false;
    }
  }

  return result;
}

QJsonObject JsonBase::toJson() const
{
  QJsonObject json_obj;

      //Rip through my entries, importing in values
  for ( int i = 0; i < _entries.length(); i++ )
  {
    auto&& obj = _entries[i];
    json_obj.insert( obj->key(), obj->exportValue() );
  }

  return json_obj;
}


QString JsonBase::key()
{
  return _key;
}

bool JsonBase::importValue(QJsonValue json_obj)
{
  return ( json_obj.isObject() )? importJson( json_obj.toObject()): false;
}

QJsonValue JsonBase::exportValue() const
{
  return QJsonValue( toJson());
}

void JsonBase::reset()
{
    //Reset all variables
  for ( int i = 0; i < _entries.length(); i++ )
    _entries[i]->reset();
}

void JsonBase::debugDump( int padding ) const
{
  if ( !_key.isEmpty() )
  {
    qDebug("%s%s: ", QString().fill(' ', padding).toUtf8().data(),
                     _key.toUtf8().data());
    padding += 2;
  }

    //Dump my keys
  for ( int i = 0; i < _entries.length(); i++ )
    _entries[i]->debugDump( padding );
}


//*** Private register object

void JsonBase::registerObject(JsonVariableBase *obj)
{
  _entries.append( obj );
}

