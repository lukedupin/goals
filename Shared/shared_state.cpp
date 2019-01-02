#include "shared_state.h"

SharedState::SharedState( Mailbox* mailbox,
                          Session* session,
                          UiCommon* ui ) :
    Mail( mailbox ),
    Sess( session ),
    Ui( ui )
{
}


//*** GPS logic

QVariant SharedState::getStorage()
{
    return Sess->storage();
}


//*** Slots
