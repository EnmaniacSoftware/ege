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
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemTexture::ResourceItemTexture() : ResourceItem()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemTexture::ResourceItemTexture(const QString& name, ResourceItem* parent) : ResourceItem(name, parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemTexture::~ResourceItemTexture()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceItemTexture::Create(const QString& name, ResourceItem* parent)
{
  return new ResourceItemTexture(name, parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemTexture::TypeName()
{
  return "image";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemTexture::ResourceLibraryWindowHook(QMenu& menu, const QString& selectedType)
{
  // only allow for group resources
  if (selectedType == ResourceItemGroup::TypeName())
  {
    ResourceLibraryWindowResourceInserter* helper = ObjectPool::Instance()->getObject<ResourceLibraryWindowResourceInserter>();
    Q_ASSERT(NULL != helper);

    menu.addAction(tr("Add 2D texture"), helper, SLOT(onAddTexture2D()));
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
QString ResourceItemTexture::type() const
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
    // store path
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
