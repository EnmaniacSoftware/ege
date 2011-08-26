#include "Core/Debug/Debug.h"
#include "Core/Debug/Logger.h"
#include "Core/Debug/Console.h"
#include "Core/Components/Render/RenderComponent.h"
#include <EGESpline.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Debug)
EGE_DEFINE_DELETE_OPERATORS(Debug)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

Logger* Debug::m_log = NULL;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug::Debug(Application* app) : Object(app)
{
  m_console = ege_new Console(app);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug::~Debug()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::Deinit()
{
  if (Debug::m_log)
  {
    delete Debug::m_log;
    Debug::m_log = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::Log(const String& text)
{
  if (NULL == Debug::m_log)
  {
    Debug::m_log = ege_new Logger("engine.log", true);
  }

  if (Debug::m_log)
  {
    Debug::m_log->write(text);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates renderable for given object. */
RenderComponent* Debug::renderable(const CubicSpline* spline)
{
  s32 vertexCount = 25;

  RenderComponent* component = ege_new RenderComponent(NULL, "debug-renderable-cubic-spline", EGEGraphics::RP_MAIN, 
                                                       EGEGraphics::RPT_LINES);
  if (component && component->isValid())
  {
    if (component->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_C4))
    {
      float32* data = (float32*) component->vertexBuffer()->lock(0, vertexCount * 2 + 2 * 2);

      Vector4f pos;

	    for (s32 i = 0; i < vertexCount; ++i)
	    {
        spline->value(pos, i / (1.0f * vertexCount));

        *data++ = pos.x;
        *data++ = pos.y;
        *data++ = pos.z;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;

        spline->value(pos, (i + 1) / (1.0f * vertexCount));

        *data++ = pos.x;
        *data++ = pos.y;
        *data++ = pos.z;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
      }

      // tangents
      //spline->value(pos, 0);
      //*data++ = pos.x;
      //*data++ = pos.y;
      //*data++ = pos.z;
      //*data++ = 1.0f;
      //*data++ = 0.0f;
      //*data++ = 0.0f;
      //*data++ = 1.0f;

      //pos = spline->controlPoint(1);

      //*data++ = pos.x;
      //*data++ = pos.y;
      //*data++ = pos.z;
      //*data++ = 1.0f;
      //*data++ = 0.0f;
      //*data++ = 0.0f;
      //*data++ = 1.0f;

      //spline->value(pos, 1);
      //*data++ = pos.x;
      //*data++ = pos.y;
      //*data++ = pos.z;
      //*data++ = 1.0f;
      //*data++ = 0.0f;
      //*data++ = 0.0f;
      //*data++ = 1.0f;

      //pos = spline->controlPoint(2);

      //*data++ = pos.x;
      //*data++ = pos.y;
      //*data++ = pos.z;
      //*data++ = 1.0f;
      //*data++ = 0.0f;
      //*data++ = 0.0f;
      //*data++ = 1.0f;

      component->vertexBuffer()->unlock();
    }
  }

  return component;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
