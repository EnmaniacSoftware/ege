#include "ResourceItemImage.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceItemGroup.h"
#include <FileSystemUtils.h>
#include <QImageReader>
#include <QDebug>
#include <QMenu>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const QString KPathArrtibute = "path";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemImage::ResourceItemImage() : ResourceItem()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemImage::ResourceItemImage(const QString& name, ResourceItem* parent) : ResourceItem(name, parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemImage::~ResourceItemImage()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceItemImage::Create(const QString& name, ResourceItem* parent)
{
  return new ResourceItemImage(name, parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemImage::TypeName()
{
  return "image";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemImage::ResourceLibraryWindowHook(QMenu& menu, const QString& selectedType)
{
  // only allow for group resources
  if (selectedType == ResourceItemGroup::TypeName())
  {
//    ResourceLibraryWindowGroupAdder* adder = ObjectPool::Instance()->getObject<ResourceLibraryWindowGroupAdder>();
//    Q_ASSERT(NULL != adder);

//    menu.addAction(tr("Add group"), adder, SLOT(onAdd()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QImage& ResourceItemImage::thumbnailImage() const
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
QString ResourceItemImage::type() const
{
  return ResourceItemImage::TypeName();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QSize ResourceItemImage::sizeHint() const
{
  return QSize(200, 40);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemImage::serialize(QXmlStreamWriter& stream) const
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
bool ResourceItemImage::unserialize(QXmlStreamReader& stream)
{
  // retrieve data
  m_path = stream.attributes().value(KPathArrtibute).toString();

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemImage::setPath(const QString& path)
{
  m_path = path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QString& ResourceItemImage::path() const
{
  return m_path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QVariant ResourceItemImage::data(int columnIndex, int role) const
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
bool ResourceItemImage::setData(const QVariant &value, int role)
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
