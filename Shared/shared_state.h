#ifndef SHARED_STATE_H
#define SHARED_STATE_H

#include <Shared/mailbox.h>
#include <Shared/session.h>
#include <UI/ui_common.h>

#include <QObject>

//Holds the shared state
class SharedState : public QObject
{
    Q_OBJECT

    public:
    static const int MAJOR = 1;
    static const int MINOR = 0;

    private:

    public:
    Mailbox* Mail = nullptr;
    Session* Sess = nullptr;
    UiCommon* Ui = nullptr;

    //The current Server connection status
    QString CurrentRadiusUid;

    //Create a shared state
    SharedState( Mailbox* mailbox, Session* session, UiCommon* common );

    //*** GPS info

    Q_INVOKABLE QVariant getStorage();

    private slots:
};

#endif
