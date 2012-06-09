#include "App.h"
#include "ImagedAnimationTest.h"
#include <EGESignal.h>
#include <EGEResources.h>
#include <EGEOverlay.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimationTest::ImagedAnimationTest(App* app) : Test(app)
{
  ege_connect(app->graphics(), preRender, this, ImagedAnimationTest::preRender);

  PResourceFont fontResource = app->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    PTextOverlay overlay = ege_new TextOverlay(app, "frame");
    overlay->setFont(fontResource->font());
    app->overlayManager()->add(overlay);
    overlay->physics()->setPosition(Vector4f(0, 80, 0));

    //overlay = ege_new TextOverlay(app, "text-1");
    //overlay->setFont(fontResource->font());
    //app->overlayManager()->add(overlay);
    //overlay->physics()->setPosition(Vector4f(40, 120, 0));

    //overlay = ege_new TextOverlay(app, "text-2");
    //overlay->setFont(fontResource->font());
    //app->overlayManager()->add(overlay);
    //overlay->physics()->setPosition(Vector4f(40, 140, 0));

    //overlay = ege_new TextOverlay(app, "text-3");
    //overlay->setFont(fontResource->font());
    //app->overlayManager()->add(overlay);
    //overlay->physics()->setPosition(Vector4f(40, 160, 0));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimationTest::~ImagedAnimationTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String ImagedAnimationTest::name() const
{
  return "Imaged Animation";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool ImagedAnimationTest::initialize()
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
  if (EGE_SUCCESS != app()->resourceManager()->loadGroup("imaged-animation-test"))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void ImagedAnimationTest::update(const Time& time)
{
  for (List<PImagedAnimation>::iterator it = m_anims.begin(); it != m_anims.end(); ++it)
  {
    PImagedAnimation anim = *it;
    anim->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void ImagedAnimationTest::pointerEvent(PPointerData data)
{
  if (EGEInput::ACTION_BUTTON_UP == data->action())
  {
    for (List<PImagedAnimation>::iterator it = m_anims.begin(); it != m_anims.end(); ++it)
    {
      PImagedAnimation anim = *it;

      if (anim)
      {
        if (!anim->isPlaying())
        {
          anim->play();
        }
        else
        {
          anim->stop();
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Slot called when resource group has been loaded. */
void ImagedAnimationTest::groupLoadComplete(const String& name)
{
  if ("imaged-animation-test" == name)
  {
    PResourceImagedAnimation animRes;

    //animRes = app()->resourceManager()->resource(RESOURCE_NAME_IMAGED_ANIMATION, "static_banana");
    //EGE_ASSERT(animRes);
    //m_anims.push_back(animRes->createInstance());

    //animRes = app()->resourceManager()->resource(RESOURCE_NAME_IMAGED_ANIMATION, "translate_scale_banana");
    //EGE_ASSERT(animRes);
    //m_anims.push_back(animRes->createInstance());

    //animRes = app()->resourceManager()->resource(RESOURCE_NAME_IMAGED_ANIMATION, "004_translate_2images");
    //EGE_ASSERT(animRes);
    //m_anims.push_back(animRes->createInstance());

    //animRes = app()->resourceManager()->resource(RESOURCE_NAME_IMAGED_ANIMATION, "blink");
    //EGE_ASSERT(animRes);
    //m_anims.push_back(animRes->createInstance());

    //animRes = app()->resourceManager()->resource(RESOURCE_NAME_IMAGED_ANIMATION, "idle");
    //EGE_ASSERT(animRes);
    //m_anims.push_back(animRes->createInstance());

    animRes = app()->resourceManager()->resource(RESOURCE_NAME_IMAGED_ANIMATION, "lvl1_idle");
    EGE_ASSERT(animRes);
    m_anims.push_back(animRes->createInstance());
    ege_connect(m_anims.back(), frameChanged, this, ImagedAnimationTest::onFrameChanged);

    //animRes = app()->resourceManager()->resource(RESOURCE_NAME_IMAGED_ANIMATION, "chomp");
    //EGE_ASSERT(animRes);
    //m_anims.push_back(animRes->createInstance());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called before target is rendered. */
void ImagedAnimationTest::preRender(PRenderTarget target)
{
  EGE_UNUSED(target);

  s32 i = 0;
  for (List<PImagedAnimation>::iterator it = m_anims.begin(); it != m_anims.end(); ++it, ++i)
  {
    PImagedAnimation anim = *it;
  
    Matrix4f matrix = Matrix4f::IDENTITY;
  //  matrix.setTranslation(100, i * 300.0f, 0);
   // matrix.setScale(2, 2, 1);

    anim->addForRendering(app()->graphics()->renderer(), matrix);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimationTest::onFrameChanged(PImagedAnimation anim, s32 frameId)
{
  ege_cast<TextOverlay*>(app()->overlayManager()->overlay("frame"))->setText(Text::Format("Frame: %d", frameId));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
