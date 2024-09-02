// Copyright(c) 2024  Case Technologies 

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef __AZURITE_DEBUG_H__
#define __AZURITE_DEBUG_H__

//|
//| Azurite Inspector/Debug API
//|

#include "azurite-api.h"

inline  VOID    SCAPI AzuriteSetupDebugOutput ( HWINDOW hwndOrNull, LPVOID param, DEBUG_OUTPUT_PROC pfOutput) { return SAPI()->AzuriteSetupDebugOutput ( hwndOrNull,param,pfOutput); }

#if defined(__cplusplus) && !defined( PLAIN_API_ONLY )

  namespace azurite
  {

    struct debug_output
    {
     
      debug_output(HWINDOW hwnd = 0)
      {
        setup_on(hwnd);
      }

      void setup_on(HWINDOW hwnd = 0)
      {
        ::AzuriteSetupDebugOutput(hwnd,this,_output_debug);
        instance(this);
      }

      static debug_output* instance(debug_output* pi = nullptr) {
        static debug_output* _instance = nullptr;
        if (pi) _instance = pi;
        return _instance;
      }
      
      static VOID SC_CALLBACK _output_debug(LPVOID param, UINT subsystem, UINT severity, LPCWSTR text, UINT text_length)
      {
        static_cast<debug_output*>(param)->output((OUTPUT_SUBSYTEMS)subsystem,(OUTPUT_SEVERITY)severity, (const WCHAR*)text,text_length);
      }
      virtual void output( OUTPUT_SUBSYTEMS subsystem, OUTPUT_SEVERITY severity, const WCHAR* text, unsigned int text_length )
      {
        switch(severity)
        {
           case OS_INFO     : print("info:"); break;
           case OS_WARNING  : print("warning:"); break;
           case OS_ERROR    : print("error:"); break;
        }
        switch(subsystem)
        {
           case OT_DOM: print("DOM: "); break;
           case OT_CSSS: print("csss!: "); break;
           case OT_CSS:  print("css: "); break;
           case OT_TIS:  print("script: "); break;
        }
        if(text[text_length])
        {
          //unsigned n = wcslen(text);
          assert(false);
        }
        else
          print(text);
      }
#if defined(WINDOWS)
      virtual void print(const WCHAR* text) { OutputDebugStringW(text); }
      virtual void print(const char* text) { OutputDebugStringA(text); }
#elif defined(OSX) || defined(LINUX)
      virtual void print(const WCHAR* text) { std::cout << aux::w2a(text).c_str(); }
      virtual void print(const char* text) { std::cout << text; }
#endif

      void printf( const char* fmt, ... )
      {
        char buffer [ 2049 ];
        va_list args;
        va_start ( args, fmt );
#if _MSC_VER == 1400
        int len = vsnprintf( buffer, sizeof(buffer), _TRUNCATE, fmt, args );
#else
        int len = vsnprintf( buffer, sizeof(buffer), fmt, args );
#endif
        va_end ( args );
        buffer [ len ] = 0;
        buffer [ 2048 ] = 0;
        print(buffer);
      }

/*      void printf( const WCHAR* fmt, ... )
      {
        WCHAR buffer [ 2049 ];
        va_list args;
        va_start ( args, fmt );
#if defined(WINDOWS)
        int len = vswprintf_s( buffer, fmt, args );
#elif defined(OSX)
        int len = vswprintf( buffer, 2048, fmt, args );
#endif
        va_end ( args );
        buffer [ len ] = 0;
        buffer [ 2048 ] = 0;
        print(buffer);
      } */


    };

    struct debug_output_console: public debug_output
    {

#ifdef _WIN32_WCE
      FILE *f;
      ~debug_output_console()
      {
        fclose(f);
      }
#endif
      debug_output_console():debug_output()
      {
#ifdef _WINCE32_WCE
        f = 0;
#endif
      }
#if !defined(_WIN32_WCE)
      virtual void print(const WCHAR* text) /*override*/
      {
        print(aux::w2oem(text));
      }
      virtual void print(const char* text) /*override*/
      {
#if defined(WINDOWS)
        static bool initialized = false;
        if(!initialized)
        {
          AllocConsole();
#pragma warning( push )
#pragma warning(disable:4996)
          freopen("conin$", "r", stdin);
          freopen("conout$", "w", stdout);
          freopen("conout$", "w", stderr);
#if 0          
          DWORD cm;
          if(GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),&cm))
            SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), cm | ENABLE_VIRTUAL_TERMINAL_INPUT);
          if (GetConsoleMode(GetStdHandle(STD_ERROR_HANDLE), &cm))
            SetConsoleMode(GetStdHandle(STD_ERROR_HANDLE), cm | ENABLE_VIRTUAL_TERMINAL_INPUT);
#endif            
#pragma warning( pop )
          initialized = true;
        }
#endif
        while(text && *text) {
          char c = *text++;
          if(c == '\r')
            putchar('\n');
          else
            putchar(c);
        }
      }
#else
      virtual void print(const WCHAR* text) /*override*/
      {
        if( !f )
          f = fopen("\\moazurite.log", "wb");
        fputws(text, f);
      }
      virtual void print(const char* text) /*override*/
      {
        if( !f )
          f = fopen("\\moazurite.log", "wb");
        fputs(text, f);
      }
#endif

    };

  }

#endif

#endif
