#include "App.h"
#include "ScrollableAreaTest.h"
#include <EGESignal.h>
#include <EGEResources.h>
#include <EGEOverlay.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScrollableAreaTest::ScrollableAreaTest(App* app) : Test(app), 
                                                   m_area(NULL)
{
  ege_connect(app->graphics(), preRender, this, ScrollableAreaTest::preRender);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScrollableAreaTest::~ScrollableAreaTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String ScrollableAreaTest::name() const
{
  return "ScrollableArea";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool ScrollableAreaTest::initialize()
{
  // get window render target (created thru config options)
  PRenderWindow window = app()->graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);

  // create new camera
  PCamera camera = ege_new Camera("myCamera", app()->sceneManager());

#ifdef ORTHO
  // setup orthographic projection
  camera->setProjectionType(EGE::Frustum::ORTHOGRAPHIC);
  camera->setOrthoSettings(Rectf(0, 0, window->width() * 1.0f, window->height() * 1.0f));
  camera->setNearPlaneDistance(-1);
  camera->setFarPlaneDistance(1);
#endif

  // create new viewport for render window
  PViewport viewport = window->addViewport("mainview", camera);
  if (NULL == viewport)
  {
    // error!
    return false;
  }

  viewport->setClearBufferTypes(Viewport::BUFFER_TYPE_COLOR);
  viewport->setClearColor(Color::BLUE);

  // load resources
  if (EGE_SUCCESS != app()->resourceManager()->loadGroup("scrollable-area-test"))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void ScrollableAreaTest::update(const Time& time)
{
  if (m_area)
  {
    m_area->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void ScrollableAreaTest::pointerEvent(PPointerData data)
{
  if (m_area)
  {
    m_area->pointerEvent(data);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called before target is rendered. */
void ScrollableAreaTest::preRender(PRenderTarget target)
{
  EGE_UNUSED(target);

  if (m_areaBackground)
  {
    app()->graphics()->renderer()->addForRendering(m_areaBackground);
  }

  if (m_area)
  {
    m_area->addForRendering(app()->graphics()->renderer());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Slot called when resource group has been loaded. */
void ScrollableAreaTest::groupLoadComplete(const String& name)
{
  if ("scrollable-area-test" == name)
  {
    Vector4f position(50, 50, 0);
    Vector2f size(400, 500);

    // create scrollable area
    m_area = ege_new ScrollableArea(app(), "my-scrollable-area");
    m_area->setDirection(DIRECTION_BOTH);
    m_area->setPosition(position);
    m_area->setSize(size);

    // create scrollable area background object
    m_areaBackground = RenderObjectFactory::CreateQuadXY(app(), "background",position, size, 
                                                         ALIGN_TOP_LEFT, EGEVertexBuffer::ST_V2, 1, EGEGraphics::RPT_TRIANGLE_STRIPS, 
                                                         EGEVertexBuffer::UT_STATIC_WRITE);

    // get material resource
    PResourceMaterial resource = app()->resourceManager()->resource(RESOURCE_NAME_MATERIAL, "background", "scrollable-area-test");
    m_areaBackground->setMaterial(resource->createInstance());

    PResourceFont fontResource = app()->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
    if (fontResource)
    {
      // add scroll area content
      for (s32 i = 0; i < 30; ++i)
      {
        PTextOverlay text = ege_new TextOverlay(app(), "text");
        text->setFont(fontResource->font());
        text->setText(Text::Format("Text line number %d YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY", i + 1));
        text->physics()->setPosition(Vector4f(0, i * 50.0f, 0));
        m_area->addObject(text);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
