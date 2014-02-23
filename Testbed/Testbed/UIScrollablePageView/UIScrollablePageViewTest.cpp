#include "App.h"
#include "UIScrollablePageViewTest.h"
#include <EGESignal.h>
#include <EGEResources.h>
#include <EGEOverlay.h>
#include <EGERenderer.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define ORTHO
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIScrollablePageViewTest::UIScrollablePageViewTest(App* app) : Test(app), 
                                                                m_area(NULL)
{
  ege_connect(app->graphics(), preRender, this, UIScrollablePageViewTest::preRender);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIScrollablePageViewTest::~UIScrollablePageViewTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String UIScrollablePageViewTest::name() const
{
  return "UIScrollablePageView";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool UIScrollablePageViewTest::initialize()
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
  if (EGE_SUCCESS != app()->resourceManager()->loadGroup("ui-scrollable-page-view-test"))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void UIScrollablePageViewTest::update(const Time& time)
{
  if (m_area)
  {
    m_area->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void UIScrollablePageViewTest::pointerEvent(PPointerData data)
{
  if (m_area)
  {
    m_area->pointerEvent(data);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called before target is rendered. */
void UIScrollablePageViewTest::preRender(PRenderTarget target)
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
void UIScrollablePageViewTest::groupLoadComplete(const String& name)
{
  if ("ui-scrollable-page-view-test" == name)
  {
    const u32 pageCount = 3;
    Vector4f position(50, 50, 0);
    Vector2f size(400, 500);

    // create scrollable area
    m_area = UIScrollablePageView::Create(app(), "my-scrollable-page-view");
    m_area->setPosition(position);
    m_area->setSize(size);
    m_area->setPageCount(pageCount);
//    m_area->setPageIndicatorEnabled(false);

    // setup vertex declaration
    VertexDeclaration vertexDeclaration;
    vertexDeclaration.addElement(NVertexBuffer::VES_POSITION_XY);

    // create scrollable area background object 
    m_areaBackground = RenderObjectFactory::CreateQuadXY(app(), "background", position, size, 
                                                         ALIGN_TOP_LEFT, false, false, vertexDeclaration, 1, EGEGraphics::RPT_TRIANGLE_STRIPS, 
                                                         NVertexBuffer::UT_STATIC_WRITE);

    // get material resource
    PResourceMaterial resource = app()->resourceManager()->resource(RESOURCE_NAME_MATERIAL, "background", "ui-scrollable-page-view-test");
    m_areaBackground->setMaterial(resource->createInstance());

    PResourceFont fontResource = app()->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
    if (fontResource)
    {
      for (u32 page = 0; page < pageCount; ++page)
      {
        // add scroll area content
        for (s32 i = 0; i < 30; ++i)
        {
          PTextOverlay text = ege_new TextOverlay(app(), "text");
          text->setFont(fontResource->font());
          text->setText(Text::Format("Text. Page: %d Line %d", page + 1, i + 1));
          text->physics()->setPosition(Vector4f(0, i * 50.0f, 0));
          m_area->addObject(text, page);
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
