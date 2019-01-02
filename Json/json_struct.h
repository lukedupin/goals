// Generated file, please edit api.py in Scripts
#ifndef JSON_STRUCT_H
#define JSON_STRUCT_H

#include <Json/json_variable.h>

#define J( type, name ) type name = type( #name, this )


class JErr : public JsonBase
{
    J_BASE( JErr )

    J( JBool, successful );
    J( JString, reason );
};

class JEmpty : public JsonBase
{
    J_BASE( JEmpty )

};

class JGoal : public JsonBase
{
    J_BASE( JGoal )

    J( JString, goal );
    J( JInt, count );
};

class JStorage : public JsonBase
{
    J_BASE( JStorage )

    J( JArray<JGoal>, goals );
};

void apiStructRegister();

#endif
