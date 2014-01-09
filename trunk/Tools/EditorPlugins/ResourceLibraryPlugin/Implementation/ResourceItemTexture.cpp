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
    m_type(EInvalidTexture),
    m_size(QSize(0, 0)),
    m_imageFormat(QImage::Format_Invalid)
{
  connect(this, SIGNAL(changed(const ResourceItem*)), this, SLOT(onInvalidate()));
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
  if (m_path != path)
  {
    m_path = path;

    // emit
    emit changed(this);
  }
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

      setPath(value.toString());
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

  // add width, height and bpp
  generalGroup.addChildProperty(NPropertyObject::PropertyDefinition(tr("Width"), NPropertyObject::EString, QString("%1 px").arg(size().width()), true));
  generalGroup.addChildProperty(NPropertyObject::PropertyDefinition(tr("Height"), NPropertyObject::EString, QString("%1 px").arg(size().height()), true));
  generalGroup.addChildProperty(NPropertyObject::PropertyDefinition(tr("Depth"), NPropertyObject::EString, imageFormatAsText(), true));

  // add groups to list
  list.push_back(generalGroup);

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::update(const QString& name, const QVariant& value)
{
  // call base class
  ResourceItem::update(name, value);
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
QSize ResourceItemTexture::size() const
{
  // check if size can be determined
  if (m_size.isNull() && ! m_path.isEmpty() && ! m_name.isEmpty())
  {
    QImageReader imageReader(FileSystemUtils::Join(m_path, m_name));

    m_size = imageReader.size();
  }

  return m_size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QImage::Format ResourceItemTexture::imageFormat() const
{
  // check if format can be determined
  if ((QImage::Format_Invalid == m_imageFormat) && ! m_path.isEmpty() && ! m_name.isEmpty())
  {
    QImageReader imageReader(FileSystemUtils::Join(m_path, m_name));

    m_imageFormat = imageReader.imageFormat();
  }

  return m_imageFormat;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemTexture::imageFormatAsText() const
{
  QString text;

  switch (imageFormat())
  {
    case QImage::Format_Invalid: text = tr("Imvalid"); break;
    case QImage::Format_Indexed8: text = tr("Indexed 8-bit"); break;
    case QImage::Format_RGB32: text = tr("32-bit RGB"); break;
    case QImage::Format_ARGB32: text = tr("32-bit ARGB"); break;
    case QImage::Format_ARGB32_Premultiplied: text = tr("Premultiplied 32-bit ARGB"); break;
    case QImage::Format_RGB16: text = tr("16-bit RGB (565)"); break;
    case QImage::Format_ARGB8565_Premultiplied: text = tr("Premultiplied 24-bit ARGB (8565"); break;
    case QImage::Format_RGB555: text = tr("15-bit RGB (555"); break;

    default:

      text = tr("Unspecified");
      break;
  }

  return text;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::onInvalidate()
{
  m_size        = QSize(0, 0);
  m_imageFormat = QImage::Format_Invalid;
  m_thumbnail   = QImage();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
