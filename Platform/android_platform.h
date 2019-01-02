#ifndef ANDROID_PLATFORM_H
#define ANDROID_PLATFORM_H

#include <QGuiApplication>
#include <Shared/shared_state.h>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QObject>

class AndroidPlatform : public QObject
{
    Q_OBJECT

    public:
    static const int TIMEOUT_CHECK = 2400;
    static const int TIMEOUT_QUIT = 5000;

    //Run a service
    static int runService( int argc, char** argv );

    private:
    QGuiApplication* _app;
    SharedState* _sharedState = nullptr;
    QTimer _timer;
    QTime _timeout;
    bool _exitApp = false;

    public:
    //Init the object
    AndroidPlatform( QGuiApplication* app, SharedState* shared );

    public slots:
    // Catch when the user changes the state
    void onCaptureStatus( bool online );

    //Capture status has changed!
    void captureStatusChanged( bool online );

    //Check for an app close situation
    void timeout();
};

#endif
