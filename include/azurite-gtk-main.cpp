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

#include <gtk/gtk.h>
#include <locale.h>

#include "azurite-window.hpp"

static std::vector<azurite::string> _argv;

#ifndef SKIP_MAIN
int main (int argc, char *argv[])
{
  /* Initialize GTK+ */
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  setlocale(LC_TIME,"");

  for( int i = 0; i < argc; ++i ) {
      aux::a2w w(argv[i]);
      _argv.push_back(azurite::string(w.c_str(),w.length()));
  }

  auto message_pump = []() -> int {
    gtk_main ();
    return 0;
  };

  return uimain(message_pump);
}
#endif


namespace azurite {

  GtkWidget* gview(HWINDOW hwnd) { return hwnd; }
  GtkWindow* gwindow(HWINDOW hwnd) { return hwnd ? GTK_WINDOW(gtk_widget_get_toplevel(hwnd)): nullptr; }

  namespace application {
    HINSTANCE hinstance()
    {
      return nullptr; // not used
    }

    const std::vector<azurite::string>& argv() {
      return _argv;
    }
  }

  void window::collapse() {
    if(_hwnd) gtk_window_iconify (gwindow(_hwnd));
  }
  void window::expand( bool maximize) {
    if(_hwnd) gtk_window_present (gwindow(_hwnd));
  }

  void window::dismiss() {
    if(_hwnd) gtk_window_close (gwindow(_hwnd));
    _hwnd = 0; //?
  }

  window::window( UINT creationFlags, RECT frame): _hwnd(NULL)
  {
    asset_add_ref();
    _hwnd = SAPI()->AzuriteCreateWindow(creationFlags, (frame.right - frame.left) > 0 ? &frame: NULL,NULL,this,NULL);
  }

}
