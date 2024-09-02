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

#ifndef __aux_platform_h__
#define __aux_platform_h__

/*
 * Terra Informatica Azurite and HTMLayout Engines
 * http://terrainformatica.com/azurite, http://terrainformatica.com/htmlayout
 *
 * platform primitives.
 *
 * The code and information provided "as-is" without
 * warranty of any kind, either expressed or implied.
 *
 * (C) 2003-2015, Andrew Fedoniouk (andrew@terrainformatica.com)
 */


#if defined(WIN64) || defined(_WIN64) || defined(_M_X64)
  #define WINDOWS
  #define X64BITS
#elif defined(WIN32) || defined(_WIN32)
  #define WINDOWS
#elif defined(__APPLE__)
  #define OSX
  #define UTF8_CHARS // const char* is UTF8 sequence
  #ifdef __x86_64__
    #define X64BITS
  #endif
  #define POSIX
#elif defined( __linux__ )
  #ifndef LINUX
    #define LINUX
  #endif
  #ifdef __x86_64__
    #define X64BITS
  #endif
  #define POSIX
  #define UTF8_CHARS // const char* is UTF8 sequence
#else
  #error "Unknown platform"
#endif

#if defined(WINDOWS)
  #define stricmp _stricmp
  #define wcsicmp _wcsicmp
#elif defined(POSIX)
  #define stricmp strcasecmp
  #define wcsicmp wcscasecmp
#endif

#endif
