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

#ifndef __azurite_msg_h__
#define __azurite_msg_h__

#include "azurite-types.h"
#include "azurite-def.h"

/** #AZURITE_X_MSG_CODE message/function identifier */
typedef enum AZURITE_X_MSG_CODE {
  SXM_CREATE  = 0,
  SXM_DESTROY = 1,
  SXM_SIZE    = 2,
  SXM_PAINT   = 3,
  SXM_RESOLUTION = 4,
  SXM_HEARTBIT = 5,
  SXM_MOUSE = 6,
  SXM_KEY = 7,
  SXM_FOCUS = 8,
} AZURITE_X_MSG_CODE;

/** #AZURITE_X_MSG common header of message structures passed to AzuriteProcX */
typedef struct AZURITE_X_MSG
{ 
  UINT msg;     /**< [in]  one of the codes of #AZURITE_X_MSG_CODE.*/
#ifdef __cplusplus
  AZURITE_X_MSG(UINT m) : msg(m) {}
#endif
} AZURITE_X_MSG;

typedef struct AZURITE_X_MSG_CREATE
{
  AZURITE_X_MSG header;
          UINT backend;
          SBOOL transparent;
#ifdef __cplusplus
   AZURITE_X_MSG_CREATE(UINT backendType = GFX_LAYER_SKIA_OPENGL, SBOOL isTransparent = TRUE ) 
     : header(SXM_CREATE), backend(backendType), transparent(isTransparent) {}
#endif
} AZURITE_X_MSG_CREATE;

typedef struct AZURITE_X_MSG_DESTROY {
  AZURITE_X_MSG header;
#ifdef __cplusplus
  AZURITE_X_MSG_DESTROY() : header(SXM_DESTROY) {}
#endif
} AZURITE_X_MSG_DESTROY;

typedef struct AZURITE_X_MSG_SIZE {
  AZURITE_X_MSG header;
          UINT width;
          UINT height;
#ifdef __cplusplus
  AZURITE_X_MSG_SIZE(UINT w, UINT h) : header(SXM_SIZE), width(w), height(h) {}
#endif
} AZURITE_X_MSG_SIZE;

typedef struct AZURITE_X_MSG_RESOLUTION {
  AZURITE_X_MSG header;
  UINT pixelsPerInch;
#ifdef __cplusplus
  AZURITE_X_MSG_RESOLUTION(UINT ppi) : header(SXM_RESOLUTION), pixelsPerInch(ppi) {}
#endif
} AZURITE_X_MSG_RESOLUTION;

typedef struct AZURITE_X_MSG_MOUSE {
  AZURITE_X_MSG    header;
  MOUSE_EVENTS    event;
  MOUSE_BUTTONS   button;
  KEYBOARD_STATES modifiers;
  POINT           pos;
#ifdef __cplusplus
  AZURITE_X_MSG_MOUSE(MOUSE_EVENTS e, MOUSE_BUTTONS b, KEYBOARD_STATES mods, POINT p) : header(SXM_MOUSE), event(e), button(b), modifiers(mods), pos(p) {}
#endif
} AZURITE_X_MSG_MOUSE;

typedef struct AZURITE_X_MSG_KEY {
  AZURITE_X_MSG    header;
  KEY_EVENTS      event;
  UINT            code;
  KEYBOARD_STATES modifiers;
#ifdef __cplusplus
  AZURITE_X_MSG_KEY(KEY_EVENTS e, UINT c, KEYBOARD_STATES mods) : header(SXM_KEY), event(e), code(c), modifiers(mods) {}
#endif
} AZURITE_X_MSG_KEY;

typedef struct AZURITE_X_MSG_FOCUS {
  AZURITE_X_MSG    header;
  SBOOL            got; // true - got, false - lost
#ifdef __cplusplus
  AZURITE_X_MSG_FOCUS(SBOOL g) : header(SXM_FOCUS), got(g) {}
#endif
} AZURITE_X_MSG_FOCUS;


typedef struct AZURITE_X_MSG_HEARTBIT {
  AZURITE_X_MSG header;
  UINT time;
#ifdef __cplusplus
  AZURITE_X_MSG_HEARTBIT(UINT t) : header(SXM_HEARTBIT), time(t) {}
#endif
} AZURITE_X_MSG_HEARTBIT;


/** #ELEMENT_BITMAP_RECEIVER - callback function that receives pointer to pixmap and location
* \param[in] bgra \b LPCBYTE, pointer to BGRA bitmap, number of bytes = width * height * 4
* \param[in] x \b INT, position of the bitmap on elements window.
* \param[in] y \b INT, position of the bitmap on elements window.
* \param[in] width \b UINT, width of bitmap (and element's shape).
* \param[in] height \b UINT, height of bitmap (and element's shape).
* \param[in] param \b LPVOID, param that passed as AZURITE_X_MSG_PAINT::receiver::param .
**/
typedef VOID SC_CALLBACK ELEMENT_BITMAP_RECEIVER(LPCBYTE rgba, INT x, INT y, UINT width, UINT height, LPVOID param);

/** #AZURITE_X_MSG_PAINT target identifier. */
typedef enum AZURITE_PAINT_TARGET_TYPE {
  SPT_DEFAULT   = 0,  /**< default rendering target - window surface */    
  SPT_RECEIVER  = 1,  /**< target::receiver fields are valid */    
  SPT_SURFACE   = 2,  /**< target::pSurface is valid */
  //SPT_OPENGL    = 3,  /**< target is not used - caller shall set current context on its side  */
  //SPT_OPENGLES  = 4,  /**< target is not used - caller shall set current context on its side  */
  //SPT_
} AZURITE_PAINT_TARGET_TYPE;

typedef struct AZURITE_X_MSG_PAINT {
  AZURITE_X_MSG header;
      HELEMENT element;    /**< [in] layer #HELEMENT, can be NULL if whole tree (document) needs to be rendered.*/
      SBOOL     isFore;     /**< [in] if element is not null tells if that element is fore-layer.*/    
      UINT     targetType; /**< [in] one of #AZURITE_PAINT_TARGET_TYPE values */
      union {
        LPVOID pSurface;   /**< [in] must be IDXGISurface* */
        struct {
          VOID*                    param;
          ELEMENT_BITMAP_RECEIVER* callback;
        } receiver;
      } target;

#ifdef __cplusplus
  AZURITE_X_MSG_PAINT(HELEMENT layerElement = NULL, SBOOL foreLayer = TRUE) : header(SXM_PAINT), element(layerElement), isFore(foreLayer), targetType(SPT_DEFAULT) {}
#endif
} AZURITE_X_MSG_PAINT;

#endif
