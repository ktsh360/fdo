// KingKML.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

#ifdef _WIN32

static wchar_t g_AppFileName[MAX_PATH];
wchar_t g_HomeDir[MAX_PATH];
wchar_t g_LogFileName[MAX_PATH];


FdoPtr<c_KgKmlFdoClassConfigCollection> g_ConfigXML;

BOOL APIENTRY DllMain( HMODULE Module,
                       DWORD  Reason,
                       LPVOID lpReserved
					 )
{
  DWORD nchars;
  wchar_t* last;
  BOOL ret;

  // UNCOMMENT THE FOLLOWING CODE TO HELP DISCOVER MEMORY LEAKS OR PREMATURE MEMORY DEALLOCATIONS;
  // WARNING: THIS CAN MAKE THE CODE RUN EXTREMELY SLOWLY IN CERTAIN PLACES!
  //
  //int debugFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
  //_CrtSetDbgFlag(debugFlags | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);



  ret = TRUE;
  if ( Reason == DLL_PROCESS_ATTACH )
  {
    nchars = GetModuleFileNameW ((HINSTANCE)Module, g_AppFileName, MAX_PATH);
    if (0 == nchars)
      ret = FALSE;
    else
    {   
      // scan the string for the last occurrence of a slash
      wcscpy (g_HomeDir, g_AppFileName);
      last = wcsrchr (g_HomeDir, L'\\');
      if (NULL == last)
        ret = FALSE;
      else
      {
        last++; // move past the slash
        *last = L'\0'; // null terminate it there
      }

      wcscpy (g_LogFileName, g_HomeDir);
      wcscat (g_LogFileName, L"\\KingKml.log");
    }
    
    
    g_ConfigXML = new c_KgKmlFdoClassConfigCollection();
    g_ConfigXML->ReadConfigXML();
    
  }

  return (ret);
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#endif