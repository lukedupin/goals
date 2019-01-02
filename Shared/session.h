#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QString>

#include "mailbox.h"
#include "Json/json_struct.h"

//Session class
class Session : public QObject
{
    private:
    //Holds status
    bool _loaded = false;
    JStorage _storage;

    public:
    Session();
    virtual ~Session();

    Q_INVOKABLE QVariant storage();

    //Load the settings
    bool load();
    bool isLoaded();

    //Save the settings
    bool save( JStorage storage );
};

#endif
