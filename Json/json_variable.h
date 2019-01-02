#ifndef JSON_VARIABLE_H
#define JSON_VARIABLE_H

#include "json_base.h"

#include <QList>
#include <QString>
#include <QDateTime>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector>

#define OPS( T ) \
    T& value(); \
    T& operator=( T v ); \
    operator const T&(void) const; \
    operator T&(void); \
    const T* operator->(); \
    QJsonValue toJson(); \
    bool importQJSValue( QJSValue val ); \
    QJSValue exportQJSValue() const; \
    bool importValue( QJsonValue val ); \
    QJsonValue exportValue() const

#define ASSIGN( T ) \
    T& operator=( const T& r )

  //Define my default type classes
class JInt : public JsonVariableBase
{
    int _value;

    public:
    static JInt create( int v );
    JInt( QString key = QString(), JsonBase* base = nullptr, int v = 0 );
    void reset();

    OPS( int );
    ASSIGN( JInt );

    void debugDump( int padding = 0 ) const;
};

  //Define my default type classes
class JDouble : public JsonVariableBase
{
    double _value;

    public:
    static JDouble create( double v );
    JDouble( QString key = QString(), JsonBase* base = nullptr, double v = 0 );
    void reset();

    OPS( double );
    ASSIGN( JDouble );

    void debugDump( int padding = 0 ) const;
};

  //Define my default type classes
class JBool : public JsonVariableBase
{
    bool _value;

    public:
    static JBool create( bool v );
    JBool( QString key = QString(), JsonBase* base = nullptr, bool v = false );
    void reset();

    OPS( bool );
    ASSIGN( JBool );

    void debugDump( int padding = 0 ) const;
};

  //Define my default type classes
class JString : public JsonVariableBase
{
    QString _value;

    public:
    static JString create( const char* v );
    static JString create( QString v );
    JString( QString key = QString(), JsonBase* base = nullptr, QString v = QString::fromUtf8("") );
    void reset();

    void debugDump( int padding = 0 ) const;

    OPS( QString );
    ASSIGN( JString );
};

  //Define my default type classes, milla second
class JDateTime : public JsonVariableBase
{
    QDateTime _value;

    public:
    static JDateTime create( QDateTime v );
    JDateTime( QString key = QString(), JsonBase* base = nullptr, QDateTime v = QDateTime() );
    void reset();

    qint64 ms() const;

    OPS( QDateTime );
    ASSIGN(JDateTime );

    void debugDump( int padding = 0 ) const;
};


  //Define my default type classes
template <typename A>
class JArray : public JsonVariableBase
{
    QList<A> _value;

    public:
    JArray( QString key = "empty", JsonBase* base = nullptr ) :
            JsonVariableBase( key )
    {
        if ( base != nullptr )
            base->registerObject( this );
    }

    //Need to reclaim my data
    ~JArray()
    {
        reset();
    }

    bool importValue( QJsonValue val )
    {
        reset();

        QJsonArray ary = val.toArray();
        for ( int i = 0; i < ary.count(); i++ )
        {
            A a( QString::number(i) );
            a.importValue( ary[i] );
            this->_value.push_back( a );
        }
        return val.isArray();
    }

    QJsonValue exportValue() const
    {
        QJsonArray ary;
        for ( int i = 0; i < this->_value.count(); i++ )
            ary.append( this->_value[i].exportValue() );

        return QJsonValue( ary );
    }

    bool importQJSValue( QJSValue ary )
    {
        reset();

        if ( !ary.isArray() )
            return false;

        const int length = ary.property("length").toInt();
        for ( int i = 0; i < length; i++ )
        {
            A a( QString::number(i) );
            a.importQJSValue( ary.property(i) );
            this->_value.push_back( a );
        }
        return true;
    }

    QJSValue exportQJSValue() const
    {
        QJSValue ary;
        ary.setProperty("length", this->_value.count() );
        for ( int i = 0; i < this->_value.count(); i++ )
            ary.setProperty( i, this->_value[i].exportQJSValue() );

        return ary;
    }

    void clear()
    { reset(); }

    void reset()
    {
        //for ( int i = 0; i < this->_value.count(); i++ ) delete this->_value[i];
        this->_value.clear();
    }

    bool removeAt( int i )
    {
        //Is the index even valid?
        if ( i < 0 || i >= this->_value.count() )
            return false;

        //Remove the item and finish
        this->_value.removeAt( i );
        return true;
    }

    int count() const
    {
        return this->_value.count();
    }

    int length() const
    {
        return this->_value.length();
    }

    JArray<A> append( A obj )
    {
        A a( QString::number(this->_value.length()));
        a.importValue( obj.toJson() );
        this->_value.append( a );
        return *this;
    }

    /*
    void append( A* const &obj )
    {
        this->_value.append( obj );
    }
    */

    QList<A> toList() const
    {
        QList<A> tmp;
        for ( int i = 0; i < this->_value.length(); i++ )
            tmp.append( this->_value[i] );

        return tmp;
    }

    QVector<A> toVector() const
    {
        QVector<A> tmp;
        for ( int i = 0; i < this->_value.length(); i++ )
            tmp.append( this->_value[i] );

        return tmp;
    }

    const A* operator[]( int idx ) const
    {
        return &this->_value.at( (idx >= 0)? idx: count() + idx );
    }

    void debugDump( int padding = 0 ) const
    {
        if ( !this->_key.isEmpty() )
            qDebug("%s%s: ", QString().fill(' ', padding).toUtf8().data(),
                             this->_key.toUtf8().data());
        for ( int i = 0; i < this->_value.count(); i++ )
            this->_value[i].debugDump( padding + 2 );
    }
};

#undef OPS
#undef ASSIGN

#endif
