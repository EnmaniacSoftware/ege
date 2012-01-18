#ifndef EGE_CORE_OBJECT_IDS_H
#define EGE_CORE_OBJECT_IDS_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define EGE_CORE_OBJECT_UID_BASE            (0)
#define EGE_CORE_AUDIO_OBJECT_UID_BASE      (EGE_CORE_OBJECT_UID_BASE + 0x100)
#define EGE_CORE_PHYSICS_OBJECT_UID_BASE    (EGE_CORE_OBJECT_UID_BASE + 0x200)
#define EGE_CORE_GRAPHICS_OBJECT_UID_BASE   (EGE_CORE_OBJECT_UID_BASE + 0x300)
#define EGE_CORE_RESOURCES_OBJECT_UID_BASE  (EGE_CORE_OBJECT_UID_BASE + 0x400)
#define EGE_CORE_UI_OBJECT_UID_BASE         (EGE_CORE_OBJECT_UID_BASE + 0x500)

// Generic object ids
#define EGE_OBJECT_UID_GENERIC                ((EGE_CORE_OBJECT_UID_BASE) + 0)
#define EGE_OBJECT_UID_APPCONTROLLER          ((EGE_CORE_OBJECT_UID_BASE) + 1)
#define EGE_OBJECT_UID_TIMER                  ((EGE_CORE_OBJECT_UID_BASE) + 2)
#define EGE_OBJECT_UID_INT                    ((EGE_CORE_OBJECT_UID_BASE) + 3)
#define EGE_OBJECT_UID_FLOAT                  ((EGE_CORE_OBJECT_UID_BASE) + 4)
#define EGE_OBJECT_UID_XML_DOCUMENT           ((EGE_CORE_OBJECT_UID_BASE) + 5)
#define EGE_OBJECT_UID_FILE                   ((EGE_CORE_OBJECT_UID_BASE) + 6)
#define EGE_OBJECT_UID_DATA_BUFFER            ((EGE_CORE_OBJECT_UID_BASE) + 7)
#define EGE_OBJECT_UID_POINTER_DATA           ((EGE_CORE_OBJECT_UID_BASE) + 8)

// Graphics related object ids
#define EGE_OBJECT_UID_GRAPHICS               ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 0))
#define EGE_OBJECT_UID_IMAGE                  ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 1))
#define EGE_OBJECT_UID_TEXTURE_2D             ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 2))
#define EGE_OBJECT_UID_TEXTURE_IMAGE          ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 3))
#define EGE_OBJECT_UID_RENDER_COMPONENT       ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 4))
#define EGE_OBJECT_UID_OVERLAY                ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 5))
#define EGE_OBJECT_UID_OVERLAY_TEXT           ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 6))
#define EGE_OBJECT_UID_OVERLAY_IMAGE          ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 7))
#define EGE_OBJECT_UID_VERTEX_BUFFER_VA       ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 8))
#define EGE_OBJECT_UID_VERTEX_BUFFER_VBO      ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 9))
#define EGE_OBJECT_UID_INDEX_BUFFER_VA        ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 10))
#define EGE_OBJECT_UID_INDEX_BUFFER_VBO       ((EGE_CORE_GRAPHICS_OBJECT_UID_BASE + 11))

// Physics related object ids
#define EGE_OBJECT_UID_PHYSICS_COMPONENT      ((EGE_CORE_PHYSICS_OBJECT_UID_BASE + 0))
#define EGE_OBJECT_UID_PHYSICS_JOINT_DISTANCE ((EGE_CORE_PHYSICS_OBJECT_UID_BASE + 1))
#define EGE_OBJECT_UID_PHYSICS_JOINT_ATTRACT  ((EGE_CORE_PHYSICS_OBJECT_UID_BASE + 2))
#define EGE_OBJECT_UID_PHYSICS_JOINT_PULLEY   ((EGE_CORE_PHYSICS_OBJECT_UID_BASE + 3))

// Resources related object ids
#define EGE_OBJECT_UID_RESOURCE               ((EGE_CORE_RESOURCES_OBJECT_UID_BASE) + 0)

// Audio related object ids
#define EGE_OBJECT_UID_AUDIO_CODEC_WAV        ((EGE_CORE_AUDIO_OBJECT_UID_BASE) + 0)
#define EGE_OBJECT_UID_AUDIO_CODEC_OGG        ((EGE_CORE_AUDIO_OBJECT_UID_BASE) + 1)
#define EGE_OBJECT_UID_AUDIO_CODEC_MP3        ((EGE_CORE_AUDIO_OBJECT_UID_BASE) + 2)

// UI related object ids
#define EGE_OBJECT_UID_UI_DIALOG              ((EGE_CORE_UI_OBJECT_UID_BASE) + 0)
#define EGE_OBJECT_UID_UI_SCROLLABLE_AREA     ((EGE_CORE_UI_OBJECT_UID_BASE) + 1)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_OBJECT_IDS_H