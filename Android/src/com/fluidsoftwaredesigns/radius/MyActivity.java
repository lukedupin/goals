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

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Debug;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;

import android.Manifest;
import android.app.Activity;
/*
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
*/
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Build;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.os.IBinder;

import com.fluidsoftwaredesigns.radius.GpsService.LocalBinder;

  //Setup my custom activity
public class MyActivity
  extends QtActivity
{
  /*
    static {
      System.loadLibrary("radius");
    }
  */

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        Log.v("radius", "onCreate");
    }

    @Override
    public void onResume()
    {
        super.onResume();
        Log.v("radius", "onResume");
    }

    @Override
    public void onPause()
    {
        super.onPause();
        Log.v("radius", "onPause");
    }

    @Override
    public void onStop()
    {
        super.onStop();
        Log.v("radius", "onStop");
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
        Log.v("radius", "onDestroy");
    }

    //Setup the binder
    IBinder _binder = null;
    public boolean isAlive()
    {
      return ( _binder != null )? _binder.isBinderAlive(): true;
    }
    protected ServiceConnection mServerConn = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder binder) {
            Log.d("radius", "onServiceConnected");
            _binder = binder;
            try {
            binder.getInterfaceDescriptor();
            //LocalBinder bin = (LocalBinder)(Object)_binder.getInterfaceDescriptor();
            //binder.queryLocalInterface("Test");//.getInterfaceDescriptor() );
            //Log.v("radius", "Hey whats up with this: "+ binder.queryLocalInterface("Test").asBinder().getInterfaceDescriptor() );
            //Log.v("radius", "Hey whats up with this: "+ _binder.getInterfaceDescriptor());
            } catch ( RemoteException e ){}
            //((LocalBinder)binder).getService().stopRunning();
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            Log.d("radius", "onServiceDisconnected");
            _binder = null;
        }
    };

    boolean _started = false;
    public boolean startService()
    {
        if ( Build.VERSION.SDK_INT < Build.VERSION_CODES.O || _started )
        {
            Log.v("radius", "Not going to start because we are at ID: "+ Build.VERSION.SDK_INT );
            return false;
        }

        //Create the instance
        Intent intent = new Intent(this, GpsService.class);
        intent.setAction("START");
        bindService( intent, mServerConn, 0 );
        startForegroundService(intent);
        Log.v("radius", "Starting the service from the activity");
        _started = true;

        return true;
    }

    boolean stopService()
    {
        if ( Build.VERSION.SDK_INT < Build.VERSION_CODES.O || !_started )
            return false;

        //Use an intent to stop the service
        _started = false;
        Intent intent = new Intent(this, GpsService.class);
        intent.setAction("STOP");
        startForegroundService(intent);
        Log.v("radius", "Stopping the service");
        return true;
    }

/*
  @Override
  public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) 
  {
      //Figure out what was given to us
    switch(requestCode) 
    {
      case LOCATION_REQUEST:
        if ( canAccessLocation() ) 
          log("Access to location is granted");
        else 
          log("Access to location is denied");
        break;
    }
  }

  private boolean hasPermission(String perm) 
  {
    try {
      return ( PackageManager.PERMISSION_GRANTED == checkSelfPermission(perm) );
    } catch ( NoSuchMethodError e ) {
      return true;
    }
  }

  public boolean canAccessLocation() 
  {
    return hasPermission(Manifest.permission.ACCESS_FINE_LOCATION);
  }

  public static void log( String s )
  {
    Log.d("Radius", s );
  }

    //Return a guid
  public String deviceId()
  {
    return Secure.getString( getContentResolver(), Secure.ANDROID_ID);
    / *
    TelephonyManager tm = (TelephonyManager)getSystemService(Context.TELEPHONY_SERVICE);
    String uid = tm.getDeviceId();
    * /
  }
*/
}
