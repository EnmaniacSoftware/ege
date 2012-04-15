#include "ObjectPool.h"
#include <QWriteLocker>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ObjectPool::ObjectPool() : QObject()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ObjectPool::~ObjectPool()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns pool instance. */       
ObjectPool* ObjectPool::Instance()
{
  static ObjectPool pool;
  return &pool;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds given object into pool. */
bool ObjectPool::addObject(QObject* object)
{
  // check if already in pool
  if ((NULL != object) && !m_objects.contains(object))
  {
    QWriteLocker lock(&m_lock);
    m_objects.append(object);
    lock.unlock();

    // emit
    emit objectAdded(object);
    return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given object from pool. */
void ObjectPool::removeObject(QObject *object)
{
  // check if in pool
  if ((NULL != object) && m_objects.contains(object))
  {
    // emit
    emit aboutToRemoveObject(object);

    QWriteLocker lock(&m_lock);
    m_objects.removeAll(object);
    lock.unlock();

    // emit
    emit objectRemoved(object);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns all objects. */
QList<QObject*> ObjectPool::allObjects() const
{
  return m_objects;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
