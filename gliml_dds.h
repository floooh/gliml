#pragma once
//------------------------------------------------------------------------------
/**
    gliml_dds.h
    DDS specific code of gliml, derived from nvdds.cpp:
    https://github.com/tlorach/nvFX/blob/master/samples/shared/nvImage/nvImageDDS.cpp
*/

struct dds_pixelformat {
    unsigned long dwSize;
    unsigned long dwFlags;
    unsigned long dwFourCC;
    unsigned long dwRGBBitCount;
    unsigned long dwRBitMask;
    unsigned long dwGBitMask;
    unsigned long dwBBitMask;
    unsigned long dwABitMask;
};
    
struct dds_header {
    unsigned long dwMagicFourCC;
    unsigned long dwSize;
    unsigned long dwFlags;
    unsigned long dwHeight;
    unsigned long dwWidth;
    unsigned long dwPitchOrLinearSize;
    unsigned long dwDepth;
    unsigned long dwMipMapCount;
    unsigned long dwReserved[11];
    dds_pixelformat ddspf;
    unsigned long dwCaps1;
    unsigned long dwCaps2;
    unsigned long dwReserved2[3];
};

// surface description flags
#define GLIML_DDSF_CAPS 0x00000001
#define GLIML_DDSF_HEIGHT 0x00000002
#define GLIML_DDSF_WIDTH 0x00000004
#define GLIML_DDSF_PITCH 0x00000008
#define GLIML_DDSF_PIXELFORMAT 0x00001000
#define GLIML_DDSF_MIPMAPCOUNT 0x00020000
#define GLIML_DDSF_LINEARSIZE 0x00080000
#define GLIML_DDSF_DEPTH 0x00800000
    
// pixel format flags
#define GLIML_DDSF_ALPHAPIXELS 0x00000001
#define GLIML_DDSF_FOURCC 0x00000004
#define GLIML_DDSF_RGB 0x00000040
#define GLIML_DDSF_RGBA 0x00000041
    
// dwCaps1 flags
#define GLIML_DDSF_COMPLEX 0x00000008
#define GLIML_DDSF_TEXTURE 0x00001000
#define GLIML_DDSF_MIPMAP 0x00400000
    
// dwCaps2 flags
#define GLIML_DDSF_CUBEMAP 0x00000200
#define GLIML_DDSF_CUBEMAP_POSITIVEX 0x00000400
#define GLIML_DDSF_CUBEMAP_NEGATIVEX 0x00000800
#define GLIML_DDSF_CUBEMAP_POSITIVEY 0x00001000
#define GLIML_DDSF_CUBEMAP_NEGATIVEY 0x00002000
#define GLIML_DDSF_CUBEMAP_POSITIVEZ 0x00004000
#define GLIML_DDSF_CUBEMAP_NEGATIVEZ 0x00008000
#define GLIML_DDSF_CUBEMAP_ALL_FACES 0x0000FC00
#define GLIML_DDSF_VOLUME 0x00200000
    
// compressed texture types
#define GLIML_FOURCC_DXT1 0x31545844
#define GLIML_FOURCC_DXT3 0x33545844
#define GLIML_FOURCC_DXT5 0x35545844

#include "gliml_dds.inl"

