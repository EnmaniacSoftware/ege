#ifndef EGE_CORE_SEQUENCE_H
#define EGE_CORE_SEQUENCE_H

/** Sequence resource definition class. This object (resource) contains definition of sequence animator.
 */

#include <EGE.h>
#include "Core/Resource/Resource.h"
#include <EGEXml.h>
#include <EGEDynamicArray.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;
EGE_DECLARE_SMART_CLASS(ResourceSequencer, PResourceSequencer)
EGE_DECLARE_SMART_CLASS(Sequencer, PSequencer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceSequencer : public IResource
{
  public:

    virtual ~ResourceSequencer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceGroup* group);

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

    /* Creates instance of sequencer object defined by resource. */
    PSequencer createInstance();
    /* Set given instance of sequencer object to what is defined by resource. */
    EGEResult setInstance(PSequencer& instance);

  private:

    ResourceSequencer(Application* app, ResourceGroup* group);

  private:

    /*! Name. */
    String m_name;
    /*! Array of all frame ids. */
    IntArray m_frameIds;
    /*! Repeat flag. */
    bool m_repeatable;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SEQUENCE_H
