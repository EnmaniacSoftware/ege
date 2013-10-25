#include "ObjectPool.h"
#include "Application.h"
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
ObjectPool& ObjectPool::Instance()
{
  return qobject_cast<Application*>(qApp)->objectPool();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ObjectPool::addObject(QObject* object)
{
  // check if already in pool
  if ((NULL != object) && ! m_objects.contains(object))
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
QList<QObject*> ObjectPool::allObjects() const
{
  return m_objects;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
