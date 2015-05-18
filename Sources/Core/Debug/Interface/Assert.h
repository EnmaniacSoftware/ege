#ifndef EGE_CORE_DEBUG_ASSERT_H
#define EGE_CORE_DEBUG_ASSERT_H

/** This object represents debug root for framework. Most debugging tools and functionalities have root in this class. 
 */

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef void (*egeAssertFunc)(const char* fileName, s32 lineNo);
typedef void (*egeAssertXFunc)(const char* reason, const char* fileName, s32 lineNo);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Assert
{
  public:

    /*! Installs/Uninstalls custom assert handlers.
     *  @param  assertFunc  Custom function to handle simple assertions. If NULL, default one will be used.
     *  @param  assertXFunc Cusomt function to handle assertions with descriptions. If NULL, default one will be used.
     */
    static void InstallHandlers(egeAssertFunc assertFunc, egeAssertXFunc assertXFunc);

    /*! Handles simple assertion.
     *  @param  fileName  File name assertion has been triggered in.
     *  @param  lineNo    Source file line number at which assertion has been triggered.
     */
    static void HandleAssert(const char* fileName, s32 lineNo);

    /*! Handles assertion with description.
     *  @param  reason    Description associated with assertion.
     *  @param  fileName  File name assertion has been triggered in.
     *  @param  lineNo    Source file line number at which assertion has been triggered.
     */
    static void HandleAssertX(const char* reason, const char* fileName, s32 lineNo);

  private:

    /*! Default simple assertion handler. */
    static void DefaultHandleAssert(const char* fileName, s32 lineNo);
    /*! Default described assertion handler. */
    static void DefaultHandleAssertX(const char* reason, const char* fileName, s32 lineNo);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#if defined (EGE_FEATURE_DEBUG) || defined(EGE_FEATURE_ASSERT)
  #define EGE_ASSERT(cond) ((!(cond)) ? Assert::HandleAssert(__FILE__, __LINE__) : ege_noop())
  #define EGE_ASSERT_X(cond, reason) ((!(cond)) ? Assert::HandleAssertX(reason, __FILE__, __LINE__) : ege_noop())
#else
  #define EGE_ASSERT(cond) ege_noop()
  #define EGE_ASSERT_X(cond, reason) ege_noop()
#endif // EGE_FEATURE_DEBUG || EGE_FEATURE_ASSERT
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEBUG_ASSERT_H