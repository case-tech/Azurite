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

#ifndef __AZURITE__
#define __AZURITE__

#if __cplusplus > 199711L
  #define CPP11
#elif _MSC_VER >= 1600
  #define CPP11
#endif

#include "azurite-types.h"
#include "azurite-def.h"
#include "azurite-dom.h"
#include "azurite-value.h"
#include "azurite-api.h"
#include "azurite-msg.h"
#include "azurite-om.h"

#if defined(__cplusplus) && !defined( PLAIN_API_ONLY )
  // C++, namespace azurite things
  #include "azurite-dom.hpp"
  #include "azurite-host-callback.h"
  #include "azurite-debug.h"
#endif

/** Signatire of Azurite extension library entry point 
  * \param psapi #IAzuriteAPI - Azurite API to be used inside the DLL.
  * \param plibobject #AZURITE_VALUE* - value to initialize, can be native functor, azurite::om::asset, array, map, etc.
  * \return TRUE, if it populates plibobject;
  *
  * The DLL should have function exported with the name "AzuriteLibraryInit" and wit this signature.
  * In script such azurite extension library can be imported as: 
  *
  *  include library "name-without-extension";
  *
 **/

typedef SBOOL SCAPI AzuriteLibraryInitFunc(IAzuriteAPI* psapi, AZURITE_VALUE* plibobject);




#endif



