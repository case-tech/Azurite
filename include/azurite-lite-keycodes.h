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

#ifndef __azurite_lite_keycodes_h__
#define __azurite_lite_keycodes_h__

enum KEYBOARD_CODES {   // table matches GLFW table

  KB_SPACE              = 32,
  KB_APOSTROPHE         = 39,  /* ' */
  KB_COMMA              = 44,  /* , */ KB_QUOTE = KB_COMMA,
  KB_MINUS              = 45,  /* - */
  KB_PERIOD             = 46,  /* . */
  KB_SLASH              = 47,  /* / */
  KB_0                  = 48,
  KB_1                  = 49,
  KB_2                  = 50,
  KB_3                  = 51,
  KB_4                  = 52,
  KB_5                  = 53,
  KB_6                  = 54,
  KB_7                  = 55,
  KB_8                  = 56,
  KB_9                  = 57,
  KB_SEMICOLON          = 59,  /* ; */
  KB_EQUAL              = 61,  /* = */
  KB_A                  = 65,
  KB_B                  = 66,
  KB_C                  = 67,
  KB_D                  = 68,
  KB_E                  = 69,
  KB_F                  = 70,
  KB_G                  = 71,
  KB_H                  = 72,
  KB_I                  = 73,
  KB_J                  = 74,
  KB_K                  = 75,
  KB_L                  = 76,
  KB_M                  = 77,
  KB_N                  = 78,
  KB_O                  = 79,
  KB_P                  = 80,
  KB_Q                  = 81,
  KB_R                  = 82,
  KB_S                  = 83,
  KB_T                  = 84,
  KB_U                  = 85,
  KB_V                  = 86,
  KB_W                  = 87,
  KB_X                  = 88,
  KB_Y                  = 89,
  KB_Z                  = 90,
  KB_LEFT_BRACKET       = 91,  /* [ */ KB_LEFTBRACKET = KB_LEFT_BRACKET,
  KB_BACKSLASH          = 92,  /* \ */
  KB_RIGHT_BRACKET      = 93,  /* ] */ KB_RIGHTBRACKET = KB_RIGHT_BRACKET,
  KB_GRAVE_ACCENT       = 96,  /* ` */
  KB_WORLD_1            = 161, /* non-US #1 */ KB_DOT = KB_WORLD_1,
  KB_WORLD_2            = 162, /* non-US #2 */

/* Function keys */ 
  KB_ESCAPE             = 256,
  KB_ENTER              = 257, KB_RETURN = KB_ENTER,
  KB_TAB                = 258,
  KB_BACKSPACE          = 259, KB_BACK = KB_BACKSPACE, 
  KB_INSERT             = 260,
  KB_DELETE             = 261,
  KB_RIGHT              = 262,
  KB_LEFT               = 263,
  KB_DOWN               = 264,
  KB_UP                 = 265,
  KB_PAGE_UP            = 266, KB_PRIOR = KB_PAGE_UP,
  KB_PAGE_DOWN          = 267, KB_NEXT = KB_PAGE_DOWN,
  KB_HOME               = 268,
  KB_END                = 269,
  KB_CAPS_LOCK          = 280, KB_CAPITAL = KB_CAPS_LOCK,
  KB_SCROLL_LOCK        = 281, KB_SCROLL = KB_SCROLL_LOCK,
  KB_NUM_LOCK           = 282, KB_NUMLOCK = KB_NUM_LOCK,
  KB_PRINT_SCREEN       = 283,
  KB_PAUSE              = 284,
  KB_F1                 = 290,
  KB_F2                 = 291,
  KB_F3                 = 292,
  KB_F4                 = 293,
  KB_F5                 = 294,
  KB_F6                 = 295,
  KB_F7                 = 296,
  KB_F8                 = 297,
  KB_F9                 = 298,
  KB_F10                = 299,
  KB_F11                = 300,
  KB_F12                = 301,
  KB_F13                = 302,
  KB_F14                = 303,
  KB_F15                = 304,
  KB_F16                = 305,
  KB_F17                = 306,
  KB_F18                = 307,
  KB_F19                = 308,
  KB_F20                = 309,
  KB_F21                = 310,
  KB_F22                = 311,
  KB_F23                = 312,
  KB_F24                = 313,
  KB_F25                = 314,
  KB_NUMPAD0            = 320,
  KB_NUMPAD1            = 321,
  KB_NUMPAD2            = 322,
  KB_NUMPAD3            = 323,
  KB_NUMPAD4            = 324,
  KB_NUMPAD5            = 325,
  KB_NUMPAD6            = 326,
  KB_NUMPAD7            = 327,
  KB_NUMPAD8            = 328,
  KB_NUMPAD9            = 329,
  KB_NUMPAD_DECIMAL     = 330, KB_DECIMAL = KB_NUMPAD_DECIMAL, KB_SEPARATOR = KB_DECIMAL,
  KB_NUMPAD_DIVIDE      = 331, KB_DIVIDE = KB_NUMPAD_DIVIDE,
  KB_NUMPAD_MULTIPLY    = 332, KB_MULTIPLY = KB_NUMPAD_MULTIPLY,
  KB_NUMPAD_SUBTRACT    = 333, KB_SUBTRACT = KB_NUMPAD_SUBTRACT,
  KB_NUMPAD_ADD         = 334, KB_ADD = KB_NUMPAD_ADD, KB_PLUS = KB_ADD,
  KB_NUMPAD_ENTER       = 335, 
  KB_NUMPAD_EQUAL       = 336, 
  KB_LEFT_SHIFT         = 340, KB_SHIFT = KB_LEFT_SHIFT,
  KB_LEFT_CONTROL       = 341, KB_CONTROL = KB_LEFT_CONTROL, KB_SHORTCUT = KB_CONTROL,
  KB_LEFT_ALT           = 342, 
  KB_LEFT_SUPER         = 343,
  KB_RIGHT_SHIFT        = 344,
  KB_RIGHT_CONTROL      = 345,
  KB_RIGHT_ALT          = 346,
  KB_RIGHT_SUPER        = 347,
  KB_MENU               = 348,
};

#endif