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

#ifndef __azurite_window_hpp__
#define __azurite_window_hpp__

#include "azurite-types.h"
#include "azurite.h"
#include "azurite-dom.h"
#include "azurite-api.h"
//#include "azurite-threads.h"
#include "azurite-dom.hpp"
#include "azurite-host-callback.h"
#include "aux-asset.h"
#include "aux-slice.h"
#include <algorithm>

// main UI application routine
int uimain( std::function<int()> run );
// {
//   ... initialization and window creation
//   int r = run();
//   ... optional de-initialization
//   return r;
// }

/**azurite namespace.*/
namespace azurite
{

  namespace application
  {
    const std::vector<azurite::string>& argv();
    HINSTANCE                          hinstance();
  }

  class window : public azurite::event_handler
               , public azurite::host<window>
  {
    friend azurite::host<window>;
  public:
    window( UINT creationFlags, RECT frame = RECT() );
    //virtual ~window() {}

    bool is_valid() const { return _hwnd != 0; }

    virtual long asset_add_ref() { return asset::asset_add_ref(); }
    virtual long asset_release() { return asset::asset_release(); }

    void collapse(); // minimize
    void expand( bool maximize = false); // show or maximize
    void dismiss(); // delete the window

    bool load(aux::bytes utf8_html, const WCHAR* base_url = 0)
    {
      bind(); return FALSE != ::AzuriteLoadHtml(_hwnd, utf8_html.start, UINT(utf8_html.length), base_url);
    }
    bool load(aux::chars utf8_html, const WCHAR* base_url = 0)
    {
      bind(); return FALSE != ::AzuriteLoadHtml(_hwnd, (LPCBYTE)utf8_html.start, UINT(utf8_html.length), base_url);
    }
    bool load(const WCHAR* url)
    {
      bind(); return FALSE != ::AzuriteLoadFile(_hwnd, url);
    }

  // azurite::host traits:
    HWINDOW   get_hwnd() const { return _hwnd; }
    HINSTANCE get_resource_instance() const { return application::hinstance(); }

    //azurite::om::iasset
    static const char* interface_name() { return "window.azurite.com"; }

  protected:

    void bind() {
      if (_hwnd && !_bound) {
        _bound = true;
        setup_callback();
        azurite::attach_dom_event_handler(get_hwnd(), this);
      }
    }

    virtual LRESULT on_engine_destroyed() 
    { 
      _hwnd = 0; asset_release();
      return 0; 
    }

#if defined(WINDOWS)
    virtual LRESULT on_message( HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam, SBOOL& handled );
    static LRESULT SC_CALLBACK msg_delegate(HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LPVOID pParam, SBOOL* pHandled);
#endif
  private:
     HWINDOW _hwnd;
     bool    _bound = false;
   };
}

#endif
