#ifndef ATLAS_NODE
#define ATLAS_NODE

#include <EGERect.h>
#include <EGEImage.h>
#include "AtlasGroupEntry.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AtlasNode
{
  public:

    AtlasNode()
    {
      m_child[0] = NULL;
      m_child[1] = NULL;

      m_rect = EGE::Recti::INVALID;

      m_entry = NULL;
    }

   ~AtlasNode()
    {
      EGE_DELETE(m_child[0]);
      EGE_DELETE(m_child[1]);
    }

    /*! Inserts given image into atlas. Returns node, image was inserted into. NULL if no suitable node found. */
    AtlasNode* insert(const AtlasGroupEntry* entry)
    {
      // check if children are present already
      if (m_child[0] && m_child[1])
      {
        // try inserting into first child
        AtlasNode* node = m_child[0]->insert(entry);
        if (node)
        {
          // success
          return node;
        }

        // try inserting into second child
        return m_child[1]->insert(entry);
      }

      // check if any image entry already assigned
      if (NULL != m_entry)
      {
        // cannot insert
        return NULL;
      }

      // check if image is too big
      // NOTE: spacing: x - left, y - top, z - right, w - bottom
      EGE::Vector4i spacing = entry->spacing();

      EGE::s32 width = entry->image()->width() + spacing.x + spacing.z;
      EGE::s32 height = entry->image()->height() + spacing.y + spacing.w;

      if ((width > m_rect.width) || (height > m_rect.height))
      {
        // cannot insert
        return NULL;
      }

      // check if image can fits perfectly
      if ((width == m_rect.width) && (height == m_rect.height))
      {
        // found
        return this;
      }

      // split by creating kids
      m_child[0] = new AtlasNode();
      m_child[1] = new AtlasNode();
      if (NULL == m_child[0] || NULL == m_child[1])
      {
        // error!
        return NULL;
      }

      // decide way to split
      EGE::s32 dw = m_rect.width - width;
      EGE::s32 dh = m_rect.height - height;

      if (dw > dh)
      {
        // divide into left and right sections
        m_child[0]->m_rect = EGE::Recti(m_rect.x, m_rect.y, width, m_rect.height);
        m_child[1]->m_rect = EGE::Recti(m_rect.x + width, m_rect.y, m_rect.width - width, m_rect.height);
      }
      else
      {
        // divide into top and bottom sections
        m_child[0]->m_rect = EGE::Recti(m_rect.x, m_rect.y, m_rect.width, height);
        m_child[1]->m_rect = EGE::Recti(m_rect.x, m_rect.y + height, m_rect.width, m_rect.height - height);
      }

      // insert into first child
      return m_child[0]->insert(entry);
    }

  public:

    /*! Pointer to children objects. */
    AtlasNode* m_child[2];
    /*! Rectangular area occupied by node. (in global coords) */
    EGE::Recti m_rect;
    /*! Currently assigned group entry. */
    AtlasGroupEntry* m_entry;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ATLAS_NODE