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

#ifndef __azurite_request_hpp__
#define __azurite_request_hpp__

#include "azurite-request.h"
#include "azurite-api.h"

#if defined(__cplusplus) && !defined( PLAIN_API_ONLY )

#include "aux-slice.h"

namespace azurite
{

    inline VOID SC_CALLBACK _LPCWSTR2STRING( LPCWSTR str, UINT str_length, LPVOID param )
    {
        azurite::string* s = (azurite::string*)param;
        *s = azurite::string(str,str_length);
    }
    inline VOID SC_CALLBACK _LPCSTR2STRING( LPCSTR str, UINT str_length, LPVOID param )
    {
        azurite::astring* s = (azurite::astring*)param;
        *s = azurite::astring(str,str_length);
    }  


/*  struct writer
  {
    virtual bool write( aux::bytes data ) = 0; // redefine to do actual writing of data.start/data.length
    static SBOOL SCAPI image_write_function(LPVOID prm, const BYTE* data, UINT data_length)
    {
      writer* pw = (writer* )prm;
      return pw->write( aux::bytes(data,data_length) );
    }
  }; */

  class request
  {
  protected:
    HREQUEST hrq;
    
    request(): hrq(0) { ; }
    
  public:
  
    request(HREQUEST h): hrq(h) { if(hrq) rapi()->RequestUse(hrq); }    
    request(const request& im): hrq(im.hrq) { if(hrq) rapi()->RequestUse(hrq); }
    request& operator = (const request& im) 
    { 
      if(hrq) rapi()->RequestUnUse(hrq); 
      hrq = im.hrq; rapi()->RequestUse(hrq); 
      return *this;
    }
    
    bool is_valid() const { return hrq != 0; }

    ~request() {
      rapi()->RequestUnUse( hrq ); 
    }
    
    azurite::astring url() const {
      azurite::astring rv; 
      rapi()->RequestUrl( hrq, _LPCSTR2STRING, &rv ); 
      return rv;
    }

    azurite::astring content_url()  const {
      azurite::astring rv; 
      rapi()->RequestContentUrl( hrq, _LPCSTR2STRING, &rv ); 
      return rv;
    }
    
    AzuriteResourceType requested_type()  const {
      AzuriteResourceType rv = AzuriteResourceType();
      rapi()->RequestGetRequestedDataType( hrq, &rv );
      return rv;
    }
    
    void succeeded( UINT status, LPCBYTE dataOrNull = NULL, UINT dataLength = 0 )  const
    {
      rapi()->RequestSetSucceeded( hrq, status, dataOrNull, dataLength);
    }

    void failed( UINT status, LPCBYTE dataOrNull = NULL, UINT dataLength = 0 )  const
    {
      rapi()->RequestSetFailed( hrq, status, dataOrNull, dataLength);
    }

    void append_data( LPCBYTE data, UINT dataLength)
    {
      rapi()->RequestAppendDataChunk( hrq, data, dataLength);
    }
    
  };

}

#endif //defined(__cplusplus) && !defined( PLAIN_API_ONLY )


#endif

