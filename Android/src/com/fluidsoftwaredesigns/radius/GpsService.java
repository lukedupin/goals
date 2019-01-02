package com.fluidsoftwaredesigns.radius;

import org.qtproject.qt5.android.bindings.QtActivity;
import android.content.Intent;
import android.content.Context;
import java.io.FileDescriptor;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import android.util.Log;

import android.os.IBinder;
import android.os.Binder;
import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.os.IInterface;
import android.os.Parcel;

import android.Manifest;
import android.app.Activity;
import android.os.Build;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.IntentService;

import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;
import org.qtproject.qt5.android.bindings.QtService;

//public class GpsService extends IntentService
//public class GpsService extends IntentService implements IBinder
public class GpsService extends QtService implements IBinder
{
  /*
  @Override
  public void onHandleIntent(Intent intent )
  {
    try {
      while(true)
          Thread.sleep(3000);
          // Do some stuff
        } catch (Exception e) {}
  }
  /*
    static {
      System.loadLibrary("radius");
    }
  */

    /*
    A constructor is required, and must call the super IntentService(String)
    constructor with a name for the worker thread.
    */
    /*
    public GpsService() {
        super("GpsService");
    }
  */

    @Override
    public void onCreate()
    {
        super.onCreate();

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            startForeground(917232, getNotification());
            Log.v("radius", "Starting foreground");
        }
        else
            Log.v("radius", "Starting in the background?");
    }

    @Override
    public int onStartCommand( Intent intent, int flags, int start_id )
    {
        //String action = intent.getAction();
        if ( intent == null || intent.getAction() == null || intent.getAction().equals("STOP") )
        {
            Log.v("radius", "Command stop");
            stopRunning();
        }
        else
        {
            Log.v("radius", "Command start");
        }

        return START_STICKY;
    }


    @Override
    public void onDestroy() {
        super.onDestroy();

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            stopRunning();
        }
    }

    @Override
    public void onTaskRemoved(Intent rootIntent) {
        Log.e("radius", "Detected app removal.  Closing down the foreground service");
        stopRunning();
    }

    //Setup my binder
    public class LocalBinder extends Binder
    {
        GpsService _this = null;

        public LocalBinder( GpsService gps )
        {
            _this = gps;
        }

        @Override
        public String getInterfaceDescriptor()
        {
            return "This is working!?";
        }

        @Override public IInterface queryLocalInterface(String description)
        {
          return new IInterface() {
            public IBinder asBinder() {
                return _this;
              }
            };
        }

        public GpsService getService() {
            return _this;
        }
    }
    //private final IBinder _binder = new LocalBinder();

    public String GetMsg()
    {
      return "Dude this is finally okay";
    }

    @Override public void 	dump(FileDescriptor fd, String[] args) {}

    @Override public void 	dumpAsync(FileDescriptor fd, String[] args){}

    @Override public String 	getInterfaceDescriptor() { return "Interfaces"; }

    @Override public boolean 	isBinderAlive() { return true; }

    @Override public void 	linkToDeath(IBinder.DeathRecipient recipient, int flags){}

    @Override public boolean 	pingBinder() { return true; }

    @Override public IInterface 	queryLocalInterface(String descriptor) { return null; }

    @Override public boolean 	transact(int code, Parcel data, Parcel reply, int flags) { return true; }

    @Override public boolean 	unlinkToDeath(IBinder.DeathRecipient recipient, int flags) { return true; }

    @Override
    public IBinder onBind(Intent intent) {
        return new LocalBinder( this );
        //return _binder;
    }

    //stop the running service
    public void stopRunning()
    {
        stopForeground(true);
        stopSelf();
        Log.v("radius", "Setting service running state to false");
    }

  /*
    @Override
    protected void onHandleIntent(Intent intent) {
        // Normally we would do some work here, like download a file.
        // For our sample, we just sleep for 5 seconds.
        try {
            while ( _running )
            {
                Thread.sleep(1);
                sendBroadcast(new Intent("CLOSE_RADIUS"));
                }
            Log.v("radius", "Service working completed");
        } catch (InterruptedException e) {
            // Restore interrupt status.
            Thread.currentThread().interrupt();
        }
    }
  */


    private Notification getNotification() {
        NotificationChannel channel = new NotificationChannel(
            "com.fluidsoftwaredesigns.radius",
            "Radius",
            NotificationManager.IMPORTANCE_DEFAULT
        );

        NotificationManager notificationManager = getSystemService(NotificationManager.class);
        notificationManager.createNotificationChannel(channel);

        Intent notificationIntent = new Intent(this, MyActivity.class);
        PendingIntent pendingIntent=PendingIntent.getActivity(this, 0,
                notificationIntent, Intent.FLAG_ACTIVITY_NEW_TASK);

        return new Notification.Builder(getApplicationContext(), "com.fluidsoftwaredesigns.radius")
                    //.setOngoing(true)
                    .setSmallIcon(R.drawable.icon)
                    .setContentText("Radius capturing your activity")
                    .setContentIntent(pendingIntent)
                    .build();
    }
}
