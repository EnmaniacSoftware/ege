#include "App.h"
#include "CurvesTest.h"
#include <EGESignal.h>
#include <EGEResources.h>
#include <EGERenderer.h>
#include <EGERenderComponent.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CurvesTest::CurvesTest(App* app) : Test(app)
{
  ege_connect(app->graphics(), preRender, this, CurvesTest::preRender);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CurvesTest::~CurvesTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String CurvesTest::name() const
{
  return "Curves";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool CurvesTest::initialize()
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
  if (EGE_SUCCESS != app()->resourceManager()->loadGroup("curves-test"))
  {
    // error!
    return false;
  }

  // create splines from resources
  PResourceCurve curveResource = app()->resourceManager()->resource(RESOURCE_NAME_CURVE, "bezier-2");
  if (curveResource)
  {
    curveResource->setInstance(m_splines[0]);
  }

  // create splines manually
  m_splines[1].setType(EGESpline::TYPE_BEZIER);
  m_splines[1].addPoint(Vector4f(200, 300, 0), Vector4f(300, 300, 0));
  m_splines[1].addPoint(Vector4f(200, 400, 0), Vector4f(300, 400, 0));
  m_splines[2].setType(EGESpline::TYPE_BEZIER);
  m_splines[2].addPoint(Vector4f(300, 100, 0), Vector4f(350, 100, 0));
  m_splines[2].addPoint(Vector4f(400, 100, 0), Vector4f(350, 100, 0));
  m_splines[3].setType(EGESpline::TYPE_BEZIER);
  m_splines[3].addPoint(Vector4f(300, 300, 0), Vector4f(300, 200, 0));
  m_splines[3].addPoint(Vector4f(400, 300, 0), Vector4f(400, 200, 0));
  m_splines[3].addPoint(Vector4f(300, 300, 0), Vector4f(300, 400, 0)).setBeginTangent(Vector4f(400, 400, 0));

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void CurvesTest::update(const Time& time)
{
  m_timeParam += time.seconds();
  if (1.0f < m_timeParam)
  {
    m_timeParam -= 1.0f;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called before target is rendered. */
void CurvesTest::preRender(PRenderTarget target)
{
  EGE_UNUSED(target);

  PRenderComponent renderComponent;

  PResourceMaterial materialResource = app()->resourceManager()->resource(RESOURCE_NAME_MATERIAL, DEFAULT_MATERIAL_NAME, DEFAULT_GROUP_NAME);
  PMaterial material = materialResource->createInstance();

  renderComponent = RenderObjectFactory::Create(&m_splines[0], app(), "0", Vector4f::ZERO, RenderObjectFactory::VS_V3_C4, 50);
  renderComponent->setMaterial(material);
  app()->graphics()->renderer()->addForRendering(renderComponent);

  renderComponent = RenderObjectFactory::Create(&m_splines[1], app(), "1", Vector4f::ZERO, RenderObjectFactory::VS_V3_C4, 50);
  renderComponent->setMaterial(material);
  app()->graphics()->renderer()->addForRendering(renderComponent);

  renderComponent = RenderObjectFactory::Create(&m_splines[2], app(), "2", Vector4f::ZERO, RenderObjectFactory::VS_V3_C4, 50);
  renderComponent->setMaterial(material);
  app()->graphics()->renderer()->addForRendering(renderComponent);

  renderComponent = RenderObjectFactory::Create(&m_splines[3], app(), "3", Vector4f::ZERO, RenderObjectFactory::VS_V3_C4, 50);
  renderComponent->setMaterial(material);
  app()->graphics()->renderer()->addForRendering(renderComponent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void CurvesTest::pointerEvent(PPointerData data)
{
  EGE_UNUSED(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
