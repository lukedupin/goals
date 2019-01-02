#include "android_platform.h"
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QtAndroidExtras>
#include <jni.h>
#endif

int AndroidPlatform::runService( int argc, char** argv )
{
#ifdef Q_OS_ANDROID
    //Service
    if ( argc > 1 )//&& strcmp(argv[1], "-service") == 0 )
    {
        QAndroidService app(argc, argv);

        app.exec();
        qDebug("Service QT thread has exited");
        return 0;
    }
#else
    Q_UNUSED(argc);
    Q_UNUSED(argv);
#endif

    return -1;
}

AndroidPlatform::AndroidPlatform( QGuiApplication* app, SharedState* state ) :
    _app(app),
    _sharedState( state )
{
    connect( &_timer, &QTimer::timeout,
             this, &AndroidPlatform::timeout );
}

/*
void AndroidPlatform::registerObjects()
{
    QAndroidJniObject java_klass = QAndroidJniObject ("com/fluidsoftwaredesigns/radius/GpsService");

    JNINativeMethod methods[] = {
        {"quitApp", "()V", reinterpret_cast<void*>(quit)}
    };

    QAndroidJniEnvironment env;
    jclass obj_klass = env->GetObjectClass(java_klass.object<jobject>());
    env->RegisterNatives(obj_klass, methods, sizeof (methods) / sizeof(methods[0]));
    env->DeleteLocalRef(obj_klass);

    qDebug("Registered C++ callbacks");
}
*/

void AndroidPlatform::onCaptureStatus( bool online )
{
#ifdef Q_OS_ANDROID
    //We are already stopping, so do nothing
    if ( _exitApp )
        return;

    QAndroidJniObject klass;

    //Reset my timer and then fire off the call.
    //We'll also start/stop the timer to be checking we are still alive
    _timeout.restart();
    if ( online )
    {
        QtAndroid::androidActivity().callMethod<jboolean>("startService", "()Z");
        _timer.start(TIMEOUT_CHECK);
    }
    else
    {
        QtAndroid::androidActivity().callMethod<jboolean>("stopService", "()Z");
        _timer.stop();
    }
#else
    Q_UNUSED(online);
#endif
}

void AndroidPlatform::captureStatusChanged( bool online )
{
#ifdef Q_OS_ANDROID
    if ( _exitApp && !online )
        _app->quit();
#else
    Q_UNUSED(online);
#endif
}

void AndroidPlatform::timeout()
{
#ifdef Q_OS_ANDROID
    auto alive = QtAndroid::androidActivity().callMethod<jboolean>("isAlive", "()Z");
    if ( alive )
    {
        _timeout.restart();
        return;
    }

    //If we are under our elapsed, then do nothing
    if ( _timeout.elapsed() <= TIMEOUT_QUIT )
        return;

    //Quit the app!
    qDebug("Qutting app.  We are running, but the notification isn't running anymore");
    if ( _exitApp )
        _app->quit();
    _exitApp = true;

    //Stop!!!
    _sharedState->Mail->emitSetCaptureStatus(false);
#endif
}

/*
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_fluidsoftwaredesigns_radius_GpsService_quitApp(JNIEnv* env, jobject obj)
{
    Q_UNUSED(env)
    Q_UNUSED(obj)

    //Kill the app
    qDebug("Calling app quit!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1");
    _instance->Running = false;
}

#ifdef __cplusplus
}
#endif
*/
