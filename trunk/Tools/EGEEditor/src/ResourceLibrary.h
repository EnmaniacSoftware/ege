#ifndef RESOURCE_LIBRARY_H
#define RESOURCE_LIBRARY_H

#include <QDockWidget>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_ResourceLibrary;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibrary : public QDockWidget
{
  Q_OBJECT

  public:

    ResourceLibrary(QWidget* parent);
   ~ResourceLibrary();

  private:

    /*! Resource library window UI. */
    Ui_ResourceLibrary* m_ui;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_H