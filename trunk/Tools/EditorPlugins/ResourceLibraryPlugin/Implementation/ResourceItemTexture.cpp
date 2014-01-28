#include "ResourceItemTexture.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceItemGroup.h"
#include "ResourceLibraryWindowResourceInserter.h"
#include <PropertyValueHelper.h>
#include <FileSystemUtils.h>
#include <ObjectPool.h>
#include <QImageReader>
#include <QDebug>
#include <QMenu>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
using NPropertyObject::PropertyDefinition;
using NPropertyObject::PropertyValueContainer;
using NPropertyObject::PropertyValueHelper;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const QString KPathArrtibute           = "path";
static const QString KTextureTypeArrtibute    = "texture-type";
static const QString KMinFilterArrtibute      = "min-filter";
static const QString KMagFilterArrtibute      = "mag-filter";
static const QString KMipMapFilterArrtibute   = "mipmap-filter";
static const QString KAddressModeSArrtibute   = "address-mode-s";
static const QString KAddressModeTArrtibute   = "address-mode-t";

const QString ResourceItemTexture::KPropertyNameMinifyingFiltering  = ResourceItemTexture::tr("Minifying");
const QString ResourceItemTexture::KPropertyNameMagnifyingFiltering = ResourceItemTexture::tr("Magnifying");
const QString ResourceItemTexture::KPropertyNameMipMappingFiltering = ResourceItemTexture::tr("Mip Mapping");
const QString ResourceItemTexture::KPropertyNameAddressingModeS     = ResourceItemTexture::tr("Coordinate S");
const QString ResourceItemTexture::KPropertyNameAddressingModeT     = ResourceItemTexture::tr("Coordinate T");
const QString ResourceItemTexture::KPropertyNameLocation            = ResourceItemTexture::tr("Location");
const QString ResourceItemTexture::KPropertyNameWidth               = ResourceItemTexture::tr("Width");
const QString ResourceItemTexture::KPropertyNameHeight              = ResourceItemTexture::tr("Height");
const QString ResourceItemTexture::KPropertyNameImageFormat         = ResourceItemTexture::tr("Format");
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct TextureTypeMap
{
  const QString name;
  const TextureType type;
};

static const TextureTypeMap l_textureTypeMappings[] = { { "none", EInvalidTexture },
                                                        { "2D", ETexture2D }
};

struct TextureFilterTypeMap
{
  const QString name;
  const QString displayName;
  const TextureFilterType type;
};

static const TextureFilterTypeMap l_textureFilterTypeMappings[] = { { "nearest", ResourceItemTexture::tr("Nearest"), ETextureFilterNearest },
                                                                    { "bilinear", ResourceItemTexture::tr("Bilinear"), ETextureFilterBilinear }
};

struct TextureMipMappingFilterTypeMap
{
  const QString name;
  const QString displayName;
  const TextureMipMappingFilterType type;
};

static const TextureMipMappingFilterTypeMap l_textureMipMappingFilterTypeMappings[] = {
  { "none", ResourceItemTexture::tr("None"), EMipMappingFilterNone },
  { "nearest", ResourceItemTexture::tr("Nearest"), EMipMappingFilterNearest },
  { "trilinear", ResourceItemTexture::tr("Trilinear"), EMipMappingFilterTrilinear }
};

struct TextureAddressingModeTypeMap
{
  const QString name;
  const QString displayName;
  const TextureAddressModeType type;
};

static const TextureAddressingModeTypeMap l_textureAddressingModeTypeMappings[] = { { "clamp", ResourceItemTexture::tr("Clamp"), EAddressModeClamp},
                                                                                    { "repeat", ResourceItemTexture::tr("Repeat"), EAddressModeRepeat }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function returning index of minifcation filter of a given type. */
static int GetMinificationFilterMapIndex(TextureFilterType type)
{
  int index = -1;

  for (unsigned int i = 0; i < sizeof (l_textureFilterTypeMappings) / sizeof (l_textureFilterTypeMappings[0]); ++i)
  {
    const TextureFilterTypeMap& currentFilter = l_textureFilterTypeMappings[i];

    if (currentFilter.type == type)
    {
      index = i;
      break;
    }
  }

  return index;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function returning index of magnifcation filter of a given type. */
static int GetMagnificationFilterMapIndex(TextureFilterType type)
{
  // NOTE: for the time being it is the same as for minification
  return GetMinificationFilterMapIndex(type);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function returning index of mip mapping filter of a given type. */
static int GetMipMappingFilterMapIndex(TextureMipMappingFilterType type)
{
  int index = -1;

  for (unsigned int i = 0; i < sizeof (l_textureMipMappingFilterTypeMappings) / sizeof (l_textureMipMappingFilterTypeMappings[0]); ++i)
  {
    const TextureMipMappingFilterTypeMap& currentFilter = l_textureMipMappingFilterTypeMappings[i];

    if (currentFilter.type == type)
    {
      index = i;
      break;
    }
  }

  return index;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function returning index of addressing mode of a given type. */
static int GetAddressingModeMapIndex(TextureAddressModeType type)
{
  int index = -1;

  for (unsigned int i = 0; i < sizeof (l_textureAddressingModeTypeMappings) / sizeof (l_textureAddressingModeTypeMappings[0]); ++i)
  {
    const TextureAddressingModeTypeMap& currentFilter = l_textureAddressingModeTypeMappings[i];

    if (currentFilter.type == type)
    {
      index = i;
      break;
    }
  }

  return index;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting texture filter type into string value. */
static QString GetTextureFilterAsText(TextureFilterType type)
{
  QString value;

  for (unsigned int i = 0; i < sizeof (l_textureFilterTypeMappings) / sizeof (l_textureFilterTypeMappings[0]); ++i)
  {
    const TextureFilterTypeMap& currentFilter = l_textureFilterTypeMappings[i];

    if (currentFilter.type == type)
    {
      value = currentFilter.name;
      break;
    }
  }

  Q_ASSERT( ! value.isEmpty());

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting string into texture filter type value. */
static TextureFilterType GetTextureFilterFromText(QString name)
{
  TextureFilterType type = ETextureFilterNearest;

  for (unsigned int i = 0; i < sizeof (l_textureFilterTypeMappings) / sizeof (l_textureFilterTypeMappings[0]); ++i)
  {
    const TextureFilterTypeMap& currentFilter = l_textureFilterTypeMappings[i];

    if (currentFilter.name == name)
    {
      type = currentFilter.type;
      break;
    }
  }

  return type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting texture mip mapping filter type into string value. */
static QString GetTextureMipMappingFilterAsText(TextureMipMappingFilterType type)
{
  QString value;

  for (unsigned int i = 0; i < sizeof (l_textureMipMappingFilterTypeMappings) / sizeof (l_textureMipMappingFilterTypeMappings[0]); ++i)
  {
    const TextureMipMappingFilterTypeMap& currentFilter = l_textureMipMappingFilterTypeMappings[i];

    if (currentFilter.type == type)
    {
      value = currentFilter.name;
      break;
    }
  }

  Q_ASSERT( ! value.isEmpty());

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting string to texture mip mapping filter type. */
static TextureMipMappingFilterType GetTextureMipMappingFilterFromText(QString name)
{
  TextureMipMappingFilterType value = EMipMappingFilterNone;

  for (unsigned int i = 0; i < sizeof (l_textureMipMappingFilterTypeMappings) / sizeof (l_textureMipMappingFilterTypeMappings[0]); ++i)
  {
    const TextureMipMappingFilterTypeMap& currentFilter = l_textureMipMappingFilterTypeMappings[i];

    if (currentFilter.name == name)
    {
      value = currentFilter.type;
      break;
    }
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting texture addressing mode type into string value. */
static QString GetAddressingModeAsText(TextureAddressModeType type)
{
  QString value;

  for (unsigned int i = 0; i < sizeof (l_textureAddressingModeTypeMappings) / sizeof (l_textureAddressingModeTypeMappings[0]); ++i)
  {
    const TextureAddressingModeTypeMap& currentMode = l_textureAddressingModeTypeMappings[i];

    if (currentMode.type == type)
    {
      value = currentMode.name;
      break;
    }
  }

  Q_ASSERT( ! value.isEmpty());

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting string to texture addressing mode type value. */
static TextureAddressModeType GetAddressingModeFromText(QString name)
{
  TextureAddressModeType value = EAddressModeClamp;

  for (unsigned int i = 0; i < sizeof (l_textureAddressingModeTypeMappings) / sizeof (l_textureAddressingModeTypeMappings[0]); ++i)
  {
    const TextureAddressingModeTypeMap& currentMode = l_textureAddressingModeTypeMappings[i];

    if (currentMode.name == name)
    {
      value = currentMode.type;
      break;
    }
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting texture type into string value. */
static QString GetTextureTypeAsText(TextureType type)
{
  QString value;

  // determine type name
  for (int i = 0; i < sizeof (l_textureTypeMappings) / sizeof (l_textureTypeMappings[0]); ++i)
  {
    const TextureTypeMap& mapping = l_textureTypeMappings[i];

    // check if type found in map table
    if (mapping.type == type)
    {
      // found
      value = mapping.name;
      break;
    }
  }

  Q_ASSERT( ! value.isEmpty());

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting string to texture type value. */
TextureType GetTextureTypeFromText(QString name)
{
  TextureType value = EInvalidTexture;

  // determine type
  for (int i = 0; i < sizeof (l_textureTypeMappings) / sizeof (l_textureTypeMappings[0]); ++i)
  {
    const TextureTypeMap& mapping = l_textureTypeMappings[i];

    // check if name found in map table
    if (mapping.name == name)
    {
      // found
      value = mapping.type;
      break;
    }
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemTexture::ResourceItemTexture(const QString& name, const QString& configurationName, ResourceItem* parent)
  : ResourceItem(name, configurationName, parent),
    m_type(EInvalidTexture),
    m_size(QSize(0, 0)),
    m_imageFormat(QImage::Format_Invalid),
    m_magnificationFilter(ETextureFilterNearest),
    m_minificationFilter(ETextureFilterNearest),
    m_mipMappingFilter(EMipMappingFilterNone),
    m_addressingModeS(EAddressModeRepeat),
    m_addressingModeT(EAddressModeRepeat)
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
  if (m_thumbnail.isNull() && ! m_fullPath.isEmpty())
  {
    // generate
    QImageReader imageReader(m_fullPath);

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
    stream.writeAttribute(KTextureTypeArrtibute, GetTextureTypeAsText(type()));
    stream.writeAttribute(KPathArrtibute, fullPath());
    stream.writeAttribute(KMinFilterArrtibute, GetTextureFilterAsText(minificationFilter()));
    stream.writeAttribute(KMagFilterArrtibute, GetTextureFilterAsText(magnificationFilter()));
    stream.writeAttribute(KMipMapFilterArrtibute, GetTextureMipMappingFilterAsText(mipMappingFilter()));
    stream.writeAttribute(KAddressModeSArrtibute, GetAddressingModeAsText(addressModeS()));
    stream.writeAttribute(KAddressModeTArrtibute, GetAddressingModeAsText(addressModeT()));

    // end serialization
    result = endSerialize(stream);
  }

  return result && ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemTexture::unserialize(QXmlStreamReader& stream)
{
  // retrieve data
  // NOTE: assigning directly to disallow signaling
  m_fullPath            = stream.attributes().value(KPathArrtibute).toString();
  m_type                = GetTextureTypeFromText(stream.attributes().value(KTextureTypeArrtibute).toString());
  m_minificationFilter  = GetTextureFilterFromText(stream.attributes().value(KMinFilterArrtibute).toString());
  m_magnificationFilter = GetTextureFilterFromText(stream.attributes().value(KMagFilterArrtibute).toString());
  m_mipMappingFilter    = GetTextureMipMappingFilterFromText(stream.attributes().value(KMipMapFilterArrtibute).toString());
  m_addressingModeS     = GetAddressingModeFromText(stream.attributes().value(KAddressModeSArrtibute).toString());
  m_addressingModeT     = GetAddressingModeFromText(stream.attributes().value(KAddressModeTArrtibute).toString());

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::setFullPath(const QString& path)
{
  if (m_fullPath != path)
  {
    m_fullPath = path;

    // emit
    emit changed(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QString& ResourceItemTexture::fullPath() const
{
  return m_fullPath;
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

      variant = fullPath();
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

      setFullPath(value.toString());
      return true;
  }

  // call base class
  return ResourceItem::setData(value, role);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QList<PropertyDefinition> ResourceItemTexture::propertiesDefinition() const
{
  // call base class first
  QList<PropertyDefinition> list = ResourceItem::propertiesDefinition();

  PropertyValueContainer values;

  // create info group
  PropertyDefinition infoGroup(tr("Info"), NPropertyObject::EGroup, values);

  // add name, location, width, height and bpp
  values = PropertyValueHelper::CreateFilePathValue(fullPath(), tr("Images") + QLatin1String(" (*.png *.jpg)"), true);
  infoGroup.addChildProperty(PropertyDefinition(KPropertyNameLocation, NPropertyObject::EFilePath, values, 0));
  values = PropertyValueHelper::CreateIntegerValue(size().width(), KMinimalSize, KMaximalSize);
  infoGroup.addChildProperty(PropertyDefinition(KPropertyNameWidth, NPropertyObject::EInt, values, 0, true));
  values = PropertyValueHelper::CreateIntegerValue(size().height(), KMinimalSize, KMaximalSize);
  infoGroup.addChildProperty(PropertyDefinition(KPropertyNameHeight, NPropertyObject::EInt, values, 0, true));
  values.clear();
  values << imageFormatAsText();
  infoGroup.addChildProperty(PropertyDefinition(KPropertyNameImageFormat, NPropertyObject::EString, values, 0, true));

  // create filtering group
  values.clear();
  PropertyDefinition filteringGroup(tr("Filtering"), NPropertyObject::EGroup, values);

  addMinificationFilterDefinitions(filteringGroup);
  addMagnificationFilterDefinitions(filteringGroup);

  // create addressing group
  values.clear();
  PropertyDefinition addressingGroup(tr("Addressing"), NPropertyObject::EGroup, values);

  addAddressingModeDefinitions(addressingGroup);

  // add groups to list
  list.push_back(infoGroup);
  list.push_back(filteringGroup);
  list.push_back(addressingGroup);

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::addMagnificationFilterDefinitions(NPropertyObject::PropertyDefinition& group) const
{
  PropertyValueContainer values;

  // create magnification filter values
  for (unsigned int i = 0; i < sizeof (l_textureFilterTypeMappings) / sizeof (l_textureFilterTypeMappings[0]); ++i)
  {
    const TextureFilterTypeMap& currentFilter = l_textureFilterTypeMappings[i];

    values << currentFilter.displayName;
    values << QIcon();
  }

  // add all into main group
  group.addChildProperty(PropertyDefinition(KPropertyNameMagnifyingFiltering, NPropertyObject::EEnum, values,
                                            GetMagnificationFilterMapIndex(m_magnificationFilter)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::addAddressingModeDefinitions(NPropertyObject::PropertyDefinition& group) const
{
  PropertyValueContainer values;

  // create magnification filter values
  for (unsigned int i = 0; i < sizeof (l_textureAddressingModeTypeMappings) / sizeof (l_textureAddressingModeTypeMappings[0]); ++i)
  {
    const TextureAddressingModeTypeMap& currentMode = l_textureAddressingModeTypeMappings[i];

    values << currentMode.displayName;
    values << QIcon();
  }

  // add all into main group
  group.addChildProperty(PropertyDefinition(KPropertyNameAddressingModeS, NPropertyObject::EEnum, values, GetAddressingModeMapIndex(m_addressingModeS)));
  group.addChildProperty(PropertyDefinition(KPropertyNameAddressingModeT, NPropertyObject::EEnum, values, GetAddressingModeMapIndex(m_addressingModeT)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::addMinificationFilterDefinitions(NPropertyObject::PropertyDefinition& group) const
{
  PropertyValueContainer values;

  // create minification filter values
  for (unsigned int i = 0; i < sizeof (l_textureFilterTypeMappings) / sizeof (l_textureFilterTypeMappings[0]); ++i)
  {
    const TextureFilterTypeMap& currentFilter = l_textureFilterTypeMappings[i];

    values << currentFilter.displayName;
    values << QIcon();
  }

  // create minification filter group
  // NOTE: mip mapping filters will be added as sub group of this one
  PropertyDefinition minificationFilterGroup(KPropertyNameMinifyingFiltering, NPropertyObject::EEnum, values,
                                             GetMinificationFilterMapIndex(m_minificationFilter));

  // create mipmapping filters values
  values.clear();
  for (unsigned int i = 0; i < sizeof (l_textureMipMappingFilterTypeMappings) / sizeof (l_textureMipMappingFilterTypeMappings[0]); ++i)
  {
    const TextureMipMappingFilterTypeMap& currentFilter = l_textureMipMappingFilterTypeMappings[i];

    values << currentFilter.displayName;
    values << QIcon();
  }

  // add mip mapping filter values as sub property of minification filter group
  minificationFilterGroup.addChildProperty(PropertyDefinition(KPropertyNameMipMappingFiltering, NPropertyObject::EEnum, values,
                                                              GetMipMappingFilterMapIndex(m_mipMappingFilter)));

  // add all into main group
  group.addChildProperty(minificationFilterGroup);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::update(const QString& name, const QVariant& value)
{
  if (KPropertyNameMinifyingFiltering == name)
  {
    Q_ASSERT(value.canConvert<int>());
    setMinificationFilter(l_textureFilterTypeMappings[value.toInt()].type);
  }
  else if (KPropertyNameMagnifyingFiltering == name)
  {
    Q_ASSERT(value.canConvert<int>());
    setMagnificationFilter(l_textureFilterTypeMappings[value.toInt()].type);
  }
  else if (KPropertyNameMipMappingFiltering == name)
  {
    Q_ASSERT(value.canConvert<int>());
    setMipMappingFilter(l_textureMipMappingFilterTypeMappings[value.toInt()].type);
  }
  else if (KPropertyNameAddressingModeS == name)
  {
    Q_ASSERT(value.canConvert<int>());
    setAddressModeS(l_textureAddressingModeTypeMappings[value.toInt()].type);
  }
  else if (KPropertyNameAddressingModeT == name)
  {
    Q_ASSERT(value.canConvert<int>());
    setAddressModeT(l_textureAddressingModeTypeMappings[value.toInt()].type);
  }
  else if (KPropertyNameLocation == name)
  {
    Q_ASSERT(value.canConvert<QString>());
    setFullPath(value.toString());
  }
  else
  {
    // call base class
    ResourceItem::update(name, value);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QSize ResourceItemTexture::size() const
{
  // check if size can be determined
  if (m_size.isNull() && ! m_fullPath.isEmpty())
  {
    QImageReader imageReader(m_fullPath);

    m_size = imageReader.size();
  }

  return m_size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QImage::Format ResourceItemTexture::imageFormat() const
{
  // check if format can be determined
  if ((QImage::Format_Invalid == m_imageFormat) && ! m_fullPath.isEmpty())
  {
    QImageReader imageReader(m_fullPath);

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
    case QImage::Format_Invalid:                text = tr("Invalid"); break;
    case QImage::Format_Indexed8:               text = tr("Indexed 8-bit"); break;
    case QImage::Format_RGB32:                  text = tr("32-bit RGB"); break;
    case QImage::Format_ARGB32:                 text = tr("32-bit ARGB"); break;
    case QImage::Format_ARGB32_Premultiplied:   text = tr("Premultiplied 32-bit ARGB"); break;
    case QImage::Format_RGB16:                  text = tr("16-bit RGB (565)"); break;
    case QImage::Format_ARGB8565_Premultiplied: text = tr("Premultiplied 24-bit ARGB (8565)"); break;
    case QImage::Format_RGB555:                 text = tr("15-bit RGB (555)"); break;

    default:

      text = tr("Unspecified");
      break;
  }

  return text;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::setMinificationFilter(TextureFilterType type)
{
  if (type != m_minificationFilter)
  {
    m_minificationFilter = type;

    // notify
    emit changed(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureFilterType ResourceItemTexture::minificationFilter() const
{
  return m_minificationFilter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::setMagnificationFilter(TextureFilterType type)
{
  if (type != m_magnificationFilter)
  {
    m_magnificationFilter = type;

    // notify
    emit changed(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureFilterType ResourceItemTexture::magnificationFilter() const
{
  return m_magnificationFilter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::setMipMappingFilter(TextureMipMappingFilterType type)
{
  if (type != m_mipMappingFilter)
  {
    m_mipMappingFilter = type;

    // notify
    emit changed(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureMipMappingFilterType ResourceItemTexture::mipMappingFilter() const
{
  return m_mipMappingFilter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::setAddressModeS(TextureAddressModeType type)
{
  if (type != m_addressingModeS)
  {
    m_addressingModeS = type;

    // notify
    emit changed(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAddressModeType ResourceItemTexture::addressModeS() const
{
  return m_addressingModeS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::setAddressModeT(TextureAddressModeType type)
{
  if (type != m_addressingModeT)
  {
    m_addressingModeT = type;

    // notify
    emit changed(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAddressModeType ResourceItemTexture::addressModeT() const
{
  return m_addressingModeT;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureType ResourceItemTexture::type() const
{
  return m_type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::onInvalidate()
{
  m_size        = QSize(0, 0);
  m_imageFormat = QImage::Format_Invalid;
  m_thumbnail   = QImage();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
