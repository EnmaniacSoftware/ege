#ifndef RESOURCELIBRARY_RESOURCELIBRARYDATAMODELPROXY_H
#define RESOURCELIBRARY_RESOURCELIBRARYDATAMODELPROXY_H

/*! Resource library model proxy used for filtering depending on current configuration setting.
 */

#include <QString>
#include <QSortFilterProxyModel>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryDataModelProxy : public QSortFilterProxyModel
{
  Q_OBJECT

  public:

    explicit ResourceLibraryDataModelProxy(QObject* parent = 0);
    ~ResourceLibraryDataModelProxy();

  signals:

    /*! Signal emitted after filter configuration change. */
    void filterChanged();

  private slots:

    /*! Slot called when configuration selection has changed.
     *  @param  name  New configuration name.
     */
    void onConfigurationChanged(const QString& name);
    /*! Slot called when current configuration name has changed.
     *  @param  oldName Old configuration name.
     *  @param  newName New configuration name.
     */
    void onConfigurationNameChanged(const QString& oldName, const QString& newName);

  private:

    /*! Filter method determining if the item in the given row should be included in the model.
     *  @param  sourceRow     Row number of the item.
     *  @param  sourceParent  Parent of the item.
     *  @return TRUE if item should be displayed. Otherwise, FALSE.
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

  private:

    /*! Configuration name for which filtering is to be done. */
    QString m_configName;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCELIBRARYDATAMODELPROXY_H
