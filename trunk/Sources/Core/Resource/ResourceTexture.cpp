#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceManager.h"
#include "EGEXml.h"

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceTexture)
EGE_DEFINE_DELETE_OPERATORS(ResourceTexture)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps literal filter function name into numeric value. */
static EGETexture::Filter mapFilterName(const EGEString& name)
{
  if ("trilinear" == name)
  {
    return EGETexture::TRILINEAR;
  }
  else if ("mipmap_bilinear" == name)
  {
    return EGETexture::MIPMAP_BILINEAR;
  }
  else if ("mipmap_trilinear" == name)
  {
    return EGETexture::MIPMAP_TRILINEAR;
  }

  return EGETexture::BILINEAR;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps literal wrap mode name into numeric value. */
static EGETexture::Wrap mapWrapName(const EGEString& name)
{
  if ("clamp" == name)
  {
    return EGETexture::CLAMP;
  }

  return EGETexture::REPEAT;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTexture::ResourceTexture(Application* app, ResourceManager* manager) : IResource(app, manager, "texture")
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTexture::~ResourceTexture()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Creates instance of resource. This method is a registration method for manager. */
PResource ResourceTexture::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceTexture(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const EGEString& ResourceTexture::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceTexture::create(const EGEString& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // get data
  m_name      = tag->attribute("name");
  m_path      = tag->attribute("path");
  m_type      = tag->attribute("type").toLower();
  m_minFilter = tag->attribute("minFilter").toLower();
  m_magFilter = tag->attribute("magFilter").toLower();
  m_wrapS     = tag->attribute("wrapS").toLower();
  m_wrapT     = tag->attribute("wrapT").toLower();

  // check if obligatory data is wrong
  if (m_name.empty() || m_path.empty() || m_type.empty())
  {
    // error!
    return EGE_ERROR_BAD_PARAM;
  }

  // compose absolute path
  m_path = path + "/" + m_path;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceTexture::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    // create according to type
    if ("2d" == type())
    {
      result = create2D();
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates 2D texture. */
EGEResult ResourceTexture::create2D()
{
  EGEResult result = EGE_SUCCESS;

  PTexture2D texture = ege_new Texture2D(app());
  if (NULL == texture)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // sets parameters
  texture->setMinFilter(mapFilterName(minFilter()));
  texture->setMagFilter(mapFilterName(magFilter()));
  texture->setWrapS(mapWrapName(wrapS()));
  texture->setWrapT(mapWrapName(wrapT()));

  // create it
  result = texture->create(path());
  
  // assign
  m_texture = texture;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceTexture::unload() 
{ 
  m_texture = NULL; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
