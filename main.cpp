#include "Shared/mailbox.h"
#include "Shared/session.h"
#include "Shared/shared_state.h"
#include "Util/util.h"
#include <main.h>

#include <UI/mobile/ui_mobile.h>

#include <QtCore/QTextStream>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include <QRandomGenerator64>
#include <QJsonDocument>

int main(int argc, char *argv[])
{
    //Attempt to run a service?
    if ( AndroidPlatform::runService( argc, argv ) >= 0 )
        return 0;

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication application(argc, argv);

    //Register my json types
    apiStructRegister();

#if QT_CONFIG(library)
    const QByteArray additionalLibraryPaths = qgetenv("QTLOCATION_EXTRA_LIBRARY_PATH");
    for (const QByteArray &p : additionalLibraryPaths.split(':'))
        QCoreApplication::addLibraryPath(QString(p));
#endif

    //Message passing interface
    auto mailbox = new Mailbox;

    //Load my session data
    auto sess = new Session;
    sess->load();

    //Setup the shared state, all of these need to be dynamic since they might be loaded into the qml render engine
    //If they aren't dynamically allocated, the program will crash on exit when the engine attempts to free static memory
    auto ui = new UiMobile; //Change this for different UIs
    auto shared_state = new SharedState( mailbox, sess, ui );

    //Run the UI
    return ui->runUi( &application, shared_state );
}

/*
//C++
Q_INVOKABLE void getData(QJSValue value) {
    if (value.isCallable()) {
        QJSValueList args;
        args << QJSValue("Hello, world!");
        value.call(args);
    }
}

//QML
action.getData(function (data){console.log(data)});
*/
