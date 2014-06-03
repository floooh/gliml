#pragma once
//------------------------------------------------------------------------------
/**
    gliml_pvr.h
    PVRTC specific code of gliml.
*/

#pragma pack(push, 1)
struct pvr_header {
    unsigned int headerLength;
    unsigned int height;
    unsigned int width;
    unsigned int numMipmaps;
    unsigned int flags;
    unsigned int dataLength;
    unsigned int bpp;
    unsigned int bitmaskRed;
    unsigned int bitmaskGreen;
    unsigned int bitmaskBlue;
    unsigned int bitmaskAlpha;
    unsigned int pvrTag;
    unsigned int numSurfs;
};
#pragma pack(pop)
enum
{
    kPVRTextureFlagTypePVRTC_2 = 24,
    kPVRTextureFlagTypePVRTC_4
};

#include "gliml_pvr.inl"
