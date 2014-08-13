#ifndef EGE_CORE_RESOURCETEXT_H
#define EGE_CORE_RESOURCETEXT_H

/** Text resource definition class. This object (resource) contains text string localization data.
 */

#include "EGE.h"
#include "Core/Resource/Resource.h"
#include "EGEXml.h"
#include "EGEMap.h"
#include "EGETextArray.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceText, PResourceText)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceText : public IResource
{
  public:

    virtual ~ResourceText();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Engine& engine, ResourceGroup* group);

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PXmlElement& tag) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Returns text translation. */
    Text text(s32 numerous = -1) const;

  private:

    ResourceText(Engine& engine, ResourceGroup* group);
    /*! Adds text localization. */
    EGEResult addLocalization(const PXmlElement& tag);
    /*! Returns index of translation for given numerous. */
    s32 translationIndex(s32 numerous) const;

  private:

    /*! Name. */
    String m_name;
    /*! Translation map. Sorted by language identifier. 
     *  @note TextArray contains all available froms starting with singular and then all types of plural ones.
     */
    Map<String, TextArray> m_translations;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCETEXT_H
