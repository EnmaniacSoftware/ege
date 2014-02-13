#include "ResourceItemTexture.h"
#include "ResourceItemTextureAtlas.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceItemGroup.h"
#include "ResourceLibraryWindowResourceInserter.h"
#include "ImageFormats.h"
#include "ResourceLibrary.h"
#include <Attachable.h>
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
static const QString KPathAttribute           = "path";
static const QString KTextureTypeAttribute    = "texture-type";
static const QString KMinFilterAttribute      = "min-filter";
static const QString KMagFilterAttribute      = "mag-filter";
static const QString KMipMapFilterAttribute   = "mipmap-filter";
static const QString KAddressModeSAttribute   = "address-mode-s";
static const QString KAddressModeTAttribute   = "address-mode-t";
static const QString KTextureAtlasAttribute   = "atlas";

const QString ResourceItemTexture::KPropertyNameMinifyingFiltering  = ResourceItemTexture::tr("Minifying");
const QString ResourceItemTexture::KPropertyNameMagnifyingFiltering = ResourceItemTexture::tr("Magnifying");
const QString ResourceItemTexture::KPropertyNameMipMappingFiltering = ResourceItemTexture::tr("Mip Mapping");
const QString ResourceItemTexture::KPropertyNameAddressingModeS     = ResourceItemTexture::tr("Coordinate S");
const QString ResourceItemTexture::KPropertyNameAddressingModeT     = ResourceItemTexture::tr("Coordinate T");
const QString ResourceItemTexture::KPropertyNameLocation            = ResourceItemTexture::tr("Location");
const QString ResourceItemTexture::KPropertyNameWidth               = ResourceItemTexture::tr("Width");
const QString ResourceItemTexture::KPropertyNameHeight              = ResourceItemTexture::tr("Height");
const QString ResourceItemTexture::KPropertyNameImageFormat         = ResourceItemTexture::tr("Format");
const QString ResourceItemTexture::KPropertyNameTextureAtlas        = ResourceItemTexture::tr("Texture Atlas");

const QString ResourceItemTexture::KGroupNameInfo                   = ResourceItemTexture::tr("Info");
const QString ResourceItemTexture::KGroupNameFiltering              = ResourceItemTexture::tr("Filtering");
const QString ResourceItemTexture::KGroupNameAddressing             = ResourceItemTexture::tr("Addressing");
const QString ResourceItemTexture::KGroupNameAtlasing               = ResourceItemTexture::tr("Atlasing");
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
static QString GetAddressingModeAsText(TextureAddressMode mode)
{
  QString value;

  TextureAddressingModeInfoList list = SupportedTextureAddressingModes();
  for (int i = 0; i < list.size(); ++i)
  {
    const TextureAddressingModeInfo& info = list.at(i);

    if (info.mode == mode)
    {
      value = info.name;
      break;
    }
  }

  Q_ASSERT( ! value.isEmpty());

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function converting string to texture addressing mode type value. */
static TextureAddressMode GetAddressingModeFromText(QString name)
{
  TextureAddressMode mode = EAddressModeClamp;

  TextureAddressingModeInfoList list = SupportedTextureAddressingModes();
  for (int i = 0; i < list.size(); ++i)
  {
    const TextureAddressingModeInfo& info = list.at(i);

    if (info.name == name)
    {
      mode = info.mode;
      break;
    }
  }

  return mode;
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
ResourceItemTexture::ResourceItemTexture(const QString& name, const QString& configurationName, const QUuid& id, ResourceItem* parent)
  : ResourceItem(name, configurationName, id, parent),
    m_type(EInvalidTexture),
    m_size(QSize(0, 0)),
    m_imageFormat(QImage::Format_Invalid),
    m_magnificationFilter(ETextureFilterNearest),
    m_minificationFilter(ETextureFilterNearest),
    m_mipMappingFilter(EMipMappingFilterNone),
    m_addressingModeS(EAddressModeRepeat),
    m_addressingModeT(EAddressModeRepeat)
{
  ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
  Q_ASSERT(NULL != library);

  connect(this, SIGNAL(changed(const ResourceItem*)), this, SLOT(onInvalidate()));
  connect(library, SIGNAL(itemRemoved(ResourceItem*)), this, SLOT(onResourceLibraryModelItemRemoved(ResourceItem*)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemTexture::~ResourceItemTexture()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceItemTexture::Create(const QString& name, const QString& configurationName, const QUuid& id, ResourceItem* parent)
{
  return new ResourceItemTexture(name, configurationName, id, parent);
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
    doSerialize(stream);

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
  m_fullPath            = stream.attributes().value(KPathAttribute).toString();
  m_type                = GetTextureTypeFromText(stream.attributes().value(KTextureTypeAttribute).toString());
  m_minificationFilter  = GetTextureFilterFromText(stream.attributes().value(KMinFilterAttribute).toString());
  m_magnificationFilter = GetTextureFilterFromText(stream.attributes().value(KMagFilterAttribute).toString());
  m_mipMappingFilter    = GetTextureMipMappingFilterFromText(stream.attributes().value(KMipMapFilterAttribute).toString());
  m_addressingModeS     = GetAddressingModeFromText(stream.attributes().value(KAddressModeSAttribute).toString());
  m_addressingModeT     = GetAddressingModeFromText(stream.attributes().value(KAddressModeTAttribute).toString());
  m_textureAtlasId      = QUuid(stream.attributes().value(KTextureAtlasAttribute).toString());

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
  PropertyDefinition infoGroup(KGroupNameInfo, NPropertyObject::EGroup, values);

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
  PropertyDefinition filteringGroup(KGroupNameFiltering, NPropertyObject::EGroup, values);

  addMinificationFilterDefinitions(filteringGroup);
  addMagnificationFilterDefinitions(filteringGroup);

  // create addressing group
  values.clear();
  PropertyDefinition addressingGroup(KGroupNameAddressing, NPropertyObject::EGroup, values);

  addAddressingModeDefinitions(addressingGroup);

  // create atlasing group
  values.clear();
  PropertyDefinition atlasingGroup(KGroupNameAtlasing, NPropertyObject::EGroup, values);

  addAtlasingDefinitions(atlasingGroup);

  // add groups to list
  list.push_back(infoGroup);
  list.push_back(filteringGroup);
  list.push_back(addressingGroup);
  list.push_back(atlasingGroup);

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::addMagnificationFilterDefinitions(NPropertyObject::PropertyDefinition& group) const
{
  PropertyValueContainer values;

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

  int defaultValueIndexS = 0;
  int defaultValueIndexT = 0;

  TextureAddressingModeInfoList list = SupportedTextureAddressingModes();
  for (int i = 0; i < list.size(); ++i)
  {
    const TextureAddressingModeInfo& info = list.at(i);

    // add to values
    values << info.displayName;
    values << QIcon();

    // check if current item is default one
    if (info.mode == addressModeS())
    {
      defaultValueIndexS = i;
    }

    if (info.mode == addressModeT())
    {
      defaultValueIndexT = i;
    }
  }

  // add all into main group
  group.addChildProperty(PropertyDefinition(KPropertyNameAddressingModeS, NPropertyObject::EEnum, values, defaultValueIndexS));
  group.addChildProperty(PropertyDefinition(KPropertyNameAddressingModeT, NPropertyObject::EEnum, values, defaultValueIndexT));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::addAtlasingDefinitions(NPropertyObject::PropertyDefinition& group) const
{
  ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
  Q_ASSERT(NULL != library);

  // get all available texture atlas resources which belong to this item group
  QList<ResourceItem*> atlases = library->items(ResourceItemTextureAtlas::TypeName(), parent());

  PropertyValueContainer values;

  // add 'no atlas' entry
  values << tr("None");
  values << QIcon();

  int defaultValueIndex = 0;

  for (int i = 0; i < atlases.size(); ++i)
  {
    const ResourceItem* item = atlases.at(i);

    // add to values
    values << item->name();
    values << QIcon();

    // check if current item is default one
    if (textureAtlasId() == item->id())
    {
      defaultValueIndex = i + 1;
    }
  }

  // add all into main group
  group.addChildProperty(PropertyDefinition(KPropertyNameTextureAtlas, NPropertyObject::EEnum, values, defaultValueIndex));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::addMinificationFilterDefinitions(NPropertyObject::PropertyDefinition& group) const
{
  PropertyValueContainer values;

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

    TextureAddressingModeInfoList list = SupportedTextureAddressingModes();
    setAddressModeS(list.at(value.toInt()).mode);
  }
  else if (KPropertyNameAddressingModeT == name)
  {
    Q_ASSERT(value.canConvert<int>());

    TextureAddressingModeInfoList list = SupportedTextureAddressingModes();
    setAddressModeT(list.at(value.toInt()).mode);
  }
  else if (KPropertyNameLocation == name)
  {
    Q_ASSERT(value.canConvert<QString>());
    setFullPath(value.toString());
  }
  else if (KPropertyNameTextureAtlas == name)
  {
    Q_ASSERT(value.canConvert<int>());

    ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
    Q_ASSERT(NULL != library);

    // get all available texture atlas resources which belong to this item group
    QList<ResourceItem*> atlases = library->items(ResourceItemTextureAtlas::TypeName(), parent());

    const int index = value.toInt();
    if (0 == index)
    {
      setTextureAtlasId(QUuid());
    }
    else
    {
      setTextureAtlasId(atlases.at(index - 1)->id());
    }
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
void ResourceItemTexture::doSerialize(QXmlStreamWriter& stream) const
{
  // store data
  stream.writeAttribute(KTextureTypeAttribute, GetTextureTypeAsText(type()));
  stream.writeAttribute(KPathAttribute, fullPath());
  stream.writeAttribute(KMinFilterAttribute, GetTextureFilterAsText(minificationFilter()));
  stream.writeAttribute(KMagFilterAttribute, GetTextureFilterAsText(magnificationFilter()));
  stream.writeAttribute(KMipMapFilterAttribute, GetTextureMipMappingFilterAsText(mipMappingFilter()));
  stream.writeAttribute(KAddressModeSAttribute, GetAddressingModeAsText(addressModeS()));
  stream.writeAttribute(KAddressModeTAttribute, GetAddressingModeAsText(addressModeT()));
  stream.writeAttribute(KTextureAtlasAttribute, textureAtlasId().toString());
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
  QString text = tr("Invalid");

  const ImageFormatInfoList& supportedImageFormats = SupportedImageFormats();
  for (int i = 0; i < supportedImageFormats.size(); ++i)
  {
    const ImageFormatInfo& info = supportedImageFormats.at(i);

    if (info.format == imageFormat())
    {
      text = info.displayName;
      break;
    }
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
void ResourceItemTexture::setAddressModeS(TextureAddressMode type)
{
  if (type != m_addressingModeS)
  {
    m_addressingModeS = type;

    // notify
    emit changed(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAddressMode ResourceItemTexture::addressModeS() const
{
  return m_addressingModeS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::setAddressModeT(TextureAddressMode type)
{
  if (type != m_addressingModeT)
  {
    m_addressingModeT = type;

    // notify
    emit changed(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAddressMode ResourceItemTexture::addressModeT() const
{
  return m_addressingModeT;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QUuid& ResourceItemTexture::textureAtlasId() const
{
  return m_textureAtlasId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::setTextureAtlasId(const QUuid& atlasId)
{
  if (atlasId != m_textureAtlasId)
  {
    ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
    Q_ASSERT(NULL != library);

    // check if any previous atlas
    if ( ! m_textureAtlasId.isNull())
    {
      // detach self from previous atlas
      Attachable* atlas = qobject_cast<Attachable*>(library->item(m_textureAtlasId));
      Q_ASSERT(NULL != atlas);

      atlas->detachObject(id());
    }

    // set new atlas texture id
    m_textureAtlasId = atlasId;

    // check if any new atlas
    if ( ! atlasId.isNull())
    {
      // attach self to new atlas
      Attachable* atlas = qobject_cast<Attachable*>(library->item(atlasId));
      Q_ASSERT(NULL != atlas);

      atlas->attachObject(id());
    }

    // notify
    emit changed(this);
  }
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
void ResourceItemTexture::onResourceLibraryModelItemRemoved(ResourceItem* item)
{
  // check if currently set atlas texture
  if (item->id() == textureAtlasId())
  {
    // reset
    setTextureAtlasId(QUuid());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
