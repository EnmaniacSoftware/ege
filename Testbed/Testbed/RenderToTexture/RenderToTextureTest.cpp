#include "App.h"
#include "RenderToTextureTest.h"
#include <EGEGraphics.h>
#include <EGERenderSystem.h>
#include <EGESignal.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO
#define RENDER_TEXTURE_WINDOW_SIZE 200

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderToTextureTest::RenderToTextureTest(App* app) : Test(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderToTextureTest::~RenderToTextureTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String RenderToTextureTest::name() const
{
  return "Render To Texture";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool RenderToTextureTest::initialize()
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

  // create scene object(s)
  m_sceneObject = RenderObject::CreateTriangle(app(), "SceneObject", 200, true, false);
  m_sceneObjectRenderTexture = RenderObject::CreateRectangle(app(), "RenderTexture", RENDER_TEXTURE_WINDOW_SIZE, RENDER_TEXTURE_WINDOW_SIZE, false, true);
  if (NULL == m_sceneObject || NULL == m_sceneObjectRenderTexture)
  {
    // error!
    return false;
  }

  // attach object(s)
  SceneNode* node = app()->sceneManager()->rootNode()->createChildSceneNode(m_sceneObject->name());
  node->attachObject(m_sceneObject);
  node->physics()->setPosition(Vector4f(300, 300, 0));
  
  node = app()->sceneManager()->rootNode()->createChildSceneNode(m_sceneObjectRenderTexture->name());
  node->attachObject(m_sceneObjectRenderTexture);
  node->physics()->setPosition(Vector4f(window->width() - RENDER_TEXTURE_WINDOW_SIZE / 2.0f, window->height() - RENDER_TEXTURE_WINDOW_SIZE / 2.0f, 0));

  // create render texture

  // TAGE - for the time being set to CLAMP so we non-power of 2 render textures can be created for iOS
  app()->graphics()->renderSystem()->setTextureAddressingModeS(EGETexture::AM_CLAMP);
  app()->graphics()->renderSystem()->setTextureAddressingModeT(EGETexture::AM_CLAMP);
  app()->graphics()->renderSystem()->setTextureMagFilter(EGETexture::BILINEAR);
  app()->graphics()->renderSystem()->setTextureMinFilter(EGETexture::BILINEAR);
  
  // create render texture
  m_texture = app()->graphics()->hardwareResourceProvider()->createRenderTexture("rttTex", 512, 512, PF_RGB_888);

  PTextureImage textureImage = ege_new TextureImage(m_texture);

  // create material with render texture for render texture object
  PMaterial material = ege_new Material(app());
  RenderPass* pass = material->addPass(NULL);
  if (NULL == pass)
  {
    // error!
    return false;
  }

  material->setDiffuseColor(Color::GREEN);
  pass->addTexture(textureImage);
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

  ege_connect(app()->graphics(), preRender, this, RenderToTextureTest::preRender);
  ege_connect(app()->graphics(), postRender, this, RenderToTextureTest::postRender);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void RenderToTextureTest::update(const Time& time)
{
  static Angle angle;
  static const s32 orbitRadius = 150;
  static const s32 orbitalSpeed = 45;

  // get window render target
  PRenderWindow window = app()->graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);

  // update angle position
  angle += Angle::FromDegrees(time.seconds() * orbitalSpeed);
  angle.normalize();

  // set new position depending on angle with center at screen center
  m_sceneObject->parentNode()->physics()->setPosition(Vector4f(window->width() / 2 + Math::Sin(angle.radians()) * orbitRadius, 
                                                               window->height() / 2 + Math::Cos(angle.radians()) * orbitRadius, 0));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called before target is rendered. */
void RenderToTextureTest::preRender(PRenderTarget target)
{
  // check if target is texture
  if (RenderTarget::PRIORITY_RENDER_TEXTURE == target->priority())
  {
    // hide render texture quad
    m_sceneObjectRenderTexture->setVisible(false);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called after target has been rendered. */
void RenderToTextureTest::postRender(PRenderTarget target)
{
  // check if target is texture
  if (RenderTarget::PRIORITY_RENDER_TEXTURE == target->priority())
  {
    // show render texture quad
    m_sceneObjectRenderTexture->setVisible(true);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void RenderToTextureTest::pointerEvent(PPointerData data)
{
  EGE_UNUSED(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
