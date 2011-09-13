#include "RippleEffect.h"
extern "C"
{
extern RIPPLE_VECTOR ripple_vector[GRID_SIZE_X][GRID_SIZE_Y];
extern RIPPLE_AMP ripple_amp[RIPPLE_LENGTH];

//#include "ripple.h"
}
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RippleEffect::RippleEffect(Application* app) : SceneNodeObject("ripple-effect"), m_app(app), m_state(STATE_IDLE)
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
  m_maxRipple = static_cast<s32>(Math::Sqrt(width * width + height * height));
  
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


  for (s32 i = 0; i < RIPPLE_COUNT; i++)
  {
    t[i] = m_maxRipple + RIPPLE_LENGTH;
    cx[i] = 0;
    cy[i] = 0;
    max[i] = 0;
  }

  // setup render data
  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, GRID_SIZE_X * GRID_SIZE_Y));
  for (s32 i = 0; i < GRID_SIZE_X; i++)
  {
    for (s32 j = 0; j < GRID_SIZE_Y; j++)
    {
      *data++ = i/(GRID_SIZE_X - 1.0f)*width;
      *data++ = j/(GRID_SIZE_Y - 1.0f)*height;
      *data++ = 0.0f;

      dt[i][j][0] = i/(GRID_SIZE_X - 1.0f);
      dt[i][j][1] = j/(GRID_SIZE_Y - 1.0f);

      *data++ = dt[i][j][0];
      *data++ = dt[i][j][1];
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
  s32 x, y;
  s32 mi, mj;
  s32 r;
  float32 sx, sy;
  float32 amp;

  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, GRID_SIZE_X * GRID_SIZE_Y));

  for (s32 i = 0; i < RIPPLE_COUNT; i++)
  {
    t[i] += RIPPLE_STEP;
  }

  for (s32 i = 0; i < GRID_SIZE_X; i++)
  {
    for (s32 j = 0; j < GRID_SIZE_Y; j++)
    {
      data += 3;

      *data = dt[i][j][0];
      *(data + 1) = dt[i][j][1];

      for (s32 k = 0; k < RIPPLE_COUNT; k++)
	    {
		    // difference between current grid cell and center cell of ripple
		    x = i - cx[k];
		    y = j - cy[k];

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

		    mi = x;
		    mj = y;
		
		    r = t[k] - ripple_vector[mi][mj].r;
		
		    if (r < 0)
        {
		      r = 0;
        }

		    if (r > RIPPLE_LENGTH - 1)
        {
		      r = RIPPLE_LENGTH - 1;
        }

		    amp = 1.0f - 1.0f*t[k]/RIPPLE_LENGTH;
		    amp *= amp;
		    if (amp < 0.0f)
        {
		      amp = 0.0f;
        }

		    *data       += ripple_vector[mi][mj].dx[0]*sx*ripple_amp[r].amplitude*amp;
		    *(data + 1) += ripple_vector[mi][mj].dx[1]*sy*ripple_amp[r].amplitude*amp;
      }

      data += 2;
    }
  }

  m_renderData->vertexBuffer()->unlock();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
	Calculate the distance between two points.
*/

float ripple_distance(int gx, int gy, int cx, int cy)
{
  return sqrtf(1.0f*(gx - cx)*(gx - cx) + 1.0f*(gy - cy)*(gy - cy));
}

/*
	Compute the distance of the given window coordinate
	to the nearest window corner, in pixels.
*/

int ripple_max_distance(int gx, int gy)
{
  float d;
  float temp_d;

  d = ripple_distance(gx, gy, 0, 0);
  temp_d = ripple_distance(gx, gy, GRID_SIZE_X, 0);
  if (temp_d > d)
    d = temp_d;
  temp_d = ripple_distance(gx, gy, GRID_SIZE_X, GRID_SIZE_Y);
  if (temp_d > d)
    d = temp_d;
  temp_d = ripple_distance(gx, gy, 0, GRID_SIZE_Y);
  if (temp_d > d)
    d = temp_d;

  return (d/GRID_SIZE_X)*400/*win_size_x*/ + RIPPLE_LENGTH/6;
}
/*! Pointer event receiver. */
void RippleEffect::pointerEvent(PPointerData data)
{
  if (EGEInput::ACTION_BUTTON_UP == data->action())
  {
    s32 index = 0;
    while (t[index] < max[index] && index < RIPPLE_COUNT)
      index++;
    
    if (index < RIPPLE_COUNT)
    {
      cx[index] = 1.0f*data->x()/400*GRID_SIZE_X;
      cy[index] = 1.0f*data->y()/400*GRID_SIZE_Y;
      t[index] = 4*RIPPLE_STEP;
      max[index] = ripple_max_distance(cx[index], cy[index]);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
