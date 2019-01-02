#include "json_variable_base.h"

JsonVariableBase::JsonVariableBase( QString key ) :
    _key( key )
{
}

//Base variable that can be used to read/write values
QString JsonVariableBase::key()
{
    return _key;
}
