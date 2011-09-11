#include "App.h"
#include "RipplesTest.h"
#include <EGESignal.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RipplesTest::RipplesTest(App* app) : Test(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RipplesTest::~RipplesTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String RipplesTest::name() const
{
  return "Ripples";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool RipplesTest::initialize()
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
  if (EGE_SUCCESS != app()->resourceManager()->loadGroup("ripples-test"))
  {
    // error!
    return false;
  }

  m_sceneObject = RenderObject::CreateRectangle(app(), "SceneObject", window->width(), window->height(), false, true);
  m_sceneObjectRenderTexture = RenderObject::CreateRectangle(app(), "RenderTexture", window->width() - 100, window->height() - 100, false, true);
  if (NULL == m_sceneObject || NULL == m_sceneObjectRenderTexture)
  {
    // error!
    return false;
  }

  SceneNode* node = app()->sceneManager()->rootNode()->createChildSceneNode(m_sceneObject->name());
  node->attachObject(m_sceneObject);
  node->physics()->setPosition(Vector4f(window->width() / 2.0f, window->height() / 2.0f, 0));

  node = app()->sceneManager()->rootNode()->createChildSceneNode(m_sceneObjectRenderTexture->name());
  node->attachObject(m_sceneObjectRenderTexture);
  node->physics()->setPosition(Vector4f(window->width() / 2.0f, window->height() / 2.0f, 0));

  // assign material to scene object
  PResourceMaterial materialResource = app()->resourceManager()->resource(RESOURCE_NAME_MATERIAL, "background");
  m_sceneObject->renderData()->setMaterial(materialResource->createInstance());

  // create render texture
  m_texture = Texture2D::CreateRenderTexture(app(), "rttTex", window->width(), window->height(), EGEImage::RGB_888);
  
  // create material with render texture for render texture object
  PMaterial material = ege_new Material(app());
  material->setDiffuseColor(Color::GREEN);
  material->addTexture(m_texture);
  m_sceneObjectRenderTexture->renderData()->setMaterial(material);

  // setup render texture
  PRenderTarget renderTarget = m_texture->renderTarget();
  if (renderTarget)
  {
    // add viewport
    viewport = renderTarget->addViewport("main", camera);
    viewport->setClearBufferTypes(Viewport::BUFFER_TYPE_COLOR);
    viewport->setClearColor(Color::RED);

    // NOTE: viewport color for render texture is used while rendering the scene for its purpose, so if normal scene has BLUE color, it wont appear in
    //       RTT as that viewport wont be clearing buffer but rather viewport of RTT.
  }

  ege_connect(app()->graphics(), preRender, this, RipplesTest::preRender);
  ege_connect(app()->graphics(), postRender, this, RipplesTest::postRender);

  return m_rippleEffect.initialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void RipplesTest::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called before target is rendered. */
void RipplesTest::preRender(PRenderTarget target)
{
  // check if target is texture
  if (RenderTarget::PRIORITY_RENDER_TEXTURE == target->priority())
  {
    // hide render texture quad
    m_sceneObjectRenderTexture->setVisible(false);

    // show scene object
    m_sceneObject->setVisible(true);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called after target has been rendered. */
void RipplesTest::postRender(PRenderTarget target)
{
  // check if target is texture
  if (RenderTarget::PRIORITY_RENDER_TEXTURE == target->priority())
  {
    // show render texture quad
    m_sceneObjectRenderTexture->setVisible(true);

    // hide scene object
    m_sceneObject->setVisible(false);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
