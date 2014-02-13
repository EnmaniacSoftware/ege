#include "ResourceItemTextureAtlas.h"
#include "ResourceLibrary.h"
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
static const QString KAttachedTag                = "Attached";
static const QString KCompressionFormatAttribute = "compression";
static const QString KFormatAttribute            = "format";
static const QString KWidthAttribute             = "width";
static const QString KHeightAttribute            = "height";
static const QString KUuidAttribute              = "uuid";

const QString ResourceItemTextureAtlas::KPropertyNameCompression = ResourceItemTextureAtlas::tr("Compression");
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting texture compression format type into string value. */
static QString GetTextureCompressionFormatAsText(ImageCompressionFormat format)
{
  QString value;

  ImageCompressionFormatInfoList list = SupportedImageCompressionFormats();
  for (int i = 0; i < list.size() && value.isEmpty(); ++i)
  {
    const ImageCompressionFormatInfo& info = list.at(i);

    if (info.format == format)
    {
      value = info.name;
    }
  }

  Q_ASSERT( ! value.isEmpty());

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting string to texture compression format type value. */
static ImageCompressionFormat GetTextureCompressionFormatFromText(QString name)
{
  ImageCompressionFormat value = EImageCompressionInvalid;

  ImageCompressionFormatInfoList list = SupportedImageCompressionFormats();
  for (int i = 0; i < list.size() && (EImageCompressionInvalid == value); ++i)
  {
    const ImageCompressionFormatInfo& info = list.at(i);

    if (info.name == name)
    {
      value = info.format;
    }
  }

  Q_ASSERT(EImageCompressionInvalid != value);

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting texture format type into string value. */
static QString GetTextureFormatAsText(QImage::Format format)
{
  QString value;

  ImageFormatInfoList list = SupportedImageFormats();
  for (int i = 0; i < list.size() && value.isEmpty(); ++i)
  {
    const ImageFormatInfo& info = list.at(i);

    if (info.format == format)
    {
      value = info.name;
    }
  }

  Q_ASSERT( ! value.isEmpty());

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting string to texture format type value. */
static QImage::Format GetTextureFormatFromText(QString name)
{
  QImage::Format value = QImage::Format_Invalid;

  ImageFormatInfoList list = SupportedImageFormats();
  for (int i = 0; i < list.size() && (QImage::Format_Invalid == value); ++i)
  {
    const ImageFormatInfo& info = list.at(i);

    if (info.name == name)
    {
      value = info.format;
    }
  }

  Q_ASSERT(QImage::Format_Invalid != value);

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemTextureAtlas::ResourceItemTextureAtlas(const QString& name, const QString& configurationName, const QUuid& id, ResourceItem* parent)
  : ResourceItemTexture(name, configurationName, id, parent)
  , m_compressionFormat(EImageCompressionPNG)
{
  ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
  Q_ASSERT(NULL != library);

  setType(ETexture2D);
  setImageFormat(QImage::Format_ARGB32);
  setSize(QSize(32, 32));

  // set addressing to CLAMP, REPEAT is not allowed
  setAddressModeS(EAddressModeClamp);
  setAddressModeT(EAddressModeClamp);

  // connect
  connect(library, SIGNAL(itemRemoved(ResourceItem*)), this, SLOT(onResourceLibraryModelItemRemoved(ResourceItem*)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemTextureAtlas::~ResourceItemTextureAtlas()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceItemTextureAtlas::Create(const QString& name, const QString& configurationName, const QUuid& id, ResourceItem* parent)
{
  return new ResourceItemTextureAtlas(name, configurationName, id, parent);
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
void ResourceItemTextureAtlas::setCompressionFormat(ImageCompressionFormat format)
{
  if (m_compressionFormat != format)
  {
    m_compressionFormat = format;

    // notify
    emit changed(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageCompressionFormat ResourceItemTextureAtlas::compressionFormat() const
{
  return m_compressionFormat;
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
bool ResourceItemTextureAtlas::serialize(QXmlStreamWriter& stream) const
{
  bool result = false;

  // begin serialization
  if (beginSerialize(stream))
  {
    // store texture specific data
    doSerialize(stream);

    // store atlas specific data
    stream.writeAttribute(KCompressionFormatAttribute, GetTextureCompressionFormatAsText(compressionFormat()));
    stream.writeAttribute(KFormatAttribute, GetTextureFormatAsText(imageFormat()));
    stream.writeAttribute(KWidthAttribute, QString("%1").arg(size().width()));
    stream.writeAttribute(KHeightAttribute, QString("%1").arg(size().height()));

    // store attached object ids
    for (int i = 0; i < m_textureIds.size(); ++i)
    {
      stream.writeStartElement(KAttachedTag);
      stream.writeAttribute(KUuidAttribute, m_textureIds.at(i).toString());
      stream.writeEndElement();
    }

    // end serialization
    result = endSerialize(stream);
  }

  return result && ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemTextureAtlas::unserialize(QXmlStreamReader& stream)
{
  // call base class
  bool result = ResourceItemTexture::unserialize(stream);
  if (result)
  {
    m_compressionFormat = GetTextureCompressionFormatFromText(stream.attributes().value(KCompressionFormatAttribute).toString());
    m_imageFormat = GetTextureFormatFromText(stream.attributes().value(KFormatAttribute).toString());
    m_size.setWidth(stream.attributes().value(KWidthAttribute).toInt());
    m_size.setHeight(stream.attributes().value(KHeightAttribute).toInt());

    // get any
    bool done = false;
    while ( ! stream.atEnd() && ! stream.hasError() && ! done)
    {
      QXmlStreamReader::TokenType token = stream.readNext();
      switch (token)
      {
        case QXmlStreamReader::StartElement:

          // check if attached tag
          if (KAttachedTag == stream.name())
          {
            // get resource item required data
            const QUuid id = QUuid(stream.attributes().value(KUuidAttribute).toString());

            m_textureIds.push_back(id);
          }
          break;

        case QXmlStreamReader::EndElement:

          // check if no more info for this
          // TAGE - need to find some way to either properly encapsulate this tag or make it somewhat accessible for all who need it
          if ("ResourceItem" == stream.name())
          {
            // done
            done = true;
          }
          break;

        default:
          break;
      }
    }
  }

  return result;
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

      // add COMPRESSION property
      addCompressionFormatDefinitions(definition);
    }
    else if (KGroupNameFiltering == definition.name())
    {
      // remove MIPMAPPING texture filtering
      result = definition.removeChildProperty(KPropertyNameMipMappingFiltering);
      Q_ASSERT(result);
    }
    // remove ATLASING group
    else if (KGroupNameAtlasing == definition.name())
    {
      list.removeAt(i);
      --i;
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

    ImageFormatInfoList list = SupportedImageFormats();
    setImageFormat(list.at(value.toInt()).format);
  }
  else if (KPropertyNameCompression == name)
  {
    Q_ASSERT(value.canConvert<int>());

    ImageCompressionFormatInfoList list = SupportedImageCompressionFormats();
    setCompressionFormat(list.at(value.toInt()).format);
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
void ResourceItemTextureAtlas::addCompressionFormatDefinitions(NPropertyObject::PropertyDefinition& group) const
{
  PropertyValueContainer values;

  int defaultValueIndex = 0;

  ImageCompressionFormatInfoList list = SupportedImageCompressionFormats();
  for (int i = 0; i < list.size(); ++i)
  {
    const ImageCompressionFormatInfo& info = list.at(i);

    // add to values
    values << info.displayName;
    values << QIcon();

    // check if current item is default one
    if (info.format == compressionFormat())
    {
      defaultValueIndex = i;
    }
  }

  // add all into main group
  group.addChildProperty(PropertyDefinition(KPropertyNameCompression, NPropertyObject::EEnum, values, defaultValueIndex));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTextureAtlas::attachObject(const QUuid& id)
{
  Q_ASSERT( ! m_textureIds.contains(id));
  m_textureIds.push_back(id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTextureAtlas::detachObject(const QUuid& id)
{
  Q_ASSERT(m_textureIds.contains(id));
  m_textureIds.removeOne(id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTextureAtlas::onResourceLibraryModelItemRemoved(ResourceItem* item)
{
  // check if currently set atlas texture
  if (m_textureIds.contains(item->id()))
  {
    // remove it
    detachObject(item->id());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
