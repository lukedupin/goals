
#include "mailbox.h"

Mailbox::Mailbox(QObject *parent) : QObject(parent)
{
}


//*** UI Mobile

void Mailbox::emitUiLoadedQml(   )
{
    emit uiLoaded(  );
}

void Mailbox::emitDoneQml( QJSValue storage  )
{
    emit done( JStorage( storage ) );
}

