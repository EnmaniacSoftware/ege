#include "ResourceItemContainer.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceItemFactory.h"
#include "MainWindow.h"
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemContainer::ResourceItemContainer() : ResourceItem()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemContainer::~ResourceItemContainer()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource item. This method is a registration method for factory. */
ResourceItem* ResourceItemContainer::Create()
{
  return new ResourceItemContainer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Returns the item flags for the given item. */
Qt::ItemFlags ResourceItemContainer::flags() const
{
  return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Returns type name. */
QString ResourceItemContainer::type() const
{
  return "container";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Serializes into given stream. */
bool ResourceItemContainer::serialize(QXmlStreamWriter& stream) const
{
  stream.writeStartElement("resource-item");
  
  stream.writeAttribute("type", type());
  stream.writeAttribute("name", name());

  // serialize children
  foreach (const ResourceItem* item, m_children)
  {
    if (!item->serialize(stream))
    {
      // error!
      return false;
    }
  }

  stream.writeEndElement();
  return !stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Unserializes from given data stream. */
bool ResourceItemContainer::unserialize(QXmlStreamReader& stream)
{
  // retrieve data
  m_name = stream.attributes().value("name").toString();

  ResourceItem* item = NULL;

  // process children
  while (!stream.atEnd())
  {
    QXmlStreamReader::TokenType token = stream.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        // check if resource item element
        if ("resource-item" == stream.name())
        {
          // check if processing an item already
          if (NULL != item)
          {
            // let item unserialize
            if (!item->unserialize(stream))
            {
              // error!
              return false;
            }
          }
          else
          {
            // create proper item
            item = app->resourceItemFactory()->createItem(stream.attributes().value("type").toString(), 
                                                          stream.attributes().value("name").toString(), this);
            if (NULL == item)
            {
              // error!
  //            QMessageBox::warning(this, tr("Open Project error"), tr("Could not open project file!"), QMessageBox::Ok);
              return false;
            }

            // add to pool
            m_children.push_back(item);
          }
        }
        else
        {
          qWarning() << "Skipping data: " << stream.name();
        }
        break;

      case QXmlStreamReader::EndElement:

        // done
        return !stream.hasError();
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Returns size hint. */
QSize ResourceItemContainer::sizeHint() const
{
  return QSize(200, 20);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
