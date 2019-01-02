#ifndef UI_COMM_H
#define UI_COMM_H

#include <functional>
#include <QString>
#include <QObject>

class UiCommon : public QObject
{
    Q_OBJECT

    public:
    virtual ~UiCommon();
};

#endif
