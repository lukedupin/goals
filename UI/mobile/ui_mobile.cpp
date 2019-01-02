#include "ui_mobile.h"

#include <Platform/android_platform.h>
#include <Platform/platform.h>

#include <QtCore/QTextStream>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include <QRandomGenerator64>
#include <QJsonDocument>

    //Register singleton types with QML
#define REGISTER_SINGLETON( x, inst ) \
    qmlRegisterSingletonType<x>("Radius", x::MAJOR, x::MINOR, #x, \
        [](QQmlEngine*, QJSEngine*) -> QObject* { return inst; })

using namespace std;

static Mailbox* _mailbox = nullptr;
static SharedState* _sharedState = nullptr;
static Platform* _platform = nullptr;

UiMobile::UiMobile()
{
}

int UiMobile::runUi(QGuiApplication* app, SharedState* shared_state)
{
    QVariantMap parameters;
    QStringList args(QCoreApplication::arguments());

    //Register to know when the UI exists
    QObject::connect( shared_state->Mail, &Mailbox::done,
                      this, &UiMobile::onDone );
    QObject::connect( shared_state->Mail, &Mailbox::uiLoaded,
                      this, &UiMobile::onUiLoaded );

    //Create my QML engine object
    QQmlApplicationEngine engine;
    _engine = &engine;

    _platform = new Platform();

    //Register static qml types
    qmlRegisterModule("Radius", 1, 0);
    qmlRegisterUncreatableMetaObject(PlatformTypes::staticMetaObject, "Radius", 1, 0, "PlatformType", "Type of platform");

    //Register my singletons
    _mailbox = shared_state->Mail;
    _sharedState = shared_state;
    REGISTER_SINGLETON( Mailbox, _mailbox );
    REGISTER_SINGLETON( SharedState, _sharedState );
    REGISTER_SINGLETON( Platform, _platform );

    //Configure the QML engine
    _engine->addImportPath(QStringLiteral(":/imports"));
    _engine->load(QUrl(QStringLiteral("qrc:///goals.qml")));
    QObject::connect(_engine, SIGNAL(quit()), qApp, SLOT(quit()));

    //QObject *item = _engine->rootObjects().first();
    //Q_ASSERT(item);

    //Setup my context
    _engine->rootContext()->setContextProperty("ui_common", this );

    //QMetaObject::invokeMethod(item, "initializeProviders",
                              //Q_ARG(QVariant, QVariant::fromValue(parameters)));

    //Android platform!
    AndroidPlatform android_platform( app, _sharedState );

    //Run the application loop
    auto result = app->exec();

    //clean up and return the result
    _engine = nullptr;
    _platform = nullptr;
    return result;
}

void UiMobile::onUiLoaded()
{
}

void UiMobile::onDone( JStorage storage )
{
    _sharedState->Sess->save( storage );
    QGuiApplication::exit();
}

/*
void UiMobile::onRequestUserSignup()
{
    if ( _engine == nullptr )
        return;

    QObject *item = _engine->rootObjects().first();
    Q_ASSERT(item);

    QMetaObject::invokeMethod(item, "getUserSignup" );
}
*/
