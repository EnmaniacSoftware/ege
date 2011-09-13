#include "RippleEffect.h"
extern "C"
{
extern RIPPLE_VECTOR ripple_vector[GRID_SIZE_X][GRID_SIZE_Y];
extern RIPPLE_AMP ripple_amp[RIPPLE_LENGTH];
}
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RippleEffect::RippleEffect(Application* app) : SceneNodeObject("ripple-effect"), m_app(app), m_state(STATE_IDLE), m_speed(500)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RippleEffect::~RippleEffect()
{
  m_texture = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes object. */
bool RippleEffect::initialize(s32 width, s32 height, PCamera camera)
{
  m_width  = width;
  m_height = height;
  
  // create render texture
  m_texture = Texture2D::CreateRenderTexture(m_app, "RippleEffect::rttTex", width, height, EGEImage::RGB_888);

  // setup render texture
  PRenderTarget renderTarget = m_texture->renderTarget();
  if (NULL == renderTarget)
  {
    // error!
    return false;
  }

  // add viewport for render target
  if (NULL == renderTarget->addViewport("main", camera))
  {
    // error!
    return false;
  }

  // create render data
  m_renderData = ege_new RenderComponent(m_app, name(), EGEGraphics::RP_MAIN);
  if ((NULL == m_renderData) || !m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_T2) ||
      !m_renderData->indexBuffer()->create(IndexBuffer::SIZE_16BIT, 6 * GRID_SIZE_X * GRID_SIZE_Y))
  {
    // error!
    return false;
  }

  // setup index data
  u16* indexData = reinterpret_cast<u16*>(m_renderData->indexBuffer()->lock(0, 6 * (GRID_SIZE_X - 1) * (GRID_SIZE_Y - 1)));
  for (u16 i = 0; i < GRID_SIZE_X - 1; ++i)
  {
    for (s16 j = 0; j < GRID_SIZE_Y - 1; ++j)
    {
      *indexData++ = i * GRID_SIZE_X + j;
      *indexData++ = i * GRID_SIZE_X + j + 1;
      *indexData++ = i * GRID_SIZE_X + j + 1 + GRID_SIZE_Y;
      *indexData++ = i * GRID_SIZE_X + j;
      *indexData++ = i * GRID_SIZE_X + j + 1 + GRID_SIZE_Y;
      *indexData++ = i * GRID_SIZE_X + j + GRID_SIZE_Y;
    }
  }
  m_renderData->indexBuffer()->unlock();

  // create material with render texture for render texture object
  PMaterial material = ege_new Material(app());
  material->setDiffuseColor(Color::GREEN);
  material->addTexture(m_texture);
  m_renderData->setMaterial(material);

  // initialize ripple data
  for (s32 i = 0; i < RIPPLE_COUNT; ++i)
  {
    m_times[i]    = static_cast<s32>(Math::Sqrt(width * width + height * height)) + RIPPLE_LENGTH;
    m_maxTimes[i] = 0;
    m_centers[i].set(0, 0);
  }

  // setup render data
  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, GRID_SIZE_X * GRID_SIZE_Y));
  for (s32 i = 0; i < GRID_SIZE_X; i++)
  {
    for (s32 j = 0; j < GRID_SIZE_Y; j++)
    {
      // position (constant all the time)
      *data++ = i / (GRID_SIZE_X - 1.0f) * width;
      *data++ = j / (GRID_SIZE_Y - 1.0f) * height;
      *data++ = 0.0f;

      // default texture coords
      m_defaultTextureCoords[i][j].set(i / (GRID_SIZE_X - 1.0f), j / (GRID_SIZE_Y - 1.0f));

      // current texture coords
      *data++ = m_defaultTextureCoords[i][j].x;
      *data++ = m_defaultTextureCoords[i][j].y;
    }
  }
  m_renderData->vertexBuffer()->unlock();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SceneNodeObject override. Adds object render data for rendering with given renderer. */
bool RippleEffect::addForRendering(Renderer* renderer)
{
  return renderer->addForRendering(parentNode()->worldMatrix(), m_renderData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates effect. */
void RippleEffect::update(const Time& time)
{
  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, GRID_SIZE_X * GRID_SIZE_Y));

  // advance animation time
  for (s32 i = 0; i < RIPPLE_COUNT; i++)
  {
    m_times[i] += static_cast<s32>(time.seconds() * m_speed);
  }

  // recalculate grid corner texture coords
  for (s32 i = 0; i < GRID_SIZE_X; i++)
  {
    for (s32 j = 0; j < GRID_SIZE_Y; j++)
    {
      // skip position
      data += 3;

      // reset texture coords to default value
      *data       = m_defaultTextureCoords[i][j].x;
      *(data + 1) = m_defaultTextureCoords[i][j].y;

      // blend all ripples together
      for (s32 k = 0; k < RIPPLE_COUNT; ++k)
	    {
        float32 sx;
        float32 sy;

		    // difference between current grid cell and center cell of ripple
		    s32 x = i - m_centers[k].x;
		    s32 y = j - m_centers[k].y;

		    if (x < 0)
		    {
		      x *= -1;
		      sx = -1.0f;
		    }
		    else
        {
		      sx = 1.0;
        }

		    if (y < 0)
		    {
		      y *= -1;
		      sy = -1.0f;
		    }
		    else
        {
		      sy = 1.0f;
        }

		    s32 mi = x;
		    s32 mj = y;
		
		    s32 r = m_times[k] - ripple_vector[mi][mj].r;
		
		    if (r < 0)
        {
		      r = 0;
        }

		    if (r > RIPPLE_LENGTH - 1)
        {
		      r = RIPPLE_LENGTH - 1;
        }

		    float32 amp = 1.0f - 1.0f * m_times[k] / RIPPLE_LENGTH;
		    amp *= amp;
		    if (0.0f > amp)
        {
		      amp = 0.0f;
        }
        
        // add contribution of current ripple to texture coords
		    *data       += ripple_vector[mi][mj].dx[0] * sx * ripple_amp[r].amplitude * amp;
		    *(data + 1) += ripple_vector[mi][mj].dx[1] * sy * ripple_amp[r].amplitude * amp;
      }

      // go over texture coords
      data += 2;
    }
  }

  m_renderData->vertexBuffer()->unlock();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*!	Computes the distance of the given window coordinate to the nearest window corner (in pixels). */
s32 RippleEffect::rippleMaxDistance(const Vector2i& pos) const
{
  float32 d;
  float32 temp_d;

  Vector2i corner(0, 0);

  // get distance to TOP-LEFT corner
  d = pos.distanceTo(corner);

  // get distance to TOP-RIGHT corner
  corner.set(GRID_SIZE_X, 0);
  temp_d = pos.distanceTo(corner);
  if (temp_d > d)
  {
    // new greater distance found
    d = temp_d;
  }

  // get distance to BOTTOM-RIGHT corner
  corner.set(GRID_SIZE_X, GRID_SIZE_Y);
  temp_d = pos.distanceTo(corner);
  if (temp_d > d)
  {
    // new greater distance found
    d = temp_d;
  }

  // get distance to BOTTOM-LEFT cornet
  corner.set(0, GRID_SIZE_Y);
  temp_d = pos.distanceTo(corner);
  if (temp_d > d)
  {
    // new greater distance found
    d = temp_d;
  }

  return static_cast<s32>((d / GRID_SIZE_X) * m_width + RIPPLE_LENGTH / 6);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event receiver. */
void RippleEffect::pointerEvent(PPointerData data)
{
  if (EGEInput::ACTION_BUTTON_UP == data->action())
  {
    s32 index = 0;
    while (m_times[index] < m_maxTimes[index] && (RIPPLE_COUNT > index))
    {
      index++;
    }

    // check if empty spot found
    if (RIPPLE_COUNT > index)
    {
      m_centers[index].set(data->x() * GRID_SIZE_X / m_width, data->y() * GRID_SIZE_Y / m_height);
      m_times[index]    = 4 * RIPPLE_STEP;
      m_maxTimes[index] = rippleMaxDistance(m_centers[index]);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets speed. */
void RippleEffect::setSpeed(s32 speed)
{
  EGE_ASSERT(0 < speed);
  m_speed = speed;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
