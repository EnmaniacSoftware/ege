#include "ResourceLibrary.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceItem.h"
#include <QList>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const QString KResourceLibraryTag = "ResourceLibrary";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibrary::ResourceLibrary(QObject* parent) : QObject(parent),
                                                    m_model(new ResourceLibraryDataModel(this))
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibrary::~ResourceLibrary()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibrary::onSaveData(QXmlStreamWriter& stream)
{
  // store data
  stream.writeStartElement(KResourceLibraryTag);

  // serialize model
  model()->serialize(stream);

  // finalize
  stream.writeEndElement();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibrary::onLoadData(QXmlStreamReader& stream)
{
  // check if project element
  if (KResourceLibraryTag == stream.name())
  {
    // unserialize model
    model()->unserialize(stream);

    // emit
    emit loaded(model()->rowCount());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModel* ResourceLibrary::model() const
{
  return m_model;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
