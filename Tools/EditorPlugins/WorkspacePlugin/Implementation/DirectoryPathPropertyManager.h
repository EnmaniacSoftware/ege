#ifndef WORKSPACE_DIRECTORYPATHPROPERTYMANAGER_H
#define WORKSPACE_DIRECTORYPATHPROPERTYMANAGER_H

/*! Property manager responsible for creation and manipulation of QtProperty objects of Directory type.
 *  Together with DirectoryPathEditFactory it allows editing and proper synchronization of managed QtProperty objects.
 */

#include <QtAbstractPropertyManager>
#include <QMap>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DirectoryPathPropertyManager : public QtAbstractPropertyManager
{
  Q_OBJECT

  public:

    explicit DirectoryPathPropertyManager(QObject* parent = 0);
   ~DirectoryPathPropertyManager();

  signals:

    /*! Singal emitted when value (path) of property changes. */
    void valueChanged(QtProperty* property, const QString& value);

  public slots:

    /*! Sets value of the given property.
     *  @param  property  Property for which value is to be set.
     *  @param  value     Value to be set (path).
     */
    void setValue(QtProperty* property, const QString& value);

  public:

    /*! Returns value (path) of the given property. */
    QString value(const QtProperty* property) const;

  protected:

    /*! @see QtAbstractPropertyManager::valueText. */
    QString valueText(const QtProperty* property) const;
    /*! @see QtAbstractPropertyManager::initializeProperty. */
    void initializeProperty(QtProperty *property);
    /*! @see QtAbstractPropertyManager::uninitializeProperty. */
    void uninitializeProperty(QtProperty *property);

  private:

    /*! Internal data for a property. */
    struct Data
    {
      QString value;      /*!< Value of the property aka directory path. */
    };

    /*! Owned properties and their accociated data. */
    QMap<const QtProperty*, Data> m_values;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_DIRECTORYPATHPROPERTYMANAGER_H
