#include "Core/Engine/Interface/EngineInternal.h"
#include "Win32/Engine/Interface/EngineInstanceWin32.h"
#include "EGECommandLine.h"
#include "EGEDictionary.h"
#include <windows.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void run(Engine& engine)
{
  EngineInternal& engineInternal = static_cast<EngineInternal&>(reinterpret_cast<EngineInstance&>(engine));

  // main loop
  while (!engineInternal.isShutDown())
  {
    // process messages for all windows within our context
    MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      // process the message
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    // send begin of frame signal
    emit engine.signalFrameBegin();

    // update
    engineInternal.update();

    // render
    engineInternal.render();

    // send end of frame signal
    emit engine.signalFrameEnd();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR strCmdLine, INT)
{
  EGEResult result = EGE_ERROR;

  try
  {
    // initialize memory manager
    if (MemoryManager::Initialize())
    {
      // process command line
      CommandLineParser commandLineParser(strCmdLine);

      // create engine
      Engine* engine = ege_new EngineInstanceWin32();

      // construct it
      if (EGE_SUCCESS == (result = engine->construct(commandLineParser.dictionary())))
      {
        // run
        run(*engine);
      }

      // clean up
      delete engine;
    }

    // deinitialize memory manager
    MemoryManager::Deinitialize();
  }
  catch (std::bad_alloc&)
  {
    result = EGE_ERROR_NO_MEMORY;
  }
  catch (...)
  {
    result = EGE_ERROR;
  }

  return static_cast<INT>(result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
