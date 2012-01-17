#include "App.h"
#include "LightningTest.h"
#include <EGESignal.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningTest::LightningTest(App* app) : Test(app), 
                                         m_effect(NULL),
                                         m_effectLines(NULL),
                                         m_effectQuads(NULL),
                                         m_effectStrips(NULL)
{
  ege_connect(app->graphics(), preRender, this, LightningTest::preRender);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningTest::~LightningTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String LightningTest::name() const
{
  return "Lightning";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool LightningTest::initialize()
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

  camera->setLookAt(Vector3f(0, 0, 0));
  camera->physics()->setPosition(Vector4f(0, 0, -50));

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
  if (EGE_SUCCESS != app()->resourceManager()->loadGroup("lightning-test"))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void LightningTest::update(const Time& time)
{
  m_effect->update(time);
  m_effectLines->update(time);

  if (m_effectQuads)
  {
    m_effectQuads->update(time);
  }

  if (m_effectStrips)
  {
    m_effectStrips->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void LightningTest::pointerEvent(PPointerData data)
{
  m_effect->pointerEvent(data);
  m_effectLines->pointerEvent(data);

  if (EGEInput::ACTION_BUTTON_DOWN == data->action())
  {
    if (m_effectQuads)
    {
      m_effectQuads->create(Vector2f(100, 350), Vector2f(600, 500), 5, true);
      m_effectQuads->start();
    }

    if (m_effectStrips)
    {
      m_effectStrips->create(Vector2f(100, 50), Vector2f(600, 200), 5, true);
      m_effectStrips->start();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Slot called when resource group has been loaded. */
void LightningTest::groupLoadComplete(const String& name)
{
  if ("lightning-test" == name)
  {
    // initialize effects
    m_effect = ege_new LightningEffect(app());
    m_effectLines = ege_new LightningEffectLines(app());
    m_effectQuads = ege_new LightningEffectQuads(app());
    m_effectStrips = ege_new LightningEffectStrips(app());

    // setup effects
    m_effectQuads->setOffshotAngle(Angle::FromDegrees(12.5f));
    m_effectQuads->setOffshotAngleVariance(Angle::FromDegrees(2.5f));
    m_effectQuads->setWidth(3.0f);

    m_effectStrips->setOffshotAngle(Angle::FromDegrees(12.5f));
    m_effectStrips->setOffshotAngleVariance(Angle::FromDegrees(2.5f));
    m_effectStrips->setWidth(3.0f);

    SceneNode* node = app()->sceneManager()->rootNode()->createChildSceneNode("lightning-effect");
    node->attachObject(m_effect);

    node = app()->sceneManager()->rootNode()->createChildSceneNode("lightning-effect-lines");
    node->attachObject(m_effectLines);

    // assign materials
    PResourceMaterial resource = app()->resourceManager()->resource(RESOURCE_NAME_MATERIAL, "beam");
    m_effectQuads->renderData()->setMaterial(resource->createInstance());
    m_effectStrips->setMaterial(resource->createInstance());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called before target is rendered. */
void LightningTest::preRender(PRenderTarget target)
{
  EGE_UNUSED(target);

  if (m_effectQuads)
  {
    m_effectQuads->render(app()->graphics()->renderer());
  }

  if (m_effectStrips)
  {
    m_effectStrips->render(app()->graphics()->renderer());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------