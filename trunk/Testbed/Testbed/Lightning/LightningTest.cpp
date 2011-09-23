#include "App.h"
#include "LightningTest.h"
#include <EGESignal.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningTest::LightningTest(App* app) : Test(app), m_effect(NULL)
{
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

  // initialize effect
  m_effect = ege_new LightningEffect(app());
  //if ((NULL == m_effect) || !m_effect->initialize(window->width(), window->height(), Vector2i(32, 32), camera))
  //{
  //  // error!
  //  return false;
  //}

  m_sceneObject = RenderObject::CreateRectangle(app(), "SceneObject", window->width(), window->height(), false, true);
  if (NULL == m_sceneObject)
  {
    // error!
    return false;
  }

  SceneNode* node = app()->sceneManager()->rootNode()->createChildSceneNode(m_sceneObject->name());
  node->attachObject(m_sceneObject);
  node->physics()->setPosition(Vector4f(window->width() / 2.0f, window->height() / 2.0f, 0));

  node = app()->sceneManager()->rootNode()->createChildSceneNode(m_effect->name());
  node->attachObject(m_effect);

  // assign material to scene object
  PResourceMaterial materialResource = app()->resourceManager()->resource(RESOURCE_NAME_MATERIAL, "background");
  m_sceneObject->renderData()->setMaterial(materialResource->createInstance());
 
  ege_connect(app()->graphics(), preRender, this, LightningTest::preRender);
  ege_connect(app()->graphics(), postRender, this, LightningTest::postRender);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void LightningTest::update(const Time& time)
{
  m_effect->update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called before target is rendered. */
void LightningTest::preRender(PRenderTarget target)
{
  // check if target is texture
  if (RenderTarget::PRIORITY_RENDER_TEXTURE == target->priority())
  {
    // hide render texture quad
    m_effect->setVisible(false);

    // show scene object
    m_sceneObject->setVisible(true);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called after target has been rendered. */
void LightningTest::postRender(PRenderTarget target)
{
  // check if target is texture
  if (RenderTarget::PRIORITY_RENDER_TEXTURE == target->priority())
  {
    // show render texture quad
    m_effect->setVisible(true);

    // hide scene object
    m_sceneObject->setVisible(false);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void LightningTest::pointerEvent(PPointerData data)
{
  m_effect->pointerEvent(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
