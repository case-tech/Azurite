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

#pragma once

#ifndef __azurite_windowless_hpp__
#define __azurite_windowless_hpp__

#include "azurite-types.h"
#include "azurite.h"
#include "azurite-dom.h"
#include "azurite-api.h"
#include "azurite-dom.hpp"
#include "azurite-host-callback.h"
#include "aux-asset.h"
#include "aux-slice.h"
#include <algorithm>

/**azurite namespace.*/
namespace azurite
{

  class lite 
    : public azurite::event_handler
    , public azurite::host<lite>
  {
  public:
    lite(UINT backend = GFX_LAYER_SKIA_OPENGL) { // or GFX_LAYER_D2D
      SBOOL r = AzuriteProcX(this, AZURITE_X_MSG_CREATE(backend, FALSE));
      assert(r);
      if (r) {
        setup_callback();
        azurite::attach_dom_event_handler(this, this);
      }
    }
    virtual ~lite() {
      AzuriteProcX(this, AZURITE_X_MSG_DESTROY());
    }

    virtual long asset_add_ref() { return 0; /* life cycle is governed by owner */ }
    virtual long asset_release() { return 0; /* life cycle is governed by owner */ }

    bool load(aux::bytes utf8_html, const WCHAR* base_url = 0)
    {
      return FALSE != ::AzuriteLoadHtml(this, utf8_html.start, UINT(utf8_html.length), base_url);
    }
    bool load(aux::chars utf8_html, const WCHAR* base_url = 0)
    {
      return FALSE != ::AzuriteLoadHtml(this, (LPCBYTE)utf8_html.start, UINT(utf8_html.length), base_url);
    }
    bool load(const WCHAR* url)
    {
      return FALSE != ::AzuriteLoadFile(this, url);
    }

  // azurite::host traits:
    HWINDOW   get_hwnd() const { return (LPVOID)this; }
    HINSTANCE get_resource_instance() const { return NULL; }

   

  protected:

   };
}

#endif
