#ifndef RESOURCE_LIBRARY_H
#define RESOURCE_LIBRARY_H

#include <QDockWidget>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_ResourceLibrary;
class Config;
class MainWindow;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibrary : public QDockWidget
{
  Q_OBJECT

  public:

    ResourceLibrary(QWidget* parent);
   ~ResourceLibrary();

    /* Saves settings. */
    void saveSettings(Config* config);
    /* Loads settings. */
    void loadSettings(Config* config);

  private:

    /*! Returns pointer to main window object. */
    inline MainWindow* mainWindow() const { return reinterpret_cast<MainWindow*>(parent()); }

  private:

    /*! Resource library window UI. */
    Ui_ResourceLibrary* m_ui;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_H