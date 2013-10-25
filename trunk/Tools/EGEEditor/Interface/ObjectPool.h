#ifndef EDITOR_OBJECTPOOL_H
#define EDITOR_OBJECTPOOL_H

#include <QObject>
#include <QList>
#include <QReadWriteLock>
#include <QReadLocker>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ObjectPool : public QObject
{
  Q_OBJECT

  public:

    ObjectPool();
   ~ObjectPool();

  signals:

    /*! Signal emitted when object has been added into the pool. */
    void objectAdded(QObject* object);
    /*! Signal emitted when object is to about to be removed from pool. */
    void aboutToRemoveObject(QObject* object);
    /*! Signal emitted when object is removed from pool. */
    void objectRemoved(QObject* object);

  public:

    /*! Returns pool instance. */
    static ObjectPool& Instance();
    
  public:

    /*! Adds given object into pool. */
    bool addObject(QObject* object);
    /*! Removes given object from pool. */
    void removeObject(QObject *object);
    /*! Returns all objects. */
    QList<QObject*> allObjects() const;

    /*! Returns list of objects of a given type. */
    template <typename T> 
    QList<T*> getObjects() const;
    
    /*! Returns first object of a given type. */
    template <typename T> 
    T* getObject() const;

  private:

    /*! List of objects. */
    QList<QObject*> m_objects;
    /*! Data access locker. */
    mutable QReadWriteLock m_lock;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
QList<T*> ObjectPool::getObjects() const
{
  QReadLocker lock(&m_lock);

  QList<T*> result;
  foreach (QObject* object, m_objects)
  {
    T* casted = qobject_cast<T*>(object);

    if (NULL != casted)
    {
      result.append(casted);
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T* ObjectPool::getObject() const
{
  QReadLocker lock(&m_lock);

  foreach (QObject* object, m_objects)
  {
    T* casted = qobject_cast<T*>(object);
    if (NULL != casted)
    {
      return casted;
    }
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EDITOR_OBJECTPOOL_H
