#include "ResourceItemTexture.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceItemGroup.h"
#include "ResourceLibraryWindowResourceInserter.h"
#include <FileSystemUtils.h>
#include <ObjectPool.h>
#include <QImageReader>
#include <QDebug>
#include <QMenu>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const QString KPathArrtibute = "path";
static const QString KTypeArrtibute = "type";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct TextureTypeMap
{
  const QString name;
  const TextureType type;
};

static const TextureTypeMap l_textureTypeMappings[] = { { "none", EInvalidTexture },
                                                        { "2D", ETexture2D }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemTexture::ResourceItemTexture(const QString& name, const QString& configurationName, ResourceItem* parent)
  : ResourceItem(name, configurationName, parent),
    m_type(EInvalidTexture)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemTexture::~ResourceItemTexture()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceItemTexture::Create(const QString& name, const QString& configurationName, ResourceItem* parent)
{
  return new ResourceItemTexture(name, configurationName, parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemTexture::TypeName()
{
  return "texture";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::ResourceLibraryWindowHook(QMenu& menu, const QString& selectedType)
{
  // allow adding only for group resources
  if (selectedType == ResourceItemGroup::TypeName())
  {
    ResourceLibraryWindowResourceInserter* helper = ObjectPool::Instance()->getObject<ResourceLibraryWindowResourceInserter>();
    Q_ASSERT(NULL != helper);

    menu.addAction(tr("Add 2D texture"), helper, SLOT(onAddTexture2D()));
  }
  // allow properties for texture items only
  else if (selectedType == ResourceItemTexture::TypeName())
  {
    ResourceLibraryWindowResourceInserter* helper = ObjectPool::Instance()->getObject<ResourceLibraryWindowResourceInserter>();
    Q_ASSERT(NULL != helper);

    menu.addAction(tr("Properties"), helper, SLOT(onShowTextureProperties()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QImage& ResourceItemTexture::thumbnailImage() const
{
  // check if thumbnail is to be generated
  if (m_thumbnail.isNull() && ! m_path.isEmpty() && ! m_name.isEmpty())
  {
    // generate
    QImageReader imageReader(FileSystemUtils::Join(m_path, m_name));

    imageReader.setScaledSize(QSize(32, 32));
    imageReader.read(&m_thumbnail);
  }

  return m_thumbnail;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemTexture::typeName() const
{
  return ResourceItemTexture::TypeName();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QSize ResourceItemTexture::sizeHint() const
{
  return QSize(200, 40);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemTexture::serialize(QXmlStreamWriter& stream) const
{
  bool result = false;

  // begin serialization
  if (beginSerialize(stream))
  {
    // store data
    stream.writeAttribute(KTypeArrtibute, textureTypeName());
    stream.writeAttribute(KPathArrtibute, path());

    // end serialization
    result = endSerialize(stream);
  }

  return result && ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemTexture::unserialize(QXmlStreamReader& stream)
{
  // retrieve data
  m_path = stream.attributes().value(KPathArrtibute).toString();
  m_type = textureTypeFromString(stream.attributes().value(KTypeArrtibute).toString());

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::setPath(const QString& path)
{
  m_path = path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QString& ResourceItemTexture::path() const
{
  return m_path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::setType(TextureType type)
{
  m_type = type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QVariant ResourceItemTexture::data(int columnIndex, int role) const
{
  // call base class first
  QVariant variant = ResourceItem::data(columnIndex, role);
  
  // process according to role
  switch (role)
  {
    case ResourceLibraryDataModel::PathRole:

      variant = path();
      break;
  }

  return variant;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemTexture::setData(const QVariant &value, int role)
{
  // process according to role
  switch (role)
  {
    case ResourceLibraryDataModel::PathRole:

      m_path = value.toString();
      return true;
  }

  // call base class
  return ResourceItem::setData(value, role);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QList<NPropertyObject::PropertyDefinition> ResourceItemTexture::propertiesDefinition() const
{
  QList<NPropertyObject::PropertyDefinition> list;

  // create general group
  NPropertyObject::PropertyDefinition generalGroup(tr("General"), NPropertyObject::EGroup);


  // add groups to list
  list.push_back(generalGroup);

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemTexture::textureTypeName() const
{
  QString textureTypeName;

  // determine type name
  for (int i = 0; i < sizeof (l_textureTypeMappings) / sizeof (l_textureTypeMappings[0]); ++i)
  {
    const TextureTypeMap& mapping = l_textureTypeMappings[i];

    // check if type found in map table
    if (mapping.type == m_type)
    {
      // found
      textureTypeName = mapping.name;
      break;
    }
  }

  Q_ASSERT( ! textureTypeName.isEmpty());

  return textureTypeName;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureType ResourceItemTexture::textureTypeFromString(const QString& typeName) const
{
  TextureType textureType = EInvalidTexture;

  // determine type
  for (int i = 0; i < sizeof (l_textureTypeMappings) / sizeof (l_textureTypeMappings[0]); ++i)
  {
    const TextureTypeMap& mapping = l_textureTypeMappings[i];

    // check if name found in map table
    if (mapping.name == typeName)
    {
      // found
      textureType = mapping.type;
      break;
    }
  }

  return textureType;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
