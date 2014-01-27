#include "ResourceItemTextureAtlas.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceItemGroup.h"
#include "ResourceLibraryWindowResourceInserter.h"
#include <ObjectPool.h>
#include <QDebug>
#include <QMenu>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
using NPropertyObject::PropertyDefinition;
using NPropertyObject::PropertyValueContainer;
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

  // update properties of base class
  for (int i = 0; i < list.size(); ++i)
  {
    PropertyDefinition& definition = list[i];

    if (definition.name() == ResourceItemTexture::tr("Info"))
    {
      PropertyDefinition* property = definition.findChildProperty(ResourceItemTexture::tr("Width"));
      Q_ASSERT(NULL != property);
      property->setReadOnlyEnabled(false);

      property = definition.findChildProperty(ResourceItemTexture::tr("Height"));
      Q_ASSERT(NULL != property);
      property->setReadOnlyEnabled(false);
    }
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
