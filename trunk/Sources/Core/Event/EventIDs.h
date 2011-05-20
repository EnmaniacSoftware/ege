#ifndef EGE_CORE_EVENT_IDS_H
#define EGE_CORE_EVENT_IDS_H

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define EGE_EVENT_UID_CORE_BASE         (0)
#define EGE_EVENT_UID_CORE_QUIT_REQUEST (EGE_EVENT_UID_CORE_BASE + 1)
#define EGE_EVENT_UID_CORE_UPDATE       (EGE_EVENT_UID_CORE_BASE + 2)
#define EGE_EVENT_UID_CORE_POINTER_DATA (EGE_EVENT_UID_CORE_BASE + 3)

#define EGE_EVENT_UID_INTERNAL_BASE         (4000)
#define EGE_EVENT_UID_INTERNAL_POINTER_DATA (EGE_EVENT_UID_INTERNAL_BASE + 1)

#define EGE_EVENT_UID_USER_BASE (5000)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_EVENT_IDS_H