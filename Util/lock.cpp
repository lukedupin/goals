#include "lock.h"

Lock::Lock( QMutex* mutex ) :
  _mutex( mutex )
{
  _mutex->lock();
}

Lock::~Lock()
{
  _mutex->unlock();
}
