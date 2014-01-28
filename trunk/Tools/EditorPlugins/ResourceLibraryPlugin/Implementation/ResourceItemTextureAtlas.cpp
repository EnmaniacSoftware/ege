#include "ResourceItemTextureAtlas.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceItemGroup.h"
#include "ResourceLibraryWindowResourceInserter.h"
#include <PropertyValueHelper.h>
#include <ObjectPool.h>
#include <QDebug>
#include <QMenu>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
using NPropertyObject::PropertyDefinition;
using NPropertyObject::PropertyValueContainer;
using NPropertyObject::PropertyValueHelper;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//struct TextureFormatTypeMap
//{
//  const QString name;
//  const QString displayName;
//  const TextureFilterType type;
//};

//static const TextureFilterTypeMap l_textureFilterTypeMappings[] = { { "nearest", ResourceItemTexture::tr("Nearest"), ETextureFilterNearest },
//                                                                    { "bilinear", ResourceItemTexture::tr("Bilinear"), ETextureFilterBilinear }
//};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemTextureAtlas::ResourceItemTextureAtlas(const QString& name, const QString& configurationName, ResourceItem* parent)
  : ResourceItemTexture(name, configurationName, parent)
{
  setType(ETexture2D);
  setImageFormat(QImage::Format_ARGB32);
  setSize(QSize(32, 32));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemTextureAtlas::~ResourceItemTextureAtlas()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceItemTextureAtlas::Create(const QString& name, const QString& configurationName, ResourceItem* parent)
{
  return new ResourceItemTextureAtlas(name, configurationName, parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemTextureAtlas::TypeName()
{
  return "texture-atlas";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemTextureAtlas::typeName() const
{
  return ResourceItemTextureAtlas::TypeName();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTextureAtlas::ResourceLibraryWindowHook(QMenu& menu, const QString& selectedType)
{
  // allow adding only for group resources
  if (selectedType == ResourceItemGroup::TypeName())
  {
    ResourceLibraryWindowResourceInserter* helper = ObjectPool::Instance()->getObject<ResourceLibraryWindowResourceInserter>();
    Q_ASSERT(NULL != helper);

    menu.addAction(tr("Add texture atlas"), helper, SLOT(onAddTextureAtlas()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QImage&ResourceItemTextureAtlas::thumbnailImage() const
{
  // check if thumbnail is to be generated
  if (m_thumbnail.isNull())
  {
    // generate
    m_thumbnail = m_image.scaled(QSize(32, 32));
  }

  return m_thumbnail;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTextureAtlas::setSize(const QSize& size)
{
  if (m_size != size)
  {
    m_size = size;

    // rebuild
    rebuild();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTextureAtlas::setImageFormat(QImage::Format format)
{
  if (m_imageFormat != format)
  {
    m_imageFormat = format;

    // rebuild
    rebuild();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTextureAtlas::rebuild()
{
  // TAGE - ideally do it in another thread
  m_image = QImage(size(), imageFormat());

  emit changed(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTextureAtlas::addTextureFormatsDefinitions(NPropertyObject::PropertyDefinition& group) const
{
//  PropertyValueContainer values;

//  // create magnification filter values
//  for (unsigned int i = 0; i < sizeof (l_textureFilterTypeMappings) / sizeof (l_textureFilterTypeMappings[0]); ++i)
//  {
//    const TextureFilterTypeMap& currentFilter = l_textureFilterTypeMappings[i];

//    values << currentFilter.displayName;
//    values << QIcon();
//  }

//  // add all into main group
//  group.addChildProperty(PropertyDefinition(KPropertyNameMagnifyingFiltering, NPropertyObject::EEnum, values,
//                                            GetMagnificationFilterMapIndex(m_magnificationFilter)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QSize ResourceItemTextureAtlas::size() const
{
  return m_size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTextureAtlas::onInvalidate()
{
  m_thumbnail = QImage();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QList<NPropertyObject::PropertyDefinition> ResourceItemTextureAtlas::propertiesDefinition() const
{
  // call base class first
  QList<PropertyDefinition> list = ResourceItemTexture::propertiesDefinition();
  Q_ASSERT(0 < list.size());

  PropertyValueContainer values;

  bool result;

  // update properties of base class
  for (int i = 0; i < list.size(); ++i)
  {
    PropertyDefinition& definition = list[i];

    if (definition.name() == ResourceItemTexture::tr("Info"))
    {
      // replace WIDTH property with non-read only version
      PropertyDefinition property = definition.findChildProperty(KPropertyNameWidth);
      Q_ASSERT(property.isValid());

      property = PropertyDefinition(property.name(), property.type(), property.values(), property.defaultValue(), false);
      result = definition.replaceChildProperty(property.name(), property);
      Q_ASSERT(result);

      // replace HEIGHT property with non-read only version
      property = definition.findChildProperty(KPropertyNameHeight);
      Q_ASSERT(property.isValid());

      property = PropertyDefinition(property.name(), property.type(), property.values(), property.defaultValue(), false);
      result = definition.replaceChildProperty(property.name(), property);
      Q_ASSERT(result);

      // replace LOCATION property with EDirectoryPath type
      values.clear();
      values << fullPath();
      property = PropertyDefinition(KPropertyNameLocation, NPropertyObject::EDirectoryPath, values);
      result = definition.replaceChildProperty(KPropertyNameLocation, property);
      Q_ASSERT(result);

      // replace DEPTH with

      // add texture format
      addTextureFormatsDefinitions(definition);
    }
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTextureAtlas::update(const QString& name, const QVariant& value)
{
  if (KPropertyNameWidth == name)
  {
    Q_ASSERT(value.canConvert<int>());
    setSize(QSize(value.toInt(), size().height()));
  }
  else if (KPropertyNameHeight == name)
  {
    Q_ASSERT(value.canConvert<int>());
    setSize(QSize(size().width(), value.toInt()));
  }
  else
  {
    // call base class
    ResourceItemTexture::update(name, value);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
