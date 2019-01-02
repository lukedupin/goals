#include "json_variable.h"

#define OPS( R, T ) \
    T& R::value() { return _value; } \
    T& R::operator=( T v )  {  _value = v; return _value; } \
    R::operator const T&(void) const  { return _value; } \
    R::operator T&(void)  {  return _value; } \
    const T* R::operator->() { return &_value; } \
    QJsonValue R::toJson() { return exportValue(); }

#define ASSIGN( T ) \
    T& T::operator=( const T& r ) { _value = r._value; return *this; }

JInt JInt::create( int v ) { return JInt( "", nullptr, v ); }
JInt::JInt( QString key, JsonBase* base, int v ) :
    JsonVariableBase( key ),
    _value( v )
{
    if ( base != nullptr )
        base->registerObject( this );
}

bool JInt::importValue( QJsonValue val )
{
    //Are we actually a string?
    if ( val.isString() )
    {
        bool result = false;
        _value = val.toString().toInt( &result );
        return result;
    }

    //Alexporty a number, return the value
    _value = val.toInt();
    return val.isDouble();
}
QJsonValue JInt::exportValue() const
{ return QJsonValue( _value ); }

bool JInt::importQJSValue( QJSValue val )
{
    //Are we actually a string?
    if ( val.isString() )
    {
        bool result = false;
        _value = val.toString().toInt( &result );
        return result;
    }

    //Alexporty a number, return the value
    _value = val.toInt();
    return val.isNumber();
}
QJSValue JInt::exportQJSValue() const
{ return QJSValue( _value ); }

void JInt::reset()
{ _value = 0; }

void JInt::debugDump( int padding ) const
{
    qDebug("%s%s: %d", QString().fill(' ', padding).toUtf8().data(),
                                         _key.toUtf8().data(), _value);
}

OPS( JInt, int )
ASSIGN( JInt )

//Define JDouble
JDouble JDouble::create( double v ) { return JDouble( "", nullptr, v ); }
JDouble::JDouble( QString key, JsonBase* base, double v ) :
    JsonVariableBase( key ),
    _value( v )
{
    if ( base != nullptr )
        base->registerObject( this );
}
bool JDouble::importValue( QJsonValue val )
{
    //Are we actually a string?
    if ( val.isString() )
    {
        bool result = false;
        _value = val.toString().toDouble( &result );
        return result;
    }

    //Alexporty a number, return the value
    _value = val.toDouble();
    return val.isDouble();
}
QJsonValue JDouble::exportValue() const
{ return QJsonValue( _value ); }

bool JDouble::importQJSValue( QJSValue val )
{
    //Are we actually a string?
    if ( val.isString() )
    {
        bool result = false;
        _value = val.toString().toDouble( &result );
        return result;
    }

    //Alexporty a number, return the value
    _value = val.toNumber();
    return val.isNumber();
}
QJSValue JDouble::exportQJSValue() const
{ return QJSValue( _value ); }


void JDouble::reset()
{ _value = 0; }

void JDouble::debugDump( int padding ) const
{
    qDebug("%s%s: %f", QString().fill(' ', padding).toUtf8().data(),
                                         _key.toUtf8().data(), _value);
}

OPS( JDouble, double );
ASSIGN( JDouble );

//Bool
JBool JBool::create( bool v ) { return JBool("", nullptr, v ); }
JBool::JBool( QString key, JsonBase* base, bool v ) :
            JsonVariableBase( key ),
            _value( v )
{
    if ( base != nullptr )
        base->registerObject( this );
}
bool JBool::importValue( QJsonValue val )
{
    _value = val.toBool();
    return val.isBool();
}
QJsonValue JBool::exportValue() const
{ return QJsonValue( _value ); }
void JBool::reset()
{ _value = false; }

void JBool::debugDump( int padding ) const
{
    qDebug("%s%s: %s", QString().fill(' ', padding).toUtf8().data(),
                                         _key.toUtf8().data(),
                                         (_value)? "true": "false");
}

OPS( JBool, bool );
ASSIGN( JBool );

bool JBool::importQJSValue( QJSValue val )
{
    //Are we actually a string?
    if ( val.isString() )
    {
        _value = val.toString().toLower() == "true";
        return true;
    }

    //Alexporty a number, return the value
    _value = val.toBool();
    return val.isBool();
}
QJSValue JBool::exportQJSValue() const
{ return QJSValue( _value ); }

//Define my default type classes
JString JString::create( const char* v ) { return JString("", nullptr, QString::fromUtf8(v) ); }
JString JString::create( QString v ) { return JString("", nullptr, v ); }
JString::JString( QString key, JsonBase* base, QString v ) :
    JsonVariableBase( key ),
    _value( v )
{
    if ( base != nullptr )
        base->registerObject( this );
}
bool JString::importValue( QJsonValue val )
{
    _value = val.toString();
    return val.isString();
}
QJsonValue JString::exportValue() const
{ return QJsonValue( _value ); }

bool JString::importQJSValue( QJSValue val )
{
    _value = val.toString();
    return val.isString();
}
QJSValue JString::exportQJSValue() const
{ return QJSValue( _value ); }

void JString::reset()
{ _value = QString::fromUtf8(""); }

void JString::debugDump( int padding ) const
{
    qDebug("%s%s: %s", QString().fill(' ', padding).toUtf8().data(),
                       _key.toUtf8().data(),
                       _value.toUtf8().data());
}

OPS( JString, QString );
ASSIGN( JString );


  //Define my default type classes, milla second
JDateTime JDateTime::create( QDateTime v ) { return JDateTime("", nullptr, v ); }
JDateTime::JDateTime( QString key, JsonBase* base, QDateTime v ) :
    JsonVariableBase( key ),
    _value( v )
{
    if ( base != nullptr )
        base->registerObject( this );
}
bool JDateTime::importValue( QJsonValue val )
{
    _value = QDateTime::fromMSecsSinceEpoch( (qint64)val.toDouble());
    return val.isDouble();
}
QJsonValue JDateTime::exportValue() const
{ return QJsonValue( (double)_value.toMSecsSinceEpoch() ); }

bool JDateTime::importQJSValue( QJSValue val )
{
    if ( val.isNumber() )
    {
        _value = QDateTime::fromMSecsSinceEpoch( val.toNumber() );
        return true;
    }

    _value = val.toDateTime();
    return val.isString();
}
QJSValue JDateTime::exportQJSValue() const
{ return QJSValue( (double)_value.toMSecsSinceEpoch() ); }

void JDateTime::reset()
{ _value = QDateTime(); }

qint64 JDateTime::ms() const
{
  return _value.toMSecsSinceEpoch();
}

void JDateTime::debugDump( int padding ) const
{
    qDebug("%s%s: %s", QString().fill(' ', padding).toUtf8().data(),
                       _key.toUtf8().data(),
                       _value.toString().toUtf8().data());
}

OPS( JDateTime, QDateTime )
ASSIGN( JDateTime )
