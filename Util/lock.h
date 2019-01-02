#ifndef LOCK_H
#define LOCK_H

#include <QObject>
#include <QMutex>

//Handy
#define ScopeLock( x ) auto&& __lock__ __attribute__((unused)) = Lock( &x )

class Lock : QObject
{
  QMutex* _mutex;

  public:
  Lock( QMutex* mutex );

  ~Lock();
};

#endif
