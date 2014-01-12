#ifndef WORKSPACE_FILEPATHPROPERTYMANAGER_H
#define WORKSPACE_FILEPATHPROPERTYMANAGER_H

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

  public:

    /*! Returns value (path) of the given property. */
    QString value(const QtProperty* property) const;
    /*! Returns filter of the given property. */
    QString filter(const QtProperty* property) const;

  protected:

    QString valueText(const QtProperty* property) const;
    void initializeProperty(QtProperty *property);
    void uninitializeProperty(QtProperty *property);

  private:

    /*! Internal data for a property. */
    struct Data
    {
      QString value;      /*!< Value of the property aka file path. */
      QString filter;     /*!< (File) Filter of the property. */
    };

    /*! Owned properties and their accociated data. */
    QMap<const QtProperty*, Data> m_values;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_FILEPATHPROPERTYMANAGER_H
