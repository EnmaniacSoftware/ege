#ifndef WORKSPACE_FILEPATHPROPERTYMANAGER_H
#define WORKSPACE_FILEPATHPROPERTYMANAGER_H

/*! Property manager responsible for creation and manipulation of QtProperty objects of FilePath type.
 *  Together with FilePathEditFactory it allows editing and proper synchronization of managed QtProperty objects.
 */

#include <QtAbstractPropertyManager>
#include <QMap>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FilePathPropertyManager : public QtAbstractPropertyManager
{
  Q_OBJECT

  public:

    explicit FilePathPropertyManager(QObject* parent = 0);
   ~FilePathPropertyManager();

  signals:

    /*! Singal emitted when value (path) of property changes. */
    void valueChanged(QtProperty* property, const QString& value);
    /*! Singal emitted when filter of property changes. */
    void filterChanged(QtProperty* property, const QString& value);

  public slots:

    /*! Sets value of the given property.
     *  @param  property  Property for which value is to be set.
     *  @param  value     Value to be set (path).
     */
    void setValue(QtProperty* property, const QString& value);
    /*! Sets file filter for a given property.
     *  @param  property  Property for which value is to be set.
     *  @param  value     Value to be set (file filter).
     */
    void setFilter(QtProperty* property, const QString& value);
    /*! Sets existance value for a given property.
     *  @param  property  Property for which value is to be set.
     *  @param  value     Value to be set (whether file should exist).
     *  @note This value is for use of editor.
     */
    void setMustExist(QtProperty* property, bool value);

  public:

    /*! Returns value (path) of the given property. */
    QString value(const QtProperty* property) const;
    /*! Returns filter of the given property. */
    QString filter(const QtProperty* property) const;
    /*! Returns existance flag for a given property. */
    bool mustExist(const QtProperty* property) const;

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
      QString value;      /*!< Value of the property aka file path. */
      QString filter;     /*!< (File) Filter of the property. */
      bool mustExist;     /*!< Existance flag for editor use. */
    };

    /*! Owned properties and their accociated data. */
    QMap<const QtProperty*, Data> m_values;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_FILEPATHPROPERTYMANAGER_H
