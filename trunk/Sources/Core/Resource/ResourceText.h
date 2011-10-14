#ifndef EGE_CORE_RESOURCETEXT_H
#define EGE_CORE_RESOURCETEXT_H

/** Text resource definition class. This object (resource) contains text string localization data.
 */

#include <EGE.h>
#include "Core/Resource/Resource.h"
#include <EGEXml.h>
#include <EGEMap.h>
#include <EGEDynamicArray.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceText, PResourceText)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceText : public IResource
{
  public:

    virtual ~ResourceText();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceManager* manager);

    /* IResource override. Returns name of resource. */
    const String& name() const override;
    /* Initializes resource from XML. 
    * 
    *  \param  path  full path to resource definition file.
    *  \param  tag   xml element with resource definition. 
    */
    EGEResult create(const String& path, const PXmlElement& tag) override;
    /* IResource override. Loads resource. */
    EGEResult load() override;
    /* IResource override. Unloads resource. */
    void unload() override;
    /* Returns text translation. */
    Text text(s32 numerous = -1) const;

  private:

    ResourceText(Application* app, ResourceManager* manager);
    /*! Returns TRUE if object is loaded. */
    inline bool isLoaded() const { return true; }
    /* Adds text localization. */
    EGEResult addLocalization(const PXmlElement& tag);
    /* Returns index of translation for given numerous. */
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
