// Generated file, please edit export_mailbox.py in Scripts
#ifndef MAILBOX_H
#define MAILBOX_H

#include "Json/json_struct.h"

#include <QObject>
#include <QVariantList>
#include <QPointF>

class Mailbox : public QObject
{
    Q_OBJECT

    public:
    static const int MAJOR = 1;
    static const int MINOR = 0;

    public:
    explicit Mailbox(QObject *parent = nullptr);
    
    public slots:

    //*** UI Mobile

    void emitUiLoadedQml(  );


    signals:

    //*** UI Mobile

    void uiLoaded(  );
};

#endif
