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

#ifndef __AZURITE_DEF__
#define __AZURITE_DEF__

#include "azurite-types.h"
#include "azurite-request.h"
#include "value.h"
#ifdef __cplusplus
  #include "aux-cvt.h"
  #include <iostream>
  #include <stdio.h>
  #include <stdarg.h>
  #include <wchar.h>
#endif


#define HAS_TISCRIPT

/** Resource data type.
 *  Used by AzuriteDataReadyAsync() function.
 **/

#define  HAS_TISCRIPT // in azurite

#include "azurite-value.h"
#include "azurite-dom.h"

/**Get name of Azurite window class.
 *
 * \return \b LPCWSTR, name of Azurite window class.
 *         \b NULL if initialization of the engine failed, Direct2D or DirectWrite are not supported on the OS.
 *
 * Use this function if you wish to create unicode version of Azurite.
 * The returned name can be used in CreateWindow(Ex)W function.
 * You can use #AzuriteClassNameT macro.
 **/
LPCWSTR SCAPI AzuriteClassName(void);

/**Returns major and minor version of Azurite engine.
  * \return UINT, hiword (16-bit) contains major number and loword contains minor number;
 **/
 UINT  SCAPI AzuriteVersion(SBOOL major);

/** #SC_LOAD_DATA notification return codes */
enum SC_LOAD_DATA_RETURN_CODES
{
  LOAD_OK = 0,      /**< do default loading if data not set */
  LOAD_DISCARD = 1, /**< discard request completely */
  LOAD_DELAYED = 2, /**< data will be delivered later by the host application.
                         Host application must call AzuriteDataReadyAsync(,,, requestId) on each LOAD_DELAYED request to avoid memory leaks. */
  LOAD_MYSELF  = 3, /**< you return LOAD_MYSELF result to indicate that your (the host) application took or will take care about HREQUEST in your code completely.
                         Use azurite-request.h[pp] API functions with SCN_LOAD_DATA::requestId handle . */
};

/**Notifies that Azurite is about to download a referred resource.
 *
 * \param lParam #LPSCN_LOAD_DATA.
 * \return #SC_LOAD_DATA_RETURN_CODES
 *
 * This notification gives application a chance to override built-in loader and
 * implement loading of resources in its own way (for example images can be loaded from
 * database or other resource). To do this set #SCN_LOAD_DATA::outData and
 * #SCN_LOAD_DATA::outDataSize members of SCN_LOAD_DATA. Azurite does not
 * store pointer to this data. You can call #AzuriteDataReady() function instead
 * of filling these fields. This allows you to free your outData buffer
 * immediately.
**/
#define SC_LOAD_DATA       0x01

/**This notification indicates that external data (for example image) download process
 * completed.
 *
 * \param lParam #LPSCN_DATA_LOADED
 *
 * This notifiaction is sent for each external resource used by document when
 * this resource has been completely downloaded. Azurite will send this
 * notification asynchronously.
 **/
#define SC_DATA_LOADED     0x02

/**This notification is sent when all external data (for example image) has been downloaded.
 *
 * This notification is sent when all external resources required by document
 * have been completely downloaded. Azurite will send this notification
 * asynchronously.
 **/
/* obsolete #define SC_DOCUMENT_COMPLETE 0x03
   use DOCUMENT_COMPLETE DOM event.
  */


/**This notification is sent on parsing the document and while processing
 * elements having non empty style.behavior attribute value.
 *
 * \param lParam #LPSCN_ATTACH_BEHAVIOR
 *
 * Application has to provide implementation of #azurite::behavior interface.
 * Set #SCN_ATTACH_BEHAVIOR::impl to address of this implementation.
 **/
#define SC_ATTACH_BEHAVIOR 0x04

/**This notification is sent when instance of the engine is destroyed.
 * It is always final notification.
 *
 * \param lParam #LPSCN_ENGINE_DESTROYED
 *
 **/
#define SC_ENGINE_DESTROYED 0x05

/**Posted notification.
 
 * \param lParam #LPSCN_POSTED_NOTIFICATION
 *
 **/
#define SC_POSTED_NOTIFICATION 0x06


/**This notification is sent when the engine encounters critical rendering error: e.g. DirectX gfx driver error.
   Most probably bad gfx drivers.
 
 * \param lParam #LPSCN_GRAPHICS_CRITICAL_FAILURE
 *
 **/
#define SC_GRAPHICS_CRITICAL_FAILURE 0x07


/**This notification is sent when the engine needs keyboard to be present on screen
   E.g. when <input|text> gets focus

 * \param lParam #LPSCN_KEYBOARD_REQUEST
 *
 **/
#define SC_KEYBOARD_REQUEST 0x08

/**This notification is sent when the engine needs some area to be redrawn
 
 * \param lParam #LPSCN_INVLIDATE_RECT
 *
 **/
#define SC_INVALIDATE_RECT 0x09


/**Notification callback structure.
 **/
typedef struct AZURITE_CALLBACK_NOTIFICATION
{
  UINT code; /**< [in] one of the codes above.*/
  HWINDOW hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/
} AZURITE_CALLBACK_NOTIFICATION;

typedef AZURITE_CALLBACK_NOTIFICATION * LPAZURITE_CALLBACK_NOTIFICATION;

typedef UINT SC_CALLBACK AzuriteHostCallback( LPAZURITE_CALLBACK_NOTIFICATION pns, LPVOID callbackParam );

typedef AzuriteHostCallback * LPAzuriteHostCallback;


/**This structure is used by #SC_LOAD_DATA notification.
 *\copydoc SC_LOAD_DATA
 **/

typedef struct SCN_LOAD_DATA
{
    UINT code;                 /**< [in] one of the codes above.*/
    HWINDOW hwnd;              /**< [in] HWINDOW of the window this callback was attached to.*/

    LPCWSTR  uri;              /**< [in] Zero terminated string, fully qualified uri, for example "http://server/folder/file.ext".*/

    LPCBYTE  outData;          /**< [in,out] pointer to loaded data to return. if data exists in the cache then this field contain pointer to it*/
    UINT     outDataSize;      /**< [in,out] loaded data size to return.*/
    UINT     dataType;         /**< [in] AzuriteResourceType */

    HREQUEST requestId;        /**< [in] request handle that can be used with azurite-request API */

    HELEMENT principal;
    HELEMENT initiator;
} SCN_LOAD_DATA;

typedef SCN_LOAD_DATA*  LPSCN_LOAD_DATA;

/**This structure is used by #SC_DATA_LOADED notification.
 *\copydoc SC_DATA_LOADED
 **/
typedef struct SCN_DATA_LOADED
{
    UINT code;                 /**< [in] one of the codes above.*/
    HWINDOW hwnd;              /**< [in] HWINDOW of the window this callback was attached to.*/

    LPCWSTR  uri;              /**< [in] zero terminated string, fully qualified uri, for example "http://server/folder/file.ext".*/
    LPCBYTE  data;             /**< [in] pointer to loaded data.*/
    UINT     dataSize;         /**< [in] loaded data size (in bytes).*/
    UINT     dataType;         /**< [in] AzuriteResourceType */
    UINT     status;           /**< [in]
                                         status = 0 (dataSize == 0) - unknown error.
                                         status = 100..505 - http response status, Note: 200 - OK!
                                         status > 12000 - wininet error code, see ERROR_INTERNET_*** in wininet.h
                                 */
} SCN_DATA_LOADED;

typedef SCN_DATA_LOADED * LPSCN_DATA_LOADED;

/**This structure is used by #SC_ATTACH_BEHAVIOR notification.
 *\copydoc SC_ATTACH_BEHAVIOR **/
typedef struct SCN_ATTACH_BEHAVIOR
{
    UINT code;                        /**< [in] one of the codes above.*/
    HWINDOW hwnd;                     /**< [in] HWINDOW of the window this callback was attached to.*/

    HELEMENT element;                 /**< [in] target DOM element handle*/
    LPCSTR   behaviorName;            /**< [in] zero terminated string, string appears as value of CSS behavior:"???" attribute.*/

    ElementEventProc* elementProc;    /**< [out] pointer to ElementEventProc function.*/
    LPVOID            elementTag;     /**< [out] tag value, passed as is into pointer ElementEventProc function.*/

} SCN_ATTACH_BEHAVIOR;
typedef SCN_ATTACH_BEHAVIOR* LPSCN_ATTACH_BEHAVIOR;

/**This structure is used by #SC_ENGINE_DESTROYED notification.
 *\copydoc SC_ENGINE_DESTROYED **/
typedef struct SCN_ENGINE_DESTROYED
{
    UINT code; /**< [in] one of the codes above.*/
    HWINDOW hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/
} SCN_ENGINE_DESTROYED;

typedef SCN_ENGINE_DESTROYED* LPSCN_ENGINE_DESTROYED;

/**This structure is used by #SC_ENGINE_DESTROYED notification.
 *\copydoc SC_ENGINE_DESTROYED **/
typedef struct SCN_POSTED_NOTIFICATION
{
    UINT      code; /**< [in] one of the codes above.*/
    HWINDOW   hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/
    UINT_PTR  wparam;
    UINT_PTR  lparam;
    UINT_PTR  lreturn;
} SCN_POSTED_NOTIFICATION;

typedef SCN_POSTED_NOTIFICATION* LPSCN_POSTED_NOTIFICATION;

/**This structure is used by #SC_GRAPHICS_CRITICAL_FAILURE notification.
 *\copydoc SC_GRAPHICS_CRITICAL_FAILURE **/
typedef struct SCN_GRAPHICS_CRITICAL_FAILURE
{
    UINT      code; /**< [in] = SC_GRAPHICS_CRITICAL_FAILURE */
    HWINDOW   hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/
} SCN_GRAPHICS_CRITICAL_FAILURE;

typedef SCN_GRAPHICS_CRITICAL_FAILURE* LPSCN_GRAPHICS_CRITICAL_FAILURE;

/**This structure is used by #SC_KEYBOARD_REQUEST notification.
 *\copydoc SC_KEYBOARD_REQUEST **/
typedef struct SCN_KEYBOARD_REQUEST {
  UINT    code; /**< [in] = SC_KEYBOARD_REQUEST */
  HWINDOW hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/
  UINT    keyboardMode; /**< [in] 0 - hide keyboard, 1 ... type of keyboard, TBD */
} SCN_KEYBOARD_REQUEST;

typedef SCN_KEYBOARD_REQUEST *LPSCN_KEYBOARD_REQUEST;

/**This structure is used by #SC_INVALIDATE_RECT notification.
 *\copydoc SC_INVALIDATE_RECT **/
typedef struct SCN_INVALIDATE_RECT {
  UINT    code; /**< [in] = SC_INVALIDATE_RECT */
  HWINDOW hwnd; /**< [in] HWINDOW of the window this callback was attached to.*/
  RECT    invalidRect; /**< [in] cumulative invalid rect.*/
} SCN_INVALIDATE_RECT;

typedef SCN_INVALIDATE_RECT *LPSCN_INVALIDATE_RECT;


#include "azurite-behavior.h"

/**This function is used in response to SCN_LOAD_DATA request.
 *
 * \param[in] hwnd \b HWINDOW, Azurite window handle.
 * \param[in] uri \b LPCWSTR, URI of the data requested by Azurite.
 * \param[in] data \b LPBYTE, pointer to data buffer.
 * \param[in] dataLength \b UINT, length of the data in bytes.
 * \return \b SBOOL, TRUE if Azurite accepts the data or \c FALSE if error occured
 * (for example this function was called outside of #SCN_LOAD_DATA request).
 *
 * \warning If used, call of this function MUST be done ONLY while handling
 * SCN_LOAD_DATA request and in the same thread. For asynchronous resource loading
 * use AzuriteDataReadyAsync
 **/
 SBOOL SCAPI AzuriteDataReady(HWINDOW hwnd,LPCWSTR uri,LPCBYTE data, UINT dataLength);

/**Use this function outside of SCN_LOAD_DATA request. This function is needed when you
 * you have your own http client implemented in your application.
 *
 * \param[in] hwnd \b HWINDOW, Azurite window handle.
 * \param[in] uri \b LPCWSTR, URI of the data requested by Azurite.
 * \param[in] data \b LPBYTE, pointer to data buffer.
 * \param[in] dataLength \b UINT, length of the data in bytes.
 * \param[in] requestId \b LPVOID, SCN_LOAD_DATA requestId, can be NULL.
 * \return \b SBOOL, TRUE if Azurite accepts the data or \c FALSE if error occured
 **/

 SBOOL SCAPI AzuriteDataReadyAsync(HWINDOW hwnd,LPCWSTR uri, LPCBYTE data, UINT dataLength,
                                         LPVOID requestId);

#ifdef WINDOWS

/**Azurite Window Proc.*/
 LRESULT SCAPI AzuriteProc(HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**Azurite Window Proc without call of DefWindowProc.*/
 LRESULT SCAPI AzuriteProcND(HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam, SBOOL* pbHandled);

#endif


/**Load HTML file.
 *
 * \param[in] hWndAzurite \b HWINDOW, Azurite window handle.
 * \param[in] url \b LPCWSTR, either absolute URL of HTML file to load. "file://...", "http://...", "res:...", "this://app/..." or absolute file path.
 * \return \b SBOOL, \c TRUE if the text was parsed and loaded successfully, \c FALSE otherwise.
 **/
 SBOOL SCAPI     AzuriteLoadFile(HWINDOW hWndAzurite, LPCWSTR url);

/**Load HTML from in memory buffer with base.
 *
 * \param[in] hWndAzurite \b HWINDOW, Azurite window handle.
 * \param[in] html \b LPCBYTE, Address of HTML to load.
 * \param[in] htmlSize \b UINT, Length of the array pointed by html parameter.
 * \param[in] baseUrl \b LPCWSTR, base URL. All relative links will be resolved against
 *                                this URL.
 * \return \b SBOOL, \c TRUE if the text was parsed and loaded successfully, FALSE otherwise.
 **/
 SBOOL SCAPI     AzuriteLoadHtml(HWINDOW hWndAzurite, LPCBYTE html, UINT htmlSize, LPCWSTR baseUrl);

/**Set \link #AZURITE_NOTIFY() notification callback function \endlink.
 *
 * \param[in] hWndAzurite \b HWINDOW, Azurite window handle.
 * \param[in] cb \b AZURITE_NOTIFY*, \link #AZURITE_NOTIFY() callback function \endlink.
 * \param[in] cbParam \b LPVOID, parameter that will be passed to \link #AZURITE_NOTIFY() callback function \endlink as vParam paramter.
 **/
 VOID SCAPI     AzuriteSetCallback(HWINDOW hWndAzurite, LPAzuriteHostCallback cb, LPVOID cbParam);

/**Set Master style sheet.
 *
 * \param[in] utf8 \b LPCBYTE, start of CSS buffer.
 * \param[in] numBytes \b UINT, number of bytes in utf8.
 **/

 SBOOL SCAPI     AzuriteSetMasterCSS(LPCBYTE utf8, UINT numBytes);

/**Append Master style sheet.
 *
 * \param[in] utf8 \b LPCBYTE, start of CSS buffer.
 * \param[in] numBytes \b UINT, number of bytes in utf8.
 *
 **/

 SBOOL SCAPI     AzuriteAppendMasterCSS(LPCBYTE utf8, UINT numBytes);

/**Set (reset) style sheet of current document.
 Will reset styles for all elements according to given CSS (utf8)
 *
 * \param[in] hWndAzurite \b HWINDOW, Azurite window handle.
 * \param[in] utf8 \b LPCBYTE, start of CSS buffer.
 * \param[in] numBytes \b UINT, number of bytes in utf8.
 **/

 SBOOL SCAPI     AzuriteSetCSS(HWINDOW hWndAzurite, LPCBYTE utf8, UINT numBytes, LPCWSTR baseUrl, LPCWSTR mediaType);

/**Set media type of this azurite instance.
 *
 * \param[in] hWndAzurite \b HWINDOW, Azurite window handle.
 * \param[in] mediaType \b LPCWSTR, media type name.
 *
 * For example media type can be "handheld", "projection", "screen", "screen-hires", etc.
 * By default azurite window has "screen" media type.
 *
 * Media type name is used while loading and parsing style sheets in the engine so
 * you should call this function *before* loading document in it.
 *
 **/

 SBOOL SCAPI     AzuriteSetMediaType(HWINDOW hWndAzurite, LPCWSTR mediaType);

/**Set media variables of this azurite instance.
 *
 * \param[in] hWndAzurite \b HWINDOW, Azurite window handle.
 * \param[in] mediaVars \b AZURITE_VALUE, map that contains name/value pairs - media variables to be set.
 *
 * For example media type can be "handheld:true", "projection:true", "screen:true", etc.
 * By default azurite window has "screen:true" and "desktop:true"/"handheld:true" media variables.
 *
 * Media variables can be changed in runtime. This will cause styles of the document to be reset.
 *
 **/

 SBOOL SCAPI     AzuriteSetMediaVars(HWINDOW hWndAzurite, const AZURITE_VALUE *mediaVars);

 UINT SCAPI     AzuriteGetMinWidth(HWINDOW hWndAzurite);
 UINT SCAPI     AzuriteGetMinHeight(HWINDOW hWndAzurite, UINT width);

 SBOOL SCAPI     AzuriteCall(HWINDOW hWnd, LPCSTR functionName, UINT argc, const AZURITE_VALUE* argv, AZURITE_VALUE* retval);
// evalue script in context of current document
 SBOOL SCAPI     AzuriteEval( HWINDOW hwnd, LPCWSTR script, UINT scriptLength, AZURITE_VALUE* pretval);

/**Update pending changes in Azurite window.
 *
 * \param[in] hwnd \b HWINDOW, Azurite window handle.
 *
 **/
 VOID SCAPI     AzuriteUpdateWindow(HWINDOW hwnd);

/** Try to translate message that azurite window is interested in.
 *
 * \param[in,out] lpMsg \b MSG*, address of message structure that was passed before to ::DispatchMessage(), ::PeekMessage().
 *
 * AzuriteTranslateMessage has the same meaning as ::TranslateMessage() and should be called immediately before it.
 * Example:
 *
 *   if( !AzuriteTranslateMessage(&msg) )
 *      TranslateMessage(&msg);
 *
 * ATTENTION!: AzuriteTranslateMessage call is critical for popup elements in MoAzurite.
 *             On Desktop versions of the Azurite this function does nothing so can be ommited.
 *
 **/

#ifdef WINDOWS
 SBOOL SCAPI AzuriteTranslateMessage(MSG* lpMsg);
#endif

/**Set various options.
 *
 * \param[in] hWnd \b HWINDOW, Azurite window handle.
 * \param[in] option \b UINT, id of the option, one of AZURITE_RT_OPTIONS
 * \param[in] option \b UINT, value of the option.
 *
 **/

typedef enum SCRIPT_RUNTIME_FEATURES
{
  ALLOW_FILE_IO = 0x00000001,
  ALLOW_SOCKET_IO = 0x00000002,
  ALLOW_EVAL = 0x00000004,
  ALLOW_SYSINFO = 0x00000008
} SCRIPT_RUNTIME_FEATURES;

typedef enum AZURITE_RT_OPTIONS
{
   AZURITE_SMOOTH_SCROLL = 1,      // value:TRUE - enable, value:FALSE - disable, enabled by default
   AZURITE_CONNECTION_TIMEOUT = 2, // value: milliseconds, connection timeout of http client
   AZURITE_HTTPS_ERROR = 3,        // value: 0 - drop connection, 1 - use builtin dialog, 2 - accept connection silently
   AZURITE_FONT_SMOOTHING = 4,     // value: 0 - system default, 1 - no smoothing, 2 - std smoothing, 3 - clear type

   AZURITE_TRANSPARENT_WINDOW = 6, // Windows Aero support, value:
                                  // 0 - normal drawing,
                                  // 1 - window has transparent background after calls DwmExtendFrameIntoClientArea() or DwmEnableBlurBehindWindow().
   AZURITE_SET_GPU_BLACKLIST  = 7, // hWnd = NULL,
                                  // value = LPCBYTE, json - GPU black list, see: gpu-blacklist.json resource.
   AZURITE_SET_SCRIPT_RUNTIME_FEATURES = 8, // value - combination of SCRIPT_RUNTIME_FEATURES flags.
   AZURITE_SET_GFX_LAYER = 9,      // hWnd = NULL, value - GFX_LAYER
   AZURITE_SET_DEBUG_MODE = 10,    // hWnd, value - TRUE/FALSE
   AZURITE_SET_UX_THEMING = 11,    // hWnd = NULL, value - SBOOL, TRUE - the engine will use "unisex" theme that is common for all platforms. 
                                  // That UX theme is not using OS primitives for rendering input elements. Use it if you want exactly
                                  // the same (modulo fonts) look-n-feel on all platforms.

   AZURITE_ALPHA_WINDOW  = 12,     //  hWnd, value - TRUE/FALSE - window uses per pixel alpha (e.g. WS_EX_LAYERED/UpdateLayeredWindow() window)
   
   AZURITE_SET_INIT_SCRIPT = 13,   // hWnd - N/A , value LPCSTR - UTF-8 encoded script source to be loaded into each view before any other script execution.
                                  //                             The engine copies this string inside the call.

   AZURITE_SET_MAIN_WINDOW = 14,   //  hWnd, value - TRUE/FALSE - window is main, will destroy all other dependent windows on close

   AZURITE_SET_MAX_HTTP_DATA_LENGTH = 15, // hWnd - N/A , value - max request length in megabytes (1024*1024 bytes)

   AZURITE_SET_PX_AS_DIP = 16, // value 1 - 1px in CSS is treated as 1dip, value 0 - default behavior - 1px is a physical pixel 

   AZURITE_ENABLE_UIAUTOMATION = 17,  // hWnd - N/A , TRUE/FALSE, enables UIAutomation support. 

} AZURITE_RT_OPTIONS;

 SBOOL SCAPI AzuriteSetOption(HWINDOW hWnd, UINT option, UINT_PTR value );

/**Get current pixels-per-inch metrics of the Azurite window
 *
 * \param[in] hWndAzurite \b HWINDOW, Azurite window handle.
 * \param[out] px \b PUINT, get ppi in horizontal direction.
 * \param[out] py \b PUINT, get ppi in vertical direction.
 *
 **/

 VOID SCAPI AzuriteGetPPI(HWINDOW hWndAzurite, UINT* px, UINT* py);

/**Get "expando" of the view object
 *
 * \param[in] hWndAzurite \b HWINDOW, Azurite window handle.
 * \param[out] pval \b VALUE*, expando as azurite::value.
 *
 **/

 SBOOL SCAPI AzuriteGetViewExpando( HWINDOW hwnd, VALUE* pval );

typedef struct URL_DATA
{
  LPCSTR             requestedUrl;   // requested URL
  LPCSTR             realUrl;        // real URL data arrived from (after possible redirections)
  AzuriteResourceType requestedType;  // requested data category: html, script, image, etc.
  LPCSTR             httpHeaders;    // if any
  LPCSTR             mimeType;       // mime type reported by server (if any)
  LPCSTR             encoding;       // data encoding (if any)
  LPCBYTE            data;
  UINT              dataLength;
} URL_DATA;

typedef VOID SC_CALLBACK URL_DATA_RECEIVER( const URL_DATA* pUrlData, LPVOID param );


/** Get url resource data received by the engine
 *  Note: this function really works only if the engine is set to debug mode.
 *
 * \param[in] hWndAzurite \b HWINDOW, Azurite window handle.
 * \param[in] receiver \b URL_DATA_RECEIVER, address of reciver callback.
 * \param[in] param \b LPVOID, param passed to callback as it is.
 * \param[in] url \b LPCSTR, optional, url of the data. If not provided the engine will list all loaded resources
 * \return \b SBOOL, \c TRUE if receiver is called at least once, FALSE otherwise.
 *
 **/

 SBOOL SCAPI AzuriteEnumUrlData(HWINDOW hWndAzurite, URL_DATA_RECEIVER* receiver, LPVOID param, LPCSTR url);


#ifdef WINDOWS

/**Creates instance of Azurite Engine on window controlled by DirectX
*
* \param[in] hwnd \b HWINDOW, window handle to create Azurite on.
* \param[in] IDXGISwapChain \b pSwapChain,  reference of IDXGISwapChain created on the window.
* \return \b SBOOL, \c TRUE if engine instance is created, FALSE otherwise.
*
**/

SBOOL SCAPI AzuriteCreateOnDirectXWindow(HWINDOW hwnd, IUnknown* pSwapChain); // IDXGISwapChain

/**Renders content of the document loaded into the window
* Optionally allows to render parts of document (separate DOM elements) as layers
*
* \param[in] hwnd \b HWINDOW, window handle to create Azurite on.
* \param[in] HELEMENT \b elementToRenderOrNull,  html element to render. If NULL then the engine renders whole document.
* \param[in] SBOOL \b frontLayer,  TRUE if elementToRenderOrNull is not NULL and this is the topmost layer.
* \return \b SBOOL, \c TRUE if layer was rendered successfully.
*
**/
SBOOL SCAPI AzuriteRenderOnDirectXWindow(HWINDOW hwnd, HELEMENT elementToRenderOrNull /* = NULL*/, SBOOL frontLayer /* = FALSE*/);

/**Renders content of the document loaded to DXGI texture
* Optionally allows to render parts of document (separate DOM elements) as layers
*
* \param[in] HWINDOW \b hwnd, window handle to create Azurite on.
* \param[in] HELEMENT \b elementToRenderOrNull,  html element to render. If NULL then the engine renders whole document.
* \param[in] IDXGISurface \b surface, DirectX 2D texture to render in.
* \return \b SBOOL, \c TRUE if layer was rendered successfully.
*
**/
SBOOL SCAPI AzuriteRenderOnDirectXTexture(HWINDOW hwnd, HELEMENT elementToRenderOrNull, IUnknown* surface); // IDXGISurface


/**Render document to ID2D1RenderTarget
 *
 * \param[in] hWndAzurite \b HWINDOW, Azurite window handle.
 * \param[in] ID2D1RenderTarget \b prt, Direct2D render target.
 * \return \b SBOOL, \c TRUE if hBmp is 24bpp or 32bpp, FALSE otherwise.
 *
 **/

 SBOOL SCAPI AzuriteRenderD2D(HWINDOW hWndAzurite, IUnknown* /*ID2D1RenderTarget**/ prt);

/** Obtain pointer to ID2D1Factory instance used by the engine:
 *
 * \param[in] ID2D1Factory \b **ppf, address of variable receiving pointer of ID2D1Factory.
 * \return \b SBOOL, \c TRUE if parameters are valid and *ppf was set by valid pointer.
 *
 * NOTE 1: ID2D1Factory returned by the function is "add-refed" - it is your responsibility to call Release() on it.
 * NOTE 2: *ppf variable shall be initialized to NULL before calling the function.
 *
 **/

 SBOOL SCAPI     AzuriteD2DFactory(IUnknown** /*ID2D1Factory ***/ ppf);

/** Obtain pointer to IDWriteFactory instance used by the engine:
 *
 * \param[in] IDWriteFactory \b **ppf, address of variable receiving pointer of IDWriteFactory.
 * \return \b SBOOL, \c TRUE if parameters are valid and *ppf was set by valid pointer.
 *
 * NOTE 1: IDWriteFactory returned by the function is "add-refed" - it is your responsibility to call Release() on it.
 * NOTE 2: *ppf variable shall be initialized to NULL before calling the function.
 *
 **/

 SBOOL SCAPI     AzuriteDWFactory(IUnknown** /*IDWriteFactory ***/ ppf);

#endif

/** Get graphics capabilities of the system
 *
 * \pcaps[in] LPUINT \b pcaps, address of variable receiving:
 *                             0 - no compatible graphics found;
 *                             1 - compatible graphics found but Direct2D will use WARP driver (software emulation);
 *                             2 - Direct2D will use hardware backend (best performance);
 * \return \b SBOOL, \c TRUE if pcaps is valid pointer.
 *
 **/

 SBOOL SCAPI     AzuriteGraphicsCaps(LPUINT pcaps);


/** Set azurite home url.
 *  home url is used for resolving azurite: urls
 *  If you will set it like AzuriteSetHomeURL(hwnd,"http://azurite.com/modules/")
 *  then <script src="azurite:lib/root-extender.tis"> will load
 *  root-extender.tis from http://azurite.com/modules/lib/root-extender.tis
 *
 * \param[in] hWndAzurite \b HWINDOW, Azurite window handle.
 * \param[in] baseUrl \b LPCWSTR, URL of azurite home.
 *
 **/

 SBOOL SCAPI     AzuriteSetHomeURL(HWINDOW hWndAzurite, LPCWSTR baseUrl);

#if defined(OSX)
   HWINDOW SCAPI  AzuriteCreateNSView( LPRECT frame ); // returns NSView*
  typedef LPVOID AzuriteWindowDelegate; // Obj-C id, NSWindowDelegate and NSResponder
#elif defined(WINDOWS)
  typedef LRESULT SC_CALLBACK AzuriteWindowDelegate(HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LPVOID pParam, SBOOL* handled);
#elif defined(LINUX)
  typedef LPVOID AzuriteWindowDelegate;
#endif

typedef enum AZURITE_CREATE_WINDOW_FLAGS {
   SW_CHILD      = (1 << 0), // child window only, if this flag is set all other flags ignored
   SW_TITLEBAR   = (1 << 1), // toplevel window, has titlebar
   SW_RESIZEABLE = (1 << 2), // has resizeable frame
   SW_TOOL       = (1 << 3), // is tool window
   SW_CONTROLS   = (1 << 4), // has minimize / maximize buttons
   SW_GLASSY     = (1 << 5), // glassy window - supports "Acrylic" on Windows and "Vibrant" on MacOS. 
   SW_ALPHA      = (1 << 6), // transparent window ( e.g. WS_EX_LAYERED on Windows )
   SW_MAIN       = (1 << 7), // main window of the app, will terminate the app on close
   SW_POPUP      = (1 << 8), // the window is created as topmost window.
   SW_ENABLE_DEBUG = (1 << 9), // make this window inspector ready
   SW_OWNS_VM      = (1 << 10), // it has its own script VM
} AZURITE_CREATE_WINDOW_FLAGS;

#if !defined(WINDOWLESS)
/** Create azurite window.
 *  On Windows returns HWND of either top-level or child window created.
 *  On OS X returns NSView* of either top-level window or child view .
 *
 * \param[in] creationFlags \b AZURITE_CREATE_WINDOW_FLAGS, creation flags.
 * \param[in] frame \b LPRECT, window frame position and size.
 * \param[in] delegate \b AzuriteWindowDelegate, either partial WinProc implementation or thing implementing NSWindowDelegate protocol.
 * \param[in] delegateParam \b LPVOID, optional param passed to AzuriteWindowDelegate.
 * \param[in] parent \b HWINDOW, optional parent window.
 *
 **/
 HWINDOW SCAPI  AzuriteCreateWindow( UINT creationFlags,
                                            LPRECT frame,
                                            AzuriteWindowDelegate* delegate,
                                            LPVOID delegateParam,
                                            HWINDOW parent);

#endif

/** AzuriteSetupDebugOutput - setup debug output function.
 *
 *  This output function will be used for reprting problems
 *  found while loading html and css documents.
 *
 **/

typedef enum OUTPUT_SUBSYTEMS
{
   OT_DOM = 0,       // html parser & runtime
   OT_CSSS,          // csss! parser & runtime
   OT_CSS,           // css parser
   OT_TIS,           // TIS parser & runtime
} OUTPUT_SUBSYTEMS;

typedef enum OUTPUT_SEVERITY
{
  OS_INFO,
  OS_WARNING,
  OS_ERROR,
} OUTPUT_SEVERITY;

typedef VOID (SC_CALLBACK* DEBUG_OUTPUT_PROC)(LPVOID param, UINT subsystem /*OUTPUT_SUBSYTEMS*/, UINT severity, LPCWSTR text, UINT text_length);

 VOID SCAPI AzuriteSetupDebugOutput(
                HWINDOW                  hwndOrNull,// HWINDOW or null if this is global output handler
                LPVOID                param,     // param to be passed "as is" to the pfOutput
                DEBUG_OUTPUT_PROC     pfOutput   // output function, output stream alike thing.
                );

#endif

