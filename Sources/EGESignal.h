#ifndef EGE_SIGNAL_H
#define EGE_SIGNAL_H

#include "Core/Signal/Delegate.h"
#include "Core/Signal/Signal.h"

using namespace Gallant;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define ege_connect(sender, signal, receiver, slot) if (NULL != (sender)) (sender)->signal.Connect((receiver), &slot)
#define ege_disconnect(sender, signal, receiver, slot) if (NULL != (sender)) (sender)->signal.Disconnect((receiver), &slot)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_SIGNAL_H