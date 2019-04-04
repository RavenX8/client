#include "StdAfx.h"
#include <windows.h>
#include "ijl/IJL.h"
#include "LIB_IJL.h"


//---------------------------------------------------------------------------------------
bool             IJL_EncodeToJPEGFile(char* lpszPathName, unsigned long dwWidth, unsigned long dwHeight, unsigned char* pRGB) {
  bool           bres;
  IJLERR         jerr;
  unsigned long  dwRgbBufferSize;
  unsigned char* lpTemp;

  // Allocate the IJL JPEG_CORE_PROPERTIES structure.
  JPEG_CORE_PROPERTIES jcprops;

  bres = true;
  __try {
    // Initialize the Intel(R) JPEG Library.
    jerr = ijlInit( &jcprops );
    if ( IJL_OK != jerr ) {
      bres = false;
      __leave;
    }

    dwRgbBufferSize = dwWidth * dwHeight * 3;
    lpTemp          = new unsigned char [ dwRgbBufferSize ];
    if ( nullptr == lpTemp ) {
      bres = false;
      __leave;
    }

    lpTemp = pRGB;

    // Set up information to write from the pixel buffer.
    jcprops.DIBWidth       = dwWidth;
    jcprops.DIBHeight      = dwHeight; // Implies a bottom-up DIB.
    jcprops.DIBBytes       = lpTemp;
    jcprops.DIBPadBytes    = 0;
    jcprops.DIBChannels    = 3;
    jcprops.DIBColor       = IJL_RGB;
    jcprops.JPGWidth       = dwWidth;
    jcprops.JPGHeight      = dwHeight;
    jcprops.JPGFile        = const_cast<LPSTR>(lpszPathName);
    jcprops.JPGChannels    = 3;
    jcprops.JPGColor       = IJL_YCBCR;
    jcprops.JPGSubsampling = IJL_411; // 4:1:1 subsampling.
    jcprops.jquality       = 100;     // Select "good" image quality
    // Write the actual JPEG image from the pixel buffer.
    jerr = ijlWrite( &jcprops, IJL_JFILE_WRITEWHOLEIMAGE );

    if ( IJL_OK != jerr ) {
      bres = false;
      __leave;
    }
  } // __try

  __finally {
    if ( nullptr != lpTemp ) {
      delete[] lpTemp;
      lpTemp = nullptr;
    }

    // Clean up the Intel(R) JPEG Library.
    ijlFree( &jcprops );
  }

  return bres;
} // EncodeToJPEGFile ()
