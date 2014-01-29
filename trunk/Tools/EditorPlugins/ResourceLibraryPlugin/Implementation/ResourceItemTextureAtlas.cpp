#include "ResourceItemTextureAtlas.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceItemGroup.h"
#include "ResourceLibraryWindowResourceInserter.h"
#include "ImageFormats.h"
#include <PropertyValueHelper.h>
#include <ObjectPool.h>
#include <QVector>
#include <QRgb>
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

  // set addressing to CLAMP, REPEAT is not allowed
  setAddressModeS(EAddressModeClamp);
  setAddressModeT(EAddressModeClamp);
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
  static const QColor KDefaultColor = Qt::magenta;

  // TAGE - ideally do it in another thread
  m_image = QImage(size(), imageFormat());

  // setup color table in case of indexed format
  if (imageFormat() == QImage::Format_Indexed8)
  {
    QVector<QRgb> colorTable;
    colorTable << KDefaultColor.rgb();
    m_image.setColorTable(colorTable);
  }

  // fill in initially
  m_image.fill(KDefaultColor);

  emit changed(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyDefinition ResourceItemTextureAtlas::createTextureFormatsDefinition() const
{
  PropertyValueContainer values;

  int defaultValueIndex = 0;

  // create magnification filter values
  ImageFormatInfoList supportedImageFormats = SupportedImageFormats();
  for (int i = 0; i < supportedImageFormats.size(); ++i)
  {
    const ImageFormatInfo& info = supportedImageFormats.at(i);

    values << info.displayName;
    values << QIcon();

    // check if this is current setting
    if (imageFormat() == info.format)
    {
      defaultValueIndex = i;
    }
  }

  return PropertyDefinition(KPropertyNameImageFormat, NPropertyObject::EEnum, values, defaultValueIndex);
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

    if (KGroupNameInfo == definition.name())
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

      // replace FORMAT property with EEnum type
      result = definition.replaceChildProperty(KPropertyNameImageFormat, createTextureFormatsDefinition());
      Q_ASSERT(result);
    }
    else if (KGroupNameFiltering == definition.name())
    {
      // remove MIPMAPPING texture filtering
      result = definition.removeChildProperty(KPropertyNameMipMappingFiltering);
      Q_ASSERT(result);
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
  else if (KPropertyNameImageFormat == name)
  {
    Q_ASSERT(value.canConvert<int>());

    ImageFormatInfoList supportedImageFormats = SupportedImageFormats();
    setImageFormat(supportedImageFormats.at(value.toInt()).format);
  }
  else
  {
    // call base class
    ResourceItemTexture::update(name, value);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTextureAtlas::addAddressingModeDefinitions(NPropertyObject::PropertyDefinition& group) const
{
  PropertyValueContainer values;

  TextureAddressingModeInfoList list = SupportedTextureAddressingModes();
  for (int i = 0; i < list.size(); ++i)
  {
    const TextureAddressingModeInfo& info = list.at(i);

    // check if CLAMP mode as we dont allow any other for atlases
    if (info.mode == EAddressModeClamp)
    {
      // add to values
      values << info.displayName;
      values << QIcon();
    }
  }

  // add all into main group
  group.addChildProperty(PropertyDefinition(KPropertyNameAddressingModeS, NPropertyObject::EEnum, values, 0));
  group.addChildProperty(PropertyDefinition(KPropertyNameAddressingModeT, NPropertyObject::EEnum, values, 0));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
