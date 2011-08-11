#ifndef TEST_H
#define TEST_H

#include <EGEString.h>
#include <EGETime.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class App;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Test
{
  public:

    Test(App* app) : m_app(app) {}
    virtual ~Test() {}

    /*! Returns test name. */
    virtual EGE::String name() const = 0;
    /*! Creates scene. */
    virtual bool createScene() = 0;
    /*! Updates test. */
    virtual void update(const EGE::Time& time) = 0;

  protected:

    /*! Returns point to app object. */
    inline App* app() const { return m_app; }

  private:
    
    /*! Pointer to app object. */
    App* m_app;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_H