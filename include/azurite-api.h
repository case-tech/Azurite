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

#ifndef __AZURITE_API__
#define __AZURITE_API__

#include "azurite-types.h"
#include "azurite-def.h"
#include "azurite-dom.h"
#include "azurite-request.h"
#include "azurite-msg.h"
#include "value.h"

#if !defined(WINDOWS)
  #include <stdlib.h>
  #include <unistd.h>
  #if defined(OSX)
    #include <libproc.h>
  #endif
  #if defined(ANDROID)
    #include <dlfcn.h>
  #endif
#endif

#if defined(OSX)
  #include <dlfcn.h>
#endif

#ifdef __cplusplus
  #include <cstddef>
#endif

struct AzuriteGraphicsAPI;
struct AZURITE_X_MSG;

#ifdef WINDOWLESS
  #define AZURITE_API_VERSION 0x10009
#else 
  #define AZURITE_API_VERSION 9
#endif

typedef struct _IAzuriteAPI {

  UINT    version; // API_VERSION

  LPCWSTR SCFN( AzuriteClassName )(void);
  UINT    SCFN( AzuriteVersion )(SBOOL major);
  SBOOL    SCFN( AzuriteDataReady )(HWINDOW hwnd,LPCWSTR uri,LPCBYTE data, UINT dataLength);
  SBOOL    SCFN( AzuriteDataReadyAsync )(HWINDOW hwnd,LPCWSTR uri, LPCBYTE data, UINT dataLength, LPVOID requestId);
#if defined(WINDOWS)
  LRESULT SCFN( AzuriteProc )(HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
  LRESULT SCFN( AzuriteProcND )(HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam, SBOOL* pbHandled);
#else 
  LPVOID   AzuriteProc; // NULL
  LPVOID   AzuriteProcND; // NULL
#endif
  SBOOL    SCFN( AzuriteLoadFile )(HWINDOW hWndAzurite, LPCWSTR filename);

  SBOOL    SCFN( AzuriteLoadHtml )(HWINDOW hWndAzurite, LPCBYTE html, UINT htmlSize, LPCWSTR baseUrl);
  VOID     SCFN( AzuriteSetCallback )(HWINDOW hWndAzurite, LPAzuriteHostCallback cb, LPVOID cbParam);
  SBOOL    SCFN( AzuriteSetMasterCSS )(LPCBYTE utf8, UINT numBytes);
  SBOOL    SCFN( AzuriteAppendMasterCSS )(LPCBYTE utf8, UINT numBytes);
  SBOOL    SCFN( AzuriteSetCSS )(HWINDOW hWndAzurite, LPCBYTE utf8, UINT numBytes, LPCWSTR baseUrl, LPCWSTR mediaType);
  SBOOL    SCFN( AzuriteSetMediaType )(HWINDOW hWndAzurite, LPCWSTR mediaType);
  SBOOL    SCFN( AzuriteSetMediaVars )(HWINDOW hWndAzurite, const AZURITE_VALUE *mediaVars);
  UINT     SCFN( AzuriteGetMinWidth )(HWINDOW hWndAzurite);
  UINT     SCFN( AzuriteGetMinHeight )(HWINDOW hWndAzurite, UINT width);
  SBOOL    SCFN( AzuriteCall )(HWINDOW hWnd, LPCSTR functionName, UINT argc, const AZURITE_VALUE* argv, AZURITE_VALUE* retval);
  SBOOL    SCFN( AzuriteEval )( HWINDOW hwnd, LPCWSTR script, UINT scriptLength, AZURITE_VALUE* pretval);
  VOID     SCFN( AzuriteUpdateWindow)(HWINDOW hwnd);
#if defined(WINDOWS)
  SBOOL    SCFN(AzuriteTranslateMessage)(MSG* lpMsg);
#else 
  LPVOID   AzuriteTranslateMessage; // NULL
#endif
  SBOOL    SCFN( AzuriteSetOption )(HWINDOW hWnd, UINT option, UINT_PTR value );
  VOID     SCFN( AzuriteGetPPI )(HWINDOW hWndAzurite, UINT* px, UINT* py);
  SBOOL    SCFN( AzuriteGetViewExpando )( HWINDOW hwnd, VALUE* pval );
#if defined(WINDOWS)
  SBOOL    SCFN( AzuriteRenderD2D )(HWINDOW hWndAzurite, IUnknown* /*ID2D1RenderTarget**/ prt);
  SBOOL    SCFN( AzuriteD2DFactory )(IUnknown** /*ID2D1Factory ***/ ppf);
  SBOOL    SCFN( AzuriteDWFactory )(IUnknown** /*IDWriteFactory ***/ ppf);
#else 
  LPVOID   AzuriteRenderD2D;
  LPVOID   AzuriteD2DFactory;
  LPVOID   AzuriteDWFactory;
#endif
  SBOOL    SCFN( AzuriteGraphicsCaps )(LPUINT pcaps);
  SBOOL    SCFN( AzuriteSetHomeURL )(HWINDOW hWndAzurite, LPCWSTR baseUrl);
#if defined(OSX)
  HWINDOW SCFN( AzuriteCreateNSView )( LPRECT frame ); // returns NSView*
#else 
  LPVOID AzuriteCreateNSView; // NULL
#endif
#if defined(LINUX)
  HWINDOW SCFN( AzuriteCreateWidget )( LPRECT frame ); // returns GtkWidget
#else 
  LPVOID AzuriteCreateWidget; // NULL
#endif
  HWINDOW SCFN( AzuriteCreateWindow )( UINT creationFlags,LPRECT frame, AzuriteWindowDelegate* delegate, LPVOID delegateParam, HWINDOW parent);

  VOID    SCFN( AzuriteSetupDebugOutput )(
                HWINDOW               hwndOrNull,// HWINDOW or null if this is global output handler
                LPVOID                param,     // param to be passed "as is" to the pfOutput
                DEBUG_OUTPUT_PROC     pfOutput   // output function, output stream alike thing.
                );
//|
//| DOM Element API
//|
  SCDOM_RESULT SCFN( Azurite_UseElement)(HELEMENT he);
  SCDOM_RESULT SCFN( Azurite_UnuseElement)(HELEMENT he);
  SCDOM_RESULT SCFN( AzuriteGetRootElement)(HWINDOW hwnd, HELEMENT *phe);
  SCDOM_RESULT SCFN( AzuriteGetFocusElement)(HWINDOW hwnd, HELEMENT *phe);
  SCDOM_RESULT SCFN( AzuriteFindElement)(HWINDOW hwnd, POINT pt, HELEMENT* phe);
  SCDOM_RESULT SCFN( AzuriteGetChildrenCount)(HELEMENT he, UINT* count);
  SCDOM_RESULT SCFN( AzuriteGetNthChild)(HELEMENT he, UINT n, HELEMENT* phe);
  SCDOM_RESULT SCFN( AzuriteGetParentElement)(HELEMENT he, HELEMENT* p_parent_he);
  SCDOM_RESULT SCFN( AzuriteGetElementHtmlCB)(HELEMENT he, SBOOL outer, LPCBYTE_RECEIVER* rcv, LPVOID rcv_param);
  SCDOM_RESULT SCFN( AzuriteGetElementTextCB)(HELEMENT he, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param);
  SCDOM_RESULT SCFN( AzuriteSetElementText)(HELEMENT he, LPCWSTR utf16, UINT length);
  SCDOM_RESULT SCFN( AzuriteGetAttributeCount)(HELEMENT he, LPUINT p_count);
  SCDOM_RESULT SCFN( AzuriteGetNthAttributeNameCB)(HELEMENT he, UINT n, LPCSTR_RECEIVER* rcv, LPVOID rcv_param);
  SCDOM_RESULT SCFN( AzuriteGetNthAttributeValueCB)(HELEMENT he, UINT n, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param);
  SCDOM_RESULT SCFN( AzuriteGetAttributeByNameCB)(HELEMENT he, LPCSTR name, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param);
  SCDOM_RESULT SCFN( AzuriteSetAttributeByName)(HELEMENT he, LPCSTR name, LPCWSTR value);
  SCDOM_RESULT SCFN( AzuriteClearAttributes)(HELEMENT he);
  SCDOM_RESULT SCFN( AzuriteGetElementIndex)(HELEMENT he, LPUINT p_index);
  SCDOM_RESULT SCFN( AzuriteGetElementType)(HELEMENT he, LPCSTR* p_type);
  SCDOM_RESULT SCFN( AzuriteGetElementTypeCB)(HELEMENT he, LPCSTR_RECEIVER* rcv, LPVOID rcv_param);
  SCDOM_RESULT SCFN( AzuriteGetStyleAttributeCB)(HELEMENT he, LPCSTR name, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param);
  SCDOM_RESULT SCFN( AzuriteSetStyleAttribute)(HELEMENT he, LPCSTR name, LPCWSTR value);
  SCDOM_RESULT SCFN( AzuriteGetElementLocation)(HELEMENT he, LPRECT p_location, UINT areas /*ELEMENT_AREAS*/);
  SCDOM_RESULT SCFN( AzuriteScrollToView)(HELEMENT he, UINT AzuriteScrollFlags);
  SCDOM_RESULT SCFN( AzuriteUpdateElement)(HELEMENT he, SBOOL andForceRender);
  SCDOM_RESULT SCFN( AzuriteRefreshElementArea)(HELEMENT he, RECT rc);
  SCDOM_RESULT SCFN( AzuriteSetCapture)(HELEMENT he);
  SCDOM_RESULT SCFN( AzuriteReleaseCapture)(HELEMENT he);
  SCDOM_RESULT SCFN( AzuriteGetElementHwnd)(HELEMENT he, HWINDOW* p_hwnd, SBOOL rootWindow);
  SCDOM_RESULT SCFN( AzuriteCombineURL)(HELEMENT he, LPWSTR szUrlBuffer, UINT UrlBufferSize);
  SCDOM_RESULT SCFN( AzuriteSelectElements)(HELEMENT  he, LPCSTR    CSS_selectors, AzuriteElementCallback* callback, LPVOID param);
  SCDOM_RESULT SCFN( AzuriteSelectElementsW)(HELEMENT  he, LPCWSTR   CSS_selectors, AzuriteElementCallback* callback, LPVOID param);
  SCDOM_RESULT SCFN( AzuriteSelectParent)(HELEMENT  he, LPCSTR    selector, UINT      depth, HELEMENT* heFound);
  SCDOM_RESULT SCFN( AzuriteSelectParentW)(HELEMENT  he, LPCWSTR   selector, UINT      depth, HELEMENT* heFound);
  SCDOM_RESULT SCFN( AzuriteSetElementHtml)(HELEMENT he, const BYTE* html, UINT htmlLength, UINT where);
  SCDOM_RESULT SCFN( AzuriteGetElementUID)(HELEMENT he, UINT* puid);
  SCDOM_RESULT SCFN( AzuriteGetElementByUID)(HWINDOW hwnd, UINT uid, HELEMENT* phe);
  SCDOM_RESULT SCFN( AzuriteShowPopup)(HELEMENT hePopup, HELEMENT heAnchor, UINT placement);
  SCDOM_RESULT SCFN( AzuriteShowPopupAt)(HELEMENT hePopup, POINT pos, UINT placement);
  SCDOM_RESULT SCFN( AzuriteHidePopup)(HELEMENT he);
  SCDOM_RESULT SCFN( AzuriteGetElementState)( HELEMENT he, UINT* pstateBits);
  SCDOM_RESULT SCFN( AzuriteSetElementState)( HELEMENT he, UINT stateBitsToSet, UINT stateBitsToClear, SBOOL updateView);
  SCDOM_RESULT SCFN( AzuriteCreateElement)( LPCSTR tagname, LPCWSTR textOrNull, /*out*/ HELEMENT *phe );
  SCDOM_RESULT SCFN( AzuriteCloneElement)( HELEMENT he, /*out*/ HELEMENT *phe );
  SCDOM_RESULT SCFN( AzuriteInsertElement)( HELEMENT he, HELEMENT hparent, UINT index );
  SCDOM_RESULT SCFN( AzuriteDetachElement)( HELEMENT he );
  SCDOM_RESULT SCFN( AzuriteDeleteElement)(HELEMENT he);
  SCDOM_RESULT SCFN( AzuriteSetTimer)( HELEMENT he, UINT milliseconds, UINT_PTR timer_id );
  SCDOM_RESULT SCFN( AzuriteDetachEventHandler)( HELEMENT he, LPELEMENT_EVENT_PROC pep, LPVOID tag );
  SCDOM_RESULT SCFN( AzuriteAttachEventHandler)( HELEMENT he, LPELEMENT_EVENT_PROC pep, LPVOID tag );
  SCDOM_RESULT SCFN( AzuriteWindowAttachEventHandler)( HWINDOW hwndLayout, LPELEMENT_EVENT_PROC pep, LPVOID tag, UINT subscription );
  SCDOM_RESULT SCFN( AzuriteWindowDetachEventHandler)( HWINDOW hwndLayout, LPELEMENT_EVENT_PROC pep, LPVOID tag );
  SCDOM_RESULT SCFN( AzuriteSendEvent)( HELEMENT he, UINT appEventCode, HELEMENT heSource, UINT_PTR reason, /*out*/ SBOOL* handled);
  SCDOM_RESULT SCFN( AzuritePostEvent)( HELEMENT he, UINT appEventCode, HELEMENT heSource, UINT_PTR reason);
  SCDOM_RESULT SCFN( AzuriteCallBehaviorMethod)(HELEMENT he, struct METHOD_PARAMS* params);
  SCDOM_RESULT SCFN( AzuriteRequestElementData)( HELEMENT he, LPCWSTR url, UINT dataType, HELEMENT initiator );
  SCDOM_RESULT SCFN( AzuriteHttpRequest)( HELEMENT        he,           // element to deliver data
          LPCWSTR         url,          // url
          UINT            dataType,     // data type, see AzuriteResourceType.
          UINT            requestType,  // one of REQUEST_TYPE values
          struct REQUEST_PARAM*  requestParams,// parameters
          UINT            nParams       // number of parameters
          );
  SCDOM_RESULT SCFN( AzuriteGetScrollInfo)( HELEMENT he, LPPOINT scrollPos, LPRECT viewRect, LPSIZE contentSize );
  SCDOM_RESULT SCFN( AzuriteSetScrollPos)( HELEMENT he, POINT scrollPos, SBOOL smooth );
  SCDOM_RESULT SCFN( AzuriteGetElementIntrinsicWidths)( HELEMENT he, INT* pMinWidth, INT* pMaxWidth );
  SCDOM_RESULT SCFN( AzuriteGetElementIntrinsicHeight)( HELEMENT he, INT forWidth, INT* pHeight );
  SCDOM_RESULT SCFN( AzuriteIsElementVisible)( HELEMENT he, SBOOL* pVisible);
  SCDOM_RESULT SCFN( AzuriteIsElementEnabled)( HELEMENT he, SBOOL* pEnabled );
  SCDOM_RESULT SCFN( AzuriteSortElements)( HELEMENT he, UINT firstIndex, UINT lastIndex, ELEMENT_COMPARATOR* cmpFunc, LPVOID cmpFuncParam );
  SCDOM_RESULT SCFN( AzuriteSwapElements)( HELEMENT he1, HELEMENT he2 );
  SCDOM_RESULT SCFN( AzuriteTraverseUIEvent)( UINT evt, LPVOID eventCtlStruct, SBOOL* bOutProcessed );
  SCDOM_RESULT SCFN( AzuriteCallScriptingMethod)( HELEMENT he, LPCSTR name, const VALUE* argv, UINT argc, VALUE* retval );
  SCDOM_RESULT SCFN( AzuriteCallScriptingFunction)( HELEMENT he, LPCSTR name, const VALUE* argv, UINT argc, VALUE* retval );
  SCDOM_RESULT SCFN( AzuriteEvalElementScript)( HELEMENT he, LPCWSTR script, UINT scriptLength, VALUE* retval );
  SCDOM_RESULT SCFN( AzuriteAttachHwndToElement)(HELEMENT he, HWINDOW hwnd);
  SCDOM_RESULT SCFN( AzuriteControlGetType)( HELEMENT he, /*CTL_TYPE*/ UINT *pType );
  SCDOM_RESULT SCFN( AzuriteGetValue)( HELEMENT he, VALUE* pval );
  SCDOM_RESULT SCFN( AzuriteSetValue)( HELEMENT he, const VALUE* pval );
  SCDOM_RESULT SCFN( AzuriteGetExpando)( HELEMENT he, VALUE* pval, SBOOL forceCreation );
  SCDOM_RESULT SCFN( AzuriteGetObject)( HELEMENT he, void* pval, SBOOL forceCreation );
  SCDOM_RESULT SCFN( AzuriteGetElementNamespace)(  HELEMENT he, void* pval);
  SCDOM_RESULT SCFN( AzuriteGetHighlightedElement)(HWINDOW hwnd, HELEMENT* phe);
  SCDOM_RESULT SCFN( AzuriteSetHighlightedElement)(HWINDOW hwnd, HELEMENT he);
//|
//| DOM Node API
//|
  SCDOM_RESULT SCFN( AzuriteNodeAddRef)(HNODE hn);
  SCDOM_RESULT SCFN( AzuriteNodeRelease)(HNODE hn);
  SCDOM_RESULT SCFN( AzuriteNodeCastFromElement)(HELEMENT he, HNODE* phn);
  SCDOM_RESULT SCFN( AzuriteNodeCastToElement)(HNODE hn, HELEMENT* he);
  SCDOM_RESULT SCFN( AzuriteNodeFirstChild)(HNODE hn, HNODE* phn);
  SCDOM_RESULT SCFN( AzuriteNodeLastChild)(HNODE hn, HNODE* phn);
  SCDOM_RESULT SCFN( AzuriteNodeNextSibling)(HNODE hn, HNODE* phn);
  SCDOM_RESULT SCFN( AzuriteNodePrevSibling)(HNODE hn, HNODE* phn);
  SCDOM_RESULT SCFN( AzuriteNodeParent)(HNODE hnode, HELEMENT* pheParent);
  SCDOM_RESULT SCFN( AzuriteNodeNthChild)(HNODE hnode, UINT n, HNODE* phn);
  SCDOM_RESULT SCFN( AzuriteNodeChildrenCount)(HNODE hnode, UINT* pn);
  SCDOM_RESULT SCFN( AzuriteNodeType)(HNODE hnode, UINT* pNodeType /*NODE_TYPE*/);
  SCDOM_RESULT SCFN( AzuriteNodeGetText)(HNODE hnode, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param);
  SCDOM_RESULT SCFN( AzuriteNodeSetText)(HNODE hnode, LPCWSTR text, UINT textLength);
  SCDOM_RESULT SCFN( AzuriteNodeInsert)(HNODE hnode, UINT where /*NODE_INS_TARGET*/, HNODE what);
  SCDOM_RESULT SCFN( AzuriteNodeRemove)(HNODE hnode, SBOOL finalize);
  SCDOM_RESULT SCFN( AzuriteCreateTextNode)(LPCWSTR text, UINT textLength, HNODE* phnode);
  SCDOM_RESULT SCFN( AzuriteCreateCommentNode)(LPCWSTR text, UINT textLength, HNODE* phnode);
//|
//| Value API
//|
  UINT SCFN( ValueInit )( VALUE* pval );
  UINT SCFN( ValueClear )( VALUE* pval );
  UINT SCFN( ValueCompare )( const VALUE* pval1, const VALUE* pval2 );
  UINT SCFN( ValueCopy )( VALUE* pdst, const VALUE* psrc );
  UINT SCFN( ValueIsolate )( VALUE* pdst );
  UINT SCFN( ValueType )( const VALUE* pval, UINT* pType, UINT* pUnits );
  UINT SCFN( ValueStringData )( const VALUE* pval, LPCWSTR* pChars, UINT* pNumChars );
  UINT SCFN( ValueStringDataSet )( VALUE* pval, LPCWSTR chars, UINT numChars, UINT units );
  UINT SCFN( ValueIntData )( const VALUE* pval, INT* pData );
  UINT SCFN( ValueIntDataSet )( VALUE* pval, INT data, UINT type, UINT units );
  UINT SCFN( ValueInt64Data )( const VALUE* pval, INT64* pData );
  UINT SCFN( ValueInt64DataSet )( VALUE* pval, INT64 data, UINT type, UINT units );
  UINT SCFN( ValueFloatData )( const VALUE* pval, FLOAT_VALUE* pData );
  UINT SCFN( ValueFloatDataSet )( VALUE* pval, FLOAT_VALUE data, UINT type, UINT units );
  UINT SCFN( ValueBinaryData )( const VALUE* pval, LPCBYTE* pBytes, UINT* pnBytes );
  UINT SCFN( ValueBinaryDataSet )( VALUE* pval, LPCBYTE pBytes, UINT nBytes, UINT type, UINT units );
  UINT SCFN( ValueElementsCount )( const VALUE* pval, INT* pn);
  UINT SCFN( ValueNthElementValue )( const VALUE* pval, INT n, VALUE* pretval);
  UINT SCFN( ValueNthElementValueSet )( VALUE* pval, INT n, const VALUE* pval_to_set);
  UINT SCFN( ValueNthElementKey )( const VALUE* pval, INT n, VALUE* pretval);
  UINT SCFN( ValueEnumElements )( const VALUE* pval, KeyValueCallback* penum, LPVOID param);
  UINT SCFN( ValueSetValueToKey )( VALUE* pval, const VALUE* pkey, const VALUE* pval_to_set);
  UINT SCFN( ValueGetValueOfKey )( const VALUE* pval, const VALUE* pkey, VALUE* pretval);
  UINT SCFN( ValueToString )( VALUE* pval, /*VALUE_STRING_CVT_TYPE*/ UINT how );
  UINT SCFN( ValueFromString )( VALUE* pval, LPCWSTR str, UINT strLength, /*VALUE_STRING_CVT_TYPE*/ UINT how );
  UINT SCFN( ValueInvoke )( const VALUE* pval, VALUE* pthis, UINT argc, const VALUE* argv, VALUE* pretval, LPCWSTR url);
  UINT SCFN( ValueNativeFunctorSet )( VALUE* pval, NATIVE_FUNCTOR_INVOKE*  pinvoke, NATIVE_FUNCTOR_RELEASE* prelease, VOID* tag );
  SBOOL SCFN( ValueIsNativeFunctor )( const VALUE* pval);
  // used to be script VM API
  LPVOID reserved1;
  LPVOID reserved2;
  LPVOID reserved3;
  LPVOID reserved4;

  HSARCHIVE SCFN( AzuriteOpenArchive ) (LPCBYTE archiveData, UINT archiveDataLength);
  SBOOL SCFN( AzuriteGetArchiveItem ) (HSARCHIVE harc, LPCWSTR path, LPCBYTE* pdata, UINT* pdataLength);
  SBOOL SCFN( AzuriteCloseArchive ) (HSARCHIVE harc);

  SCDOM_RESULT SCFN( AzuriteFireEvent)( const BEHAVIOR_EVENT_PARAMS* evt, SBOOL post, SBOOL *handled );

  LPVOID SCFN( AzuriteGetCallbackParam )(HWINDOW hwnd);
  UINT_PTR SCFN( AzuritePostCallback )(HWINDOW hwnd, UINT_PTR wparam, UINT_PTR lparam, UINT timeoutms);

  LPAzuriteGraphicsAPI SCFN( GetAzuriteGraphicsAPI )();
  LPAzuriteRequestAPI SCFN( GetAzuriteRequestAPI )();

#if defined(WINDOWS)
    SBOOL SCFN( AzuriteCreateOnDirectXWindow ) (HWINDOW hwnd, IUnknown* pSwapChain); // IDXGISwapChain
    SBOOL SCFN( AzuriteRenderOnDirectXWindow ) (HWINDOW hwnd, HELEMENT elementToRenderOrNull, SBOOL frontLayer);
    SBOOL SCFN( AzuriteRenderOnDirectXTexture ) (HWINDOW hwnd, HELEMENT elementToRenderOrNull, IUnknown* surface); // IDXGISurface
#else 
    LPVOID AzuriteCreateOnDirectXWindow;
    LPVOID AzuriteRenderOnDirectXWindow;
    LPVOID AzuriteRenderOnDirectXTexture;
#endif

  SBOOL SCFN(AzuriteProcX)(HWINDOW hwnd, AZURITE_X_MSG* pMsg ); // returns TRUE if handled

  UINT64 SCFN(AzuriteAtomValue)(const char* name); //
  SBOOL   SCFN(AzuriteAtomNameCB)(UINT64 atomv, LPCSTR_RECEIVER* rcv, LPVOID rcv_param);
  SBOOL   SCFN(AzuriteSetGlobalAsset)(som_asset_t* pass);

  SCDOM_RESULT SCFN(AzuriteGetElementAsset)(HELEMENT el, UINT64 nameAtom, som_asset_t** ppass);

  UINT   SCFN(AzuriteSetVariable)(HWINDOW hwndOrNull, LPCWSTR path, const VALUE* pvalToSet);
  UINT   SCFN(AzuriteGetVariable)(HWINDOW hwndOrNull, LPCWSTR path, VALUE* pvalToGet);

  UINT   SCFN(AzuriteElementUnwrap)(const VALUE* pval, HELEMENT* ppElement);
  UINT   SCFN(AzuriteElementWrap)(VALUE* pval, HELEMENT pElement);

  UINT   SCFN(AzuriteNodeUnwrap)(const VALUE* pval, HNODE* ppNode);
  UINT   SCFN(AzuriteNodeWrap)(VALUE* pval, HNODE pNode);

  SBOOL   SCFN(AzuriteReleaseGlobalAsset)(som_asset_t* pass);

} IAzuriteAPI;

typedef IAzuriteAPI* (SCAPI *AzuriteAPI_ptr)();

// getting IAzuriteAPI reference:

#if defined(STATIC_LIB) || defined(AZURITE_BUILD)

    EXTERN_C IAzuriteAPI* SCAPI AzuriteAPI();

    inline IAzuriteAPI* _SAPI(IAzuriteAPI* ext) {
       static IAzuriteAPI* _api = NULL;
       if( ext ) _api = ext;
       if( !_api )
       {
          _api = AzuriteAPI();
#if defined(__cplusplus) && !defined(PLAIN_API_ONLY)
//          tiscript::ni(_api->TIScriptAPI());
#endif
       }
       assert(_api);
       return _api;
    }

#elif defined(WINDOWS)

    inline IAzuriteAPI* _SAPI( IAzuriteAPI* ext ) {
       static IAzuriteAPI* _api = NULL;
       char gxsgdg[] = { 83, 99, 105, 116, 101, 114, 65, 80, 73, 0 };
       if( ext ) _api = ext;
       if( !_api )
       {
          HMODULE hm = LoadLibrary( TEXT("azurite.dll") );
          if(hm) {
            AzuriteAPI_ptr azuriteAPI = (AzuriteAPI_ptr) GetProcAddress(hm, gxsgdg);
            if( azuriteAPI ) {
              _api = azuriteAPI();
//#if defined(__cplusplus) && !defined(PLAIN_API_ONLY)
//              tiscript::ni( _api->TIScriptAPI() );
//#endif
            } else {
              FreeLibrary(hm);
            }

          }
       }
       assert(_api);
       if( !_api ) {
         //::MessageBox(NULL, TEXT("Azurite engine not found, quiting"),TEXT("Error"),MB_OK);
         exit(-1);
       }
       return _api;
    }

#elif defined(OSX)

    //typedef IAzuriteAPI* SCAPI (*AzuriteAPI_ptr)();

    inline IAzuriteAPI* _SAPI( IAzuriteAPI* ext )
    {
        static IAzuriteAPI* _api = NULL;
        if( ext ) _api = ext;
        if( !_api ) {
            pid_t pid;
            char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
            char folderpath[PROC_PIDPATHINFO_MAXSIZE];
            pid = getpid();
            proc_pidpath (pid, pathbuf, sizeof(pathbuf));
            realpath(pathbuf, folderpath);
            *strrchr(folderpath, '/') = '\0';

            void* lib_azurite_handle = dlopen(AZURITE_DLL_NAME, RTLD_LOCAL|RTLD_LAZY);
            if( !lib_azurite_handle ) {
                const char* lookup_paths[] =
                {
                    "/" AZURITE_DLL_NAME,
                    "/../Frameworks/" AZURITE_DLL_NAME, // - (bundle folder)/Contents/Frameworks/
                    "/../../../" AZURITE_DLL_NAME // aside of bundle folder (SDK samples)
                };
                for( int n = 0; !lib_azurite_handle && n < 3; ++n ) {
                    char tpath[PROC_PIDPATHINFO_MAXSIZE];
                    strcpy(tpath,folderpath);
                    strcat(tpath, lookup_paths[n]);
                    lib_azurite_handle = dlopen(tpath, RTLD_LOCAL|RTLD_LAZY);
                }
            }
            if (!lib_azurite_handle) {
                fprintf(stderr, "[%s] Unable to load library: %s\n", __FILE__, dlerror());
                exit(EXIT_FAILURE);
            }

            AzuriteAPI_ptr sapi = (AzuriteAPI_ptr) dlsym(lib_azurite_handle, "AzuriteAPI");
            if (!sapi) {
                fprintf(stderr,"[%s] Unable to get symbol: %s\n", __FILE__, dlerror());
                exit(EXIT_FAILURE);
            }
            _api = sapi();
            //tiscript::ni( _api->TIScriptAPI() );
        }
        assert(_api);
        return _api;
    }

#elif defined(ANDROID)

#if 1
    EXTERN_C IAzuriteAPI *AzuriteAPI();

    inline IAzuriteAPI* _SAPI( IAzuriteAPI* ext )
    {
        static IAzuriteAPI* _api = NULL;
        if( ext )
          _api = ext;
        else
          _api = AzuriteAPI();
        assert(_api);
        //if (!sapi) {
        //  fprintf(stderr, "[%s] Unable to get symbol: %s\n", __FILE__, dlerror());
        //  exit(EXIT_FAILURE);
        //}
        return _api;
    }

#else
    inline IAzuriteAPI *_SAPI(IAzuriteAPI *ext) {
      static IAzuriteAPI *_api = NULL;
      if (ext) _api = ext;
      if (!_api) {
        void *lib_azurite_handle = NULL;
        // 1. try to load from the same folder as this executable
        lib_azurite_handle = dlopen(AZURITE_DLL_NAME, RTLD_LOCAL | RTLD_LAZY);
        if (!lib_azurite_handle) {
          fprintf(stderr, "[%s] Unable to load library: %s\n", dlerror(), AZURITE_DLL_NAME);
        }

        AzuriteAPI_ptr sapi = (AzuriteAPI_ptr)dlsym(lib_azurite_handle, "AzuriteAPI");
        if (!sapi) {
          fprintf(stderr, "[%s] Unable to get symbol: %s\n", __FILE__, dlerror());
          exit(EXIT_FAILURE);
        }
        _api = sapi();
      }
      assert(_api);
      return _api;
}

#endif

#elif defined(LINUX)

#include <libgen.h>
#include <dlfcn.h>

// typedef IAzuriteAPI* SCAPI (*AzuriteAPI_ptr)();

inline IAzuriteAPI *_SAPI(IAzuriteAPI *ext) {
  static IAzuriteAPI *_api = NULL;
  if (ext) _api = ext;
  if (!_api) {

    char folderpath[2048] = {0};

    if (readlink("/proc/self/exe", folderpath, sizeof(folderpath)) != -1) {
      dirname(folderpath);
      // strcat  (pathbuf, "/");
    }

    void *lib_azurite_handle = NULL;

    {
      // 1. try to load from the same folder as this executable
      const char *lookup_paths[] = {"/" AZURITE_DLL_NAME, NULL};
      for (int n = 0; !lib_azurite_handle; ++n) {
        if (!lookup_paths[n]) break;
        char tpath[2048] = {0};
        strcpy(tpath, folderpath);
        strcat(tpath, lookup_paths[n]);
        // fprintf(stdout, "[%s]\n",tpath);
        lib_azurite_handle = dlopen(tpath, RTLD_LOCAL | RTLD_LAZY);
        if (!lib_azurite_handle) {
          fprintf(stderr, "[%s] Unable to load library: %s\n", dlerror(),
                  tpath);
        }
      }
    }

    if (!lib_azurite_handle) // 2. no luck, try to load from system paths
      lib_azurite_handle = dlopen(AZURITE_DLL_NAME, RTLD_LOCAL | RTLD_LAZY);

    if (!lib_azurite_handle) {
      fprintf(stderr, "[%s] Unable to load library: %s\n", __FILE__, dlerror());
      exit(EXIT_FAILURE);
    }

    AzuriteAPI_ptr sapi = (AzuriteAPI_ptr)dlsym(lib_azurite_handle, "AzuriteAPI");
    if (!sapi) {
      fprintf(stderr, "[%s] Unable to get symbol: %s\n", __FILE__, dlerror());
      exit(EXIT_FAILURE);
    }
    _api = sapi();
  }
  assert(_api);
  return _api;
}

#endif


  inline IAzuriteAPI* SAPI() {
#if defined(__cplusplus) && !defined(PLAIN_API_ONLY)
    static IAzuriteAPI* _api = _SAPI(NULL);
#else 
    static IAzuriteAPI* _api = NULL;
    if (!_api) _api = _SAPI(NULL);
#endif
    return _api;
  }

  inline LPAzuriteGraphicsAPI gapi()
  {
    return SAPI()->GetAzuriteGraphicsAPI();
  }

  inline LPAzuriteRequestAPI rapi()
  {
    return SAPI()->GetAzuriteRequestAPI();
  }


  // defining "official" API functions:

  inline   LPCWSTR SCAPI AzuriteClassName (void) { return SAPI()->AzuriteClassName(); }
  inline   UINT    SCAPI AzuriteVersion (SBOOL major) { return SAPI()->AzuriteVersion (major); }
  inline   SBOOL    SCAPI AzuriteDataReady (HWINDOW hwnd,LPCWSTR uri,LPCBYTE data, UINT dataLength) { return SAPI()->AzuriteDataReady (hwnd,uri,data,dataLength); }
  inline   SBOOL    SCAPI AzuriteDataReadyAsync (HWINDOW hwnd,LPCWSTR uri, LPCBYTE data, UINT dataLength, LPVOID requestId) { return SAPI()->AzuriteDataReadyAsync (hwnd,uri, data, dataLength, requestId); }
#if defined(WINDOWS) && !defined(WINDOWLESS)
  inline   LRESULT SCAPI AzuriteProc (HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { return SAPI()->AzuriteProc (hwnd,msg,wParam,lParam); }
  inline   LRESULT SCAPI AzuriteProcND (HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam, SBOOL* pbHandled) { return SAPI()->AzuriteProcND (hwnd,msg,wParam,lParam,pbHandled); }
#endif
  inline   SBOOL    SCAPI AzuriteLoadFile (HWINDOW hWndAzurite, LPCWSTR filename) { return SAPI()->AzuriteLoadFile (hWndAzurite,filename); }
  inline   SBOOL    SCAPI AzuriteLoadHtml (HWINDOW hWndAzurite, LPCBYTE html, UINT htmlSize, LPCWSTR baseUrl) { return SAPI()->AzuriteLoadHtml (hWndAzurite,html,htmlSize,baseUrl); }
  inline   VOID    SCAPI AzuriteSetCallback (HWINDOW hWndAzurite, LPAzuriteHostCallback cb, LPVOID cbParam) { SAPI()->AzuriteSetCallback (hWndAzurite,cb,cbParam); }
  inline   SBOOL    SCAPI AzuriteSetMasterCSS (LPCBYTE utf8, UINT numBytes) { return SAPI()->AzuriteSetMasterCSS (utf8,numBytes); }
  inline   SBOOL    SCAPI AzuriteAppendMasterCSS (LPCBYTE utf8, UINT numBytes) { return SAPI()->AzuriteAppendMasterCSS (utf8,numBytes); }
  inline   SBOOL    SCAPI AzuriteSetCSS (HWINDOW hWndAzurite, LPCBYTE utf8, UINT numBytes, LPCWSTR baseUrl, LPCWSTR mediaType) { return SAPI()->AzuriteSetCSS (hWndAzurite,utf8,numBytes,baseUrl,mediaType); }
  inline   SBOOL    SCAPI AzuriteSetMediaType (HWINDOW hWndAzurite, LPCWSTR mediaType) { return SAPI()->AzuriteSetMediaType (hWndAzurite,mediaType); }
  inline   SBOOL    SCAPI AzuriteSetMediaVars (HWINDOW hWndAzurite, const AZURITE_VALUE *mediaVars) { return SAPI()->AzuriteSetMediaVars (hWndAzurite,mediaVars); }
  inline   UINT    SCAPI AzuriteGetMinWidth (HWINDOW hWndAzurite) { return SAPI()->AzuriteGetMinWidth (hWndAzurite); }
  inline   UINT    SCAPI AzuriteGetMinHeight (HWINDOW hWndAzurite, UINT width) { return SAPI()->AzuriteGetMinHeight (hWndAzurite,width); }
  inline   SBOOL    SCAPI AzuriteCall (HWINDOW hWnd, LPCSTR functionName, UINT argc, const AZURITE_VALUE* argv, AZURITE_VALUE* retval) { return SAPI()->AzuriteCall (hWnd,functionName, argc,argv,retval); }
  inline   SBOOL    SCAPI AzuriteEval ( HWINDOW hwnd, LPCWSTR script, UINT scriptLength, AZURITE_VALUE* pretval) { return SAPI()->AzuriteEval ( hwnd, script, scriptLength, pretval); }
  inline   VOID    SCAPI AzuriteUpdateWindow(HWINDOW hwnd) { SAPI()->AzuriteUpdateWindow(hwnd); }
#if defined(WINDOWS) && !defined(WINDOWLESS)
  inline   SBOOL    SCAPI AzuriteTranslateMessage (MSG* lpMsg) { return SAPI()->AzuriteTranslateMessage (lpMsg); }
#endif
  inline  SBOOL    SCAPI AzuriteSetOption (HWINDOW hWnd, UINT option, UINT_PTR value ) { return SAPI()->AzuriteSetOption (hWnd,option,value ); }
  inline  VOID    SCAPI AzuriteGetPPI (HWINDOW hWndAzurite, UINT* px, UINT* py) { SAPI()->AzuriteGetPPI (hWndAzurite,px,py); }
  inline  SBOOL    SCAPI AzuriteGetViewExpando ( HWINDOW hwnd, VALUE* pval ) { return SAPI()->AzuriteGetViewExpando ( hwnd, pval ); }
#if defined(WINDOWS) && !defined(WINDOWLESS)
  inline  SBOOL    SCAPI AzuriteRenderD2D (HWINDOW hWndAzurite, IUnknown* /*ID2D1RenderTarget**/ prt) { return SAPI()->AzuriteRenderD2D (hWndAzurite,prt); }
  inline  SBOOL    SCAPI AzuriteD2DFactory (IUnknown** /*ID2D1Factory ***/ ppf) { return SAPI()->AzuriteD2DFactory (ppf); }
  inline  SBOOL    SCAPI AzuriteDWFactory (IUnknown** /*IDWriteFactory ***/ ppf) { return SAPI()->AzuriteDWFactory (ppf); }
#endif
  inline  SBOOL    SCAPI AzuriteGraphicsCaps (LPUINT pcaps) { return SAPI()->AzuriteGraphicsCaps (pcaps); }
  inline  SBOOL    SCAPI AzuriteSetHomeURL (HWINDOW hWndAzurite, LPCWSTR baseUrl) { return SAPI()->AzuriteSetHomeURL (hWndAzurite,baseUrl); }
#if defined(OSX) && !defined(WINDOWLESS)
  inline  HWINDOW SCAPI AzuriteCreateNSView ( LPRECT frame ) { return SAPI()->AzuriteCreateNSView ( frame ); }
#endif
#if !defined(WINDOWLESS)
  inline  HWINDOW SCAPI AzuriteCreateWindow ( UINT creationFlags,LPRECT frame, AzuriteWindowDelegate* delegate, LPVOID delegateParam, HWINDOW parent) { return SAPI()->AzuriteCreateWindow (creationFlags,frame,delegate,delegateParam,parent); }
#endif

  inline SCDOM_RESULT SCAPI Azurite_UseElement(HELEMENT he) { return SAPI()->Azurite_UseElement(he); }
  inline SCDOM_RESULT SCAPI Azurite_UnuseElement(HELEMENT he) { return SAPI()->Azurite_UnuseElement(he); }
  inline SCDOM_RESULT SCAPI AzuriteGetRootElement(HWINDOW hwnd, HELEMENT *phe) { return SAPI()->AzuriteGetRootElement(hwnd, phe); }
  inline SCDOM_RESULT SCAPI AzuriteGetFocusElement(HWINDOW hwnd, HELEMENT *phe) { return SAPI()->AzuriteGetFocusElement(hwnd, phe); }
  inline SCDOM_RESULT SCAPI AzuriteFindElement(HWINDOW hwnd, POINT pt, HELEMENT* phe) { return SAPI()->AzuriteFindElement(hwnd,pt,phe); }
  inline SCDOM_RESULT SCAPI AzuriteGetChildrenCount(HELEMENT he, UINT* count) { return SAPI()->AzuriteGetChildrenCount(he, count); }
  inline SCDOM_RESULT SCAPI AzuriteGetNthChild(HELEMENT he, UINT n, HELEMENT* phe) { return SAPI()->AzuriteGetNthChild(he,n,phe); }
  inline SCDOM_RESULT SCAPI AzuriteGetParentElement(HELEMENT he, HELEMENT* p_parent_he) { return SAPI()->AzuriteGetParentElement(he,p_parent_he); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementHtmlCB(HELEMENT he, SBOOL outer, LPCBYTE_RECEIVER* rcv, LPVOID rcv_param) { return SAPI()->AzuriteGetElementHtmlCB( he, outer, rcv, rcv_param); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementTextCB(HELEMENT he, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param) { return SAPI()->AzuriteGetElementTextCB(he, rcv, rcv_param); }
  inline SCDOM_RESULT SCAPI AzuriteSetElementText(HELEMENT he, LPCWSTR utf16, UINT length) { return SAPI()->AzuriteSetElementText(he, utf16, length); }
  inline SCDOM_RESULT SCAPI AzuriteGetAttributeCount(HELEMENT he, LPUINT p_count) { return SAPI()->AzuriteGetAttributeCount(he, p_count); }
  inline SCDOM_RESULT SCAPI AzuriteGetNthAttributeNameCB(HELEMENT he, UINT n, LPCSTR_RECEIVER* rcv, LPVOID rcv_param) { return SAPI()->AzuriteGetNthAttributeNameCB(he,n,rcv,rcv_param); }
  inline SCDOM_RESULT SCAPI AzuriteGetNthAttributeValueCB(HELEMENT he, UINT n, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param) { return SAPI()->AzuriteGetNthAttributeValueCB(he, n, rcv, rcv_param); }
  inline SCDOM_RESULT SCAPI AzuriteGetAttributeByNameCB(HELEMENT he, LPCSTR name, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param) { return SAPI()->AzuriteGetAttributeByNameCB(he,name,rcv,rcv_param); }
  inline SCDOM_RESULT SCAPI AzuriteSetAttributeByName(HELEMENT he, LPCSTR name, LPCWSTR value) { return SAPI()->AzuriteSetAttributeByName(he,name,value); }
  inline SCDOM_RESULT SCAPI AzuriteClearAttributes(HELEMENT he) { return SAPI()->AzuriteClearAttributes(he); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementIndex(HELEMENT he, LPUINT p_index) { return SAPI()->AzuriteGetElementIndex(he,p_index); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementType(HELEMENT he, LPCSTR* p_type) { return SAPI()->AzuriteGetElementType(he,p_type); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementTypeCB(HELEMENT he, LPCSTR_RECEIVER* rcv, LPVOID rcv_param) { return SAPI()->AzuriteGetElementTypeCB(he,rcv,rcv_param); }
  inline SCDOM_RESULT SCAPI AzuriteGetStyleAttributeCB(HELEMENT he, LPCSTR name, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param) { return SAPI()->AzuriteGetStyleAttributeCB(he,name,rcv,rcv_param); }
  inline SCDOM_RESULT SCAPI AzuriteSetStyleAttribute(HELEMENT he, LPCSTR name, LPCWSTR value) { return SAPI()->AzuriteSetStyleAttribute(he,name,value); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementLocation(HELEMENT he, LPRECT p_location, UINT areas /*ELEMENT_AREAS*/) { return SAPI()->AzuriteGetElementLocation(he,p_location,areas); }
  inline SCDOM_RESULT SCAPI AzuriteScrollToView(HELEMENT he, UINT AzuriteScrollFlags) { return SAPI()->AzuriteScrollToView(he,AzuriteScrollFlags); }
  inline SCDOM_RESULT SCAPI AzuriteUpdateElement(HELEMENT he, SBOOL andForceRender) { return SAPI()->AzuriteUpdateElement(he,andForceRender); }
  inline SCDOM_RESULT SCAPI AzuriteRefreshElementArea(HELEMENT he, RECT rc) { return SAPI()->AzuriteRefreshElementArea(he,rc); }
  inline SCDOM_RESULT SCAPI AzuriteSetCapture(HELEMENT he) { return SAPI()->AzuriteSetCapture(he); }
  inline SCDOM_RESULT SCAPI AzuriteReleaseCapture(HELEMENT he) { return SAPI()->AzuriteReleaseCapture(he); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementHwnd(HELEMENT he, HWINDOW* p_hwnd, SBOOL rootWindow) { return SAPI()->AzuriteGetElementHwnd(he,p_hwnd,rootWindow); }
  inline SCDOM_RESULT SCAPI AzuriteCombineURL(HELEMENT he, LPWSTR szUrlBuffer, UINT UrlBufferSize) { return SAPI()->AzuriteCombineURL(he,szUrlBuffer,UrlBufferSize); }
  inline SCDOM_RESULT SCAPI AzuriteSelectElements(HELEMENT  he, LPCSTR    CSS_selectors, AzuriteElementCallback* callback, LPVOID param) { return SAPI()->AzuriteSelectElements(he,CSS_selectors,callback,param); }
  inline SCDOM_RESULT SCAPI AzuriteSelectElementsW(HELEMENT  he, LPCWSTR   CSS_selectors, AzuriteElementCallback* callback, LPVOID param) { return SAPI()->AzuriteSelectElementsW(he,CSS_selectors,callback,param); }
  inline SCDOM_RESULT SCAPI AzuriteSelectParent(HELEMENT  he, LPCSTR    selector, UINT      depth, HELEMENT* heFound) { return SAPI()->AzuriteSelectParent(he,selector,depth,heFound); }
  inline SCDOM_RESULT SCAPI AzuriteSelectParentW(HELEMENT  he, LPCWSTR   selector, UINT      depth, HELEMENT* heFound) { return SAPI()->AzuriteSelectParentW(he,selector,depth,heFound); }
  inline SCDOM_RESULT SCAPI AzuriteSetElementHtml(HELEMENT he, const BYTE* html, UINT htmlLength, UINT where) { return SAPI()->AzuriteSetElementHtml(he,html,htmlLength,where); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementUID(HELEMENT he, UINT* puid) { return SAPI()->AzuriteGetElementUID(he,puid); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementByUID(HWINDOW hwnd, UINT uid, HELEMENT* phe) { return SAPI()->AzuriteGetElementByUID(hwnd,uid,phe); }
  inline SCDOM_RESULT SCAPI AzuriteShowPopup(HELEMENT hePopup, HELEMENT heAnchor, UINT placement) { return SAPI()->AzuriteShowPopup(hePopup,heAnchor,placement); }
  inline SCDOM_RESULT SCAPI AzuriteShowPopupAt(HELEMENT hePopup, POINT pos, UINT placement) { return SAPI()->AzuriteShowPopupAt(hePopup,pos,placement); }
  inline SCDOM_RESULT SCAPI AzuriteHidePopup(HELEMENT he) { return SAPI()->AzuriteHidePopup(he); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementState( HELEMENT he, UINT* pstateBits) { return SAPI()->AzuriteGetElementState(he,pstateBits); }
  inline SCDOM_RESULT SCAPI AzuriteSetElementState( HELEMENT he, UINT stateBitsToSet, UINT stateBitsToClear, SBOOL updateView) { return SAPI()->AzuriteSetElementState(he,stateBitsToSet,stateBitsToClear,updateView); }
  inline SCDOM_RESULT SCAPI AzuriteCreateElement( LPCSTR tagname, LPCWSTR textOrNull, /*out*/ HELEMENT *phe ) { return SAPI()->AzuriteCreateElement(tagname,textOrNull,phe ); }
  inline SCDOM_RESULT SCAPI AzuriteCloneElement( HELEMENT he, /*out*/ HELEMENT *phe ) { return SAPI()->AzuriteCloneElement(he,phe ); }
  inline SCDOM_RESULT SCAPI AzuriteInsertElement( HELEMENT he, HELEMENT hparent, UINT index ) { return SAPI()->AzuriteInsertElement(he,hparent,index ); }
  inline SCDOM_RESULT SCAPI AzuriteDetachElement( HELEMENT he ) { return SAPI()->AzuriteDetachElement( he ); }
  inline SCDOM_RESULT SCAPI AzuriteDeleteElement(HELEMENT he) { return SAPI()->AzuriteDeleteElement(he); }
  inline SCDOM_RESULT SCAPI AzuriteSetTimer( HELEMENT he, UINT milliseconds, UINT_PTR timer_id ) { return SAPI()->AzuriteSetTimer(he,milliseconds,timer_id ); }
  inline SCDOM_RESULT SCAPI AzuriteDetachEventHandler( HELEMENT he, LPELEMENT_EVENT_PROC pep, LPVOID tag ) { return SAPI()->AzuriteDetachEventHandler(he,pep,tag ); }
  inline SCDOM_RESULT SCAPI AzuriteAttachEventHandler( HELEMENT he, LPELEMENT_EVENT_PROC pep, LPVOID tag ) { return SAPI()->AzuriteAttachEventHandler( he,pep,tag ); }
  inline SCDOM_RESULT SCAPI AzuriteWindowAttachEventHandler( HWINDOW hwndLayout, LPELEMENT_EVENT_PROC pep, LPVOID tag, UINT subscription ) { return SAPI()->AzuriteWindowAttachEventHandler(hwndLayout,pep,tag,subscription ); }
  inline SCDOM_RESULT SCAPI AzuriteWindowDetachEventHandler( HWINDOW hwndLayout, LPELEMENT_EVENT_PROC pep, LPVOID tag ) { return SAPI()->AzuriteWindowDetachEventHandler(hwndLayout,pep,tag ); }
  inline SCDOM_RESULT SCAPI AzuriteSendEvent( HELEMENT he, UINT appEventCode, HELEMENT heSource, UINT_PTR reason, /*out*/ SBOOL* handled) { return SAPI()->AzuriteSendEvent( he,appEventCode,heSource,reason,handled); }
  inline SCDOM_RESULT SCAPI AzuritePostEvent( HELEMENT he, UINT appEventCode, HELEMENT heSource, UINT_PTR reason) { return SAPI()->AzuritePostEvent(he,appEventCode,heSource,reason); }
  inline SCDOM_RESULT SCAPI AzuriteFireEvent( const BEHAVIOR_EVENT_PARAMS* evt, SBOOL post, SBOOL *handled) { return SAPI()->AzuriteFireEvent( evt, post, handled ); }
  inline SCDOM_RESULT SCAPI AzuriteCallBehaviorMethod(HELEMENT he, struct METHOD_PARAMS* params) { return SAPI()->AzuriteCallBehaviorMethod(he,params); }
  inline SCDOM_RESULT SCAPI AzuriteRequestElementData( HELEMENT he, LPCWSTR url, UINT dataType, HELEMENT initiator ) { return SAPI()->AzuriteRequestElementData(he,url,dataType,initiator ); }
  inline SCDOM_RESULT SCAPI AzuriteHttpRequest( HELEMENT he, LPCWSTR url, UINT dataType, UINT requestType, struct REQUEST_PARAM* requestParams, UINT nParams) { return SAPI()->AzuriteHttpRequest(he,url,dataType,requestType,requestParams,nParams); }
  inline SCDOM_RESULT SCAPI AzuriteGetScrollInfo( HELEMENT he, LPPOINT scrollPos, LPRECT viewRect, LPSIZE contentSize ) { return SAPI()->AzuriteGetScrollInfo( he,scrollPos,viewRect,contentSize ); }
  inline SCDOM_RESULT SCAPI AzuriteSetScrollPos( HELEMENT he, POINT scrollPos, SBOOL smooth ) { return SAPI()->AzuriteSetScrollPos( he,scrollPos,smooth ); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementIntrinsicWidths( HELEMENT he, INT* pMinWidth, INT* pMaxWidth ) { return SAPI()->AzuriteGetElementIntrinsicWidths(he,pMinWidth,pMaxWidth ); }
  inline SCDOM_RESULT SCAPI AzuriteGetElementIntrinsicHeight( HELEMENT he, INT forWidth, INT* pHeight ) { return SAPI()->AzuriteGetElementIntrinsicHeight( he,forWidth,pHeight ); }
  inline SCDOM_RESULT SCAPI AzuriteIsElementVisible( HELEMENT he, SBOOL* pVisible) { return SAPI()->AzuriteIsElementVisible( he,pVisible); }
  inline SCDOM_RESULT SCAPI AzuriteIsElementEnabled( HELEMENT he, SBOOL* pEnabled ) { return SAPI()->AzuriteIsElementEnabled( he, pEnabled ); }
  inline SCDOM_RESULT SCAPI AzuriteSortElements( HELEMENT he, UINT firstIndex, UINT lastIndex, ELEMENT_COMPARATOR* cmpFunc, LPVOID cmpFuncParam ) { return SAPI()->AzuriteSortElements( he, firstIndex, lastIndex, cmpFunc, cmpFuncParam ); }
  inline SCDOM_RESULT SCAPI AzuriteSwapElements( HELEMENT he1, HELEMENT he2 ) { return SAPI()->AzuriteSwapElements( he1,he2 ); }
  inline SCDOM_RESULT SCAPI AzuriteTraverseUIEvent( UINT evt, LPVOID eventCtlStruct, SBOOL* bOutProcessed ) { return SAPI()->AzuriteTraverseUIEvent( evt,eventCtlStruct,bOutProcessed ); }
  inline SCDOM_RESULT SCAPI AzuriteCallScriptingMethod( HELEMENT he, LPCSTR name, const VALUE* argv, UINT argc, VALUE* retval ) { return SAPI()->AzuriteCallScriptingMethod( he,name,argv,argc,retval ); }
  inline SCDOM_RESULT SCAPI AzuriteCallScriptingFunction( HELEMENT he, LPCSTR name, const VALUE* argv, UINT argc, VALUE* retval ) { return SAPI()->AzuriteCallScriptingFunction( he,name,argv,argc,retval ); }
  inline SCDOM_RESULT SCAPI AzuriteEvalElementScript( HELEMENT he, LPCWSTR script, UINT scriptLength, VALUE* retval ) { return SAPI()->AzuriteEvalElementScript( he, script, scriptLength, retval ); }
  inline SCDOM_RESULT SCAPI AzuriteAttachHwndToElement(HELEMENT he, HWINDOW hwnd) { return SAPI()->AzuriteAttachHwndToElement(he,hwnd); }
  inline SCDOM_RESULT SCAPI AzuriteControlGetType( HELEMENT he, /*CTL_TYPE*/ UINT *pType ) { return SAPI()->AzuriteControlGetType( he, pType ); }
  inline SCDOM_RESULT SCAPI AzuriteGetValue( HELEMENT he, VALUE* pval ) { return SAPI()->AzuriteGetValue( he,pval ); }
  inline SCDOM_RESULT SCAPI AzuriteSetValue( HELEMENT he, const VALUE* pval ) { return SAPI()->AzuriteSetValue( he, pval ); }
  inline SCDOM_RESULT SCAPI AzuriteGetExpando( HELEMENT he, VALUE* pval, SBOOL forceCreation ) { return SAPI()->AzuriteGetExpando( he, pval, forceCreation ); }
  //inline SCDOM_RESULT SCAPI AzuriteGetObject( HELEMENT he, void* pval, SBOOL forceCreation ) { return SAPI()->AzuriteGetObject( he, pval, forceCreation ); }
  //inline SCDOM_RESULT SCAPI AzuriteGetElementNamespace(  HELEMENT he, void* pval) { return SAPI()->AzuriteGetElementNamespace( he,pval); }
  inline SCDOM_RESULT SCAPI AzuriteGetHighlightedElement(HWINDOW hwnd, HELEMENT* phe) { return SAPI()->AzuriteGetHighlightedElement(hwnd, phe); }
  inline SCDOM_RESULT SCAPI AzuriteSetHighlightedElement(HWINDOW hwnd, HELEMENT he) { return SAPI()->AzuriteSetHighlightedElement(hwnd,he); }
  inline SCDOM_RESULT SCAPI AzuriteNodeAddRef(HNODE hn) { return SAPI()->AzuriteNodeAddRef(hn); }
  inline SCDOM_RESULT SCAPI AzuriteNodeRelease(HNODE hn) { return SAPI()->AzuriteNodeRelease(hn); }
  inline SCDOM_RESULT SCAPI AzuriteNodeCastFromElement(HELEMENT he, HNODE* phn) { return SAPI()->AzuriteNodeCastFromElement(he,phn); }
  inline SCDOM_RESULT SCAPI AzuriteNodeCastToElement(HNODE hn, HELEMENT* he) { return SAPI()->AzuriteNodeCastToElement(hn,he); }
  inline SCDOM_RESULT SCAPI AzuriteNodeFirstChild(HNODE hn, HNODE* phn) { return SAPI()->AzuriteNodeFirstChild(hn,phn); }
  inline SCDOM_RESULT SCAPI AzuriteNodeLastChild(HNODE hn, HNODE* phn) { return SAPI()->AzuriteNodeLastChild(hn, phn); }
  inline SCDOM_RESULT SCAPI AzuriteNodeNextSibling(HNODE hn, HNODE* phn) { return SAPI()->AzuriteNodeNextSibling(hn, phn); }
  inline SCDOM_RESULT SCAPI AzuriteNodePrevSibling(HNODE hn, HNODE* phn) { return SAPI()->AzuriteNodePrevSibling(hn,phn); }
  inline SCDOM_RESULT SCAPI AzuriteNodeParent(HNODE hnode, HELEMENT* pheParent) { return SAPI()->AzuriteNodeParent(hnode,pheParent) ; }
  inline SCDOM_RESULT SCAPI AzuriteNodeNthChild(HNODE hnode, UINT n, HNODE* phn) { return SAPI()->AzuriteNodeNthChild(hnode,n,phn); }
  inline SCDOM_RESULT SCAPI AzuriteNodeChildrenCount(HNODE hnode, UINT* pn) { return SAPI()->AzuriteNodeChildrenCount(hnode, pn); }
  inline SCDOM_RESULT SCAPI AzuriteNodeType(HNODE hnode, UINT* pNodeType /*NODE_TYPE*/) { return SAPI()->AzuriteNodeType(hnode,pNodeType); }
  inline SCDOM_RESULT SCAPI AzuriteNodeGetText(HNODE hnode, LPCWSTR_RECEIVER* rcv, LPVOID rcv_param) { return SAPI()->AzuriteNodeGetText(hnode,rcv,rcv_param); }
  inline SCDOM_RESULT SCAPI AzuriteNodeSetText(HNODE hnode, LPCWSTR text, UINT textLength) { return SAPI()->AzuriteNodeSetText(hnode,text,textLength); }
  inline SCDOM_RESULT SCAPI AzuriteNodeInsert(HNODE hnode, UINT where /*NODE_INS_TARGET*/, HNODE what) { return SAPI()->AzuriteNodeInsert(hnode,where,what); }
  inline SCDOM_RESULT SCAPI AzuriteNodeRemove(HNODE hnode, SBOOL finalize) { return SAPI()->AzuriteNodeRemove(hnode,finalize); }
  inline SCDOM_RESULT SCAPI AzuriteCreateTextNode(LPCWSTR text, UINT textLength, HNODE* phnode) { return SAPI()->AzuriteCreateTextNode(text,textLength,phnode); }
  inline SCDOM_RESULT SCAPI AzuriteCreateCommentNode(LPCWSTR text, UINT textLength, HNODE* phnode) { return SAPI()->AzuriteCreateCommentNode(text,textLength,phnode); }

  //inline HVM   SCAPI AzuriteGetVM( HWINDOW hwnd )  { return SAPI()->AzuriteGetVM(hwnd); }

  inline UINT SCAPI ValueInit ( VALUE* pval ) { return SAPI()->ValueInit(pval); }
  inline UINT SCAPI ValueClear ( VALUE* pval ) { return SAPI()->ValueClear(pval); }
  inline UINT SCAPI ValueCompare ( const VALUE* pval1, const VALUE* pval2 ) { return SAPI()->ValueCompare(pval1,pval2); }
  inline UINT SCAPI ValueCopy ( VALUE* pdst, const VALUE* psrc ) { return SAPI()->ValueCopy(pdst, psrc); }
  inline UINT SCAPI ValueIsolate ( VALUE* pdst ) { return SAPI()->ValueIsolate(pdst); }
  inline UINT SCAPI ValueType ( const VALUE* pval, UINT* pType, UINT* pUnits ) { return SAPI()->ValueType(pval,pType,pUnits); }
  inline UINT SCAPI ValueStringData ( const VALUE* pval, LPCWSTR* pChars, UINT* pNumChars ) { return SAPI()->ValueStringData(pval,pChars,pNumChars); }
  inline UINT SCAPI ValueStringDataSet ( VALUE* pval, LPCWSTR chars, UINT numChars, UINT units ) { return SAPI()->ValueStringDataSet(pval, chars, numChars, units); }
  inline UINT SCAPI ValueIntData ( const VALUE* pval, INT* pData ) { return SAPI()->ValueIntData ( pval, pData ); }
  inline UINT SCAPI ValueIntDataSet ( VALUE* pval, INT data, UINT type, UINT units ) { return SAPI()->ValueIntDataSet ( pval, data,type,units ); }
  inline UINT SCAPI ValueInt64Data ( const VALUE* pval, INT64* pData ) { return SAPI()->ValueInt64Data ( pval,pData ); }
  inline UINT SCAPI ValueInt64DataSet ( VALUE* pval, INT64 data, UINT type, UINT units ) { return SAPI()->ValueInt64DataSet ( pval,data,type,units ); }
  inline UINT SCAPI ValueFloatData ( const VALUE* pval, FLOAT_VALUE* pData ) { return SAPI()->ValueFloatData ( pval,pData ); }
  inline UINT SCAPI ValueFloatDataSet ( VALUE* pval, FLOAT_VALUE data, UINT type, UINT units ) { return SAPI()->ValueFloatDataSet ( pval,data,type,units ); }
  inline UINT SCAPI ValueBinaryData ( const VALUE* pval, LPCBYTE* pBytes, UINT* pnBytes ) { return SAPI()->ValueBinaryData ( pval,pBytes,pnBytes ); }
  inline UINT SCAPI ValueBinaryDataSet ( VALUE* pval, LPCBYTE pBytes, UINT nBytes, UINT type, UINT units ) { return SAPI()->ValueBinaryDataSet ( pval,pBytes,nBytes,type,units ); }
  inline UINT SCAPI ValueElementsCount ( const VALUE* pval, INT* pn) { return SAPI()->ValueElementsCount ( pval,pn); }
  inline UINT SCAPI ValueNthElementValue ( const VALUE* pval, INT n, VALUE* pretval) { return SAPI()->ValueNthElementValue ( pval, n, pretval); }
  inline UINT SCAPI ValueNthElementValueSet ( VALUE* pval, INT n, const VALUE* pval_to_set) { return SAPI()->ValueNthElementValueSet ( pval,n,pval_to_set); }
  inline UINT SCAPI ValueNthElementKey ( const VALUE* pval, INT n, VALUE* pretval) { return SAPI()->ValueNthElementKey ( pval,n,pretval); }
  inline UINT SCAPI ValueEnumElements ( const VALUE* pval, KeyValueCallback* penum, LPVOID param) { return SAPI()->ValueEnumElements (pval,penum,param); }
  inline UINT SCAPI ValueSetValueToKey ( VALUE* pval, const VALUE* pkey, const VALUE* pval_to_set) { return SAPI()->ValueSetValueToKey ( pval, pkey, pval_to_set); }
  inline UINT SCAPI ValueGetValueOfKey ( const VALUE* pval, const VALUE* pkey, VALUE* pretval) { return SAPI()->ValueGetValueOfKey ( pval, pkey,pretval); }
  inline UINT SCAPI ValueToString ( VALUE* pval, UINT how ) { return SAPI()->ValueToString ( pval,how ); }
  inline UINT SCAPI ValueFromString ( VALUE* pval, LPCWSTR str, UINT strLength, UINT how ) { return SAPI()->ValueFromString ( pval, str,strLength,how ); }
  inline UINT SCAPI ValueInvoke ( const VALUE* pval, VALUE* pthis, UINT argc, const VALUE* argv, VALUE* pretval, LPCWSTR url) { return SAPI()->ValueInvoke ( pval, pthis, argc, argv, pretval, url); }
  inline UINT SCAPI ValueNativeFunctorSet (VALUE* pval, NATIVE_FUNCTOR_INVOKE*  pinvoke, NATIVE_FUNCTOR_RELEASE* prelease, VOID* tag ) { return SAPI()->ValueNativeFunctorSet ( pval, pinvoke,prelease,tag); }
  inline SBOOL SCAPI ValueIsNativeFunctor ( const VALUE* pval) { return SAPI()->ValueIsNativeFunctor (pval); }

#if defined(WINDOWS) && !defined(WINDOWLESS)
  inline SBOOL SCAPI AzuriteCreateOnDirectXWindow(HWINDOW hwnd, IUnknown* pSwapChain) { return SAPI()->AzuriteCreateOnDirectXWindow(hwnd,pSwapChain); }
  inline SBOOL SCAPI AzuriteRenderOnDirectXWindow(HWINDOW hwnd, HELEMENT elementToRenderOrNull, SBOOL frontLayer) { return SAPI()->AzuriteRenderOnDirectXWindow(hwnd,elementToRenderOrNull,frontLayer); }
  inline SBOOL SCAPI AzuriteRenderOnDirectXTexture(HWINDOW hwnd, HELEMENT elementToRenderOrNull, IUnknown* surface) { return SAPI()->AzuriteRenderOnDirectXTexture(hwnd,elementToRenderOrNull,surface); }
#endif

  inline   SBOOL SCAPI AzuriteProcX(HWINDOW hwnd, AZURITE_X_MSG* pMsg) { return SAPI()->AzuriteProcX(hwnd, pMsg); }
#ifdef __cplusplus
  template<class MSG>
  inline   SBOOL SCAPI AzuriteProcX(HWINDOW hwnd, const MSG &msg) {
     static_assert(offsetof(MSG, header) == 0, "must contain AZURITE_X_MSG as first memeber");
     static_assert(std::is_same<decltype(MSG::header), AZURITE_X_MSG>::value, "must contain AZURITE_X_MSG");
     return SAPI()->AzuriteProcX(hwnd, (AZURITE_X_MSG*)(&msg));
  }
#endif

  inline UINT64 SCAPI AzuriteAtomValue(const char* name) { return SAPI()->AzuriteAtomValue(name); }
  inline SBOOL  SCAPI AzuriteAtomNameCB(UINT64 atomv, LPCSTR_RECEIVER* rcv, LPVOID rcv_param) { return SAPI()->AzuriteAtomNameCB(atomv, rcv, rcv_param); }
  inline SBOOL  SCAPI AzuriteSetGlobalAsset(som_asset_t* pass) { return SAPI()->AzuriteSetGlobalAsset(pass); }
  inline SBOOL  SCAPI AzuriteReleaseGlobalAsset(som_asset_t* pass) { return SAPI()->AzuriteReleaseGlobalAsset(pass); }

  inline UINT   SCAPI AzuriteElementUnwrap(const VALUE* pval, HELEMENT* ppElement) { return SAPI()->AzuriteElementUnwrap(pval, ppElement); }
  inline UINT   SCAPI AzuriteElementWrap(VALUE* pval, HELEMENT pElement) { return SAPI()->AzuriteElementWrap(pval, pElement); }

  inline UINT   SCAPI AzuriteNodeUnwrap(const VALUE* pval, HNODE* ppNode) { return SAPI()->AzuriteNodeUnwrap(pval, ppNode); }
  inline UINT   SCAPI AzuriteNodeWrap(VALUE* pval, HNODE pNode) { return SAPI()->AzuriteNodeWrap(pval, pNode); }

#endif
