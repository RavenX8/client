#include "StdAfx.h"
#include "http.h"

CHttp::CHttp(void) {}

CHttp::~CHttp(void) {}

bool        CHttp::SendPostData(const char* szServerIP, const char* szPage, const char* pData) {
  bool      bSended = false;
  HINTERNET hInternet, hHttp, hReq;

  char* szHeader = "Content-Type: application/x-www-form-urlencoded";

  hInternet = InternetOpen( nullptr, INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0 );

  if ( hInternet != nullptr ) {
    hHttp = InternetConnect( hInternet, szServerIP, 0, "", "", INTERNET_SERVICE_HTTP, 0, 0 );

    if ( hHttp != nullptr ) {
      hReq = HttpOpenRequest( hHttp, "POST", szPage, "HTTP/1.1", nullptr
                              , nullptr, INTERNET_FLAG_NO_CACHE_WRITE, 0 );
      if ( hReq != nullptr ) {
        if ( HttpSendRequest( hReq, szHeader, (DWORD)strlen( szHeader ), (LPVOID)pData, (DWORD)strlen( pData ) ) == 1 )
          bSended = true;
        else
          bSended = false;

        //if(pRtn)
        {
          DWORD nSize, nRead;
          InternetQueryDataAvailable( hReq, &nSize, NULL, NULL );
          if ( nSize > 0 ) {
            InternetReadFile( hReq, &pData, nSize, (LPDWORD)&nRead );
          }
        }

        InternetCloseHandle( hReq );
      }

      InternetCloseHandle( hHttp );
    }

    InternetCloseHandle( hInternet );
  }

  return bSended;
}

bool        CHttp::SendQueryData(const char* szURL) {
  bool      bSended = false;
  HINTERNET hInternet;

  // char *szHeader	= NULL; // "Content-Type: html/text";

  hInternet = InternetOpen( nullptr, INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0 );

  if ( hInternet != nullptr ) {
    // URLÀ» ¿¬´Ù.
    HINTERNET hURL = InternetOpenUrl( hInternet, szURL, nullptr, 0
                                      , INTERNET_FLAG_RELOAD, 0 );
    if ( hURL ) {
      DWORD dwBufLen     = sizeof( DWORD ), dwIndex = 0;
      DWORD dwStatusCODE = 0;

      if ( HttpQueryInfo( hURL
                          , HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER
                          , &dwStatusCODE
                          , &dwBufLen
                          , &dwIndex ) ) {
        if ( dwStatusCODE == HTTP_STATUS_OK )
          bSended = true;
      }

      InternetCloseHandle( hURL );
    }

    InternetCloseHandle( hInternet );
  }

  return bSended;
}
