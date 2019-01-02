#include <Json/json_struct.h>

void apiStructRegister()
{
    qRegisterMetaType<JErr>( "JErr" );
    qRegisterMetaType<JEmpty>( "JEmpty" );
    qRegisterMetaType<JGoal>( "JGoal" );
    qRegisterMetaType<JStorage>( "JStorage" );
}

JErr::~JErr() {}
JEmpty::~JEmpty() {}
JGoal::~JGoal() {}
JStorage::~JStorage() {}
