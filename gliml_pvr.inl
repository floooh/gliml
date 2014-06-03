//------------------------------------------------------------------------------
//  gliml_pvr.inl
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool
is_pvr(const void* data, unsigned int byteSize) {
    if (byteSize > sizeof(pvr_header)) {
        const pvr_header* hdr = (const pvr_header*) data;
        return hdr->pvrTag == '!RVP';
    }
    return false;
}

//------------------------------------------------------------------------------
bool
context::load_pvr(const void* data, unsigned int byteSize) {
    GLIML_ASSERT(gliml::is_pvr(data, byteSize));
    this->clear();
    
    const pvr_header* hdr = (const pvr_header*) data;
    const unsigned char* dataBytePtr = (const unsigned char*) hdr;
    dataBytePtr += sizeof(pvr_header);
    
    // PVR only supports 2D textures(?)
    this->target = GLIML_GL_TEXTURE_2D;
    this->is2D = true;
    this->is3D = false;
    this->numFaces = 1;
    
    // image format
    if ((hdr->flags & 0xFF) == kPVRTextureFlagTypePVRTC_4) {
        this->format = this->internalFormat = GLIML_GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
    }
    else if ((hdr->flags & 0xFF) == kPVRTextureFlagTypePVRTC_2) {
        this->format = this->internalFormat = GLIML_GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
    }
    else {
        this->errorCode = GLIML_ERROR_INVALID_COMPRESSED_FORMAT;
        return false;
    }
    
    // only one face...
    face& curFace = this->faces[0];
    curFace.numMipmaps = hdr->numMipmaps + 1;
    
    // for each mipmap...
    int mipIndex;
    for (mipIndex = 0; mipIndex < curFace.numMipmaps; mipIndex++) {
        face::mipmap& curMip = curFace.mipmaps[mipIndex];
        
        // mipmap dimensions
        int w = hdr->width >> mipIndex;
        if (w <= 0) w = 1;
        int h = hdr->height >> mipIndex;
        if (h <= 0) h = 1;
        curMip.width = w;
        curMip.height = h;
        
        int blockSize, widthBlocks, heightBlocks, bpp;
        if (GLIML_GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG == this->format) {
            blockSize = 4 * 4;
            widthBlocks = w / 4;
            heightBlocks = h / 4;
            bpp = 4;
        }
        else {
            blockSize = 8 * 4;
            widthBlocks = w / 8;
            heightBlocks = h / 4;
            bpp = 2;
        }
        
        // clamp to minimal block size
        widthBlocks = widthBlocks < 2 ? 2 : widthBlocks;
        heightBlocks = heightBlocks < 2 ? 2 : heightBlocks;
        
        curMip.size = widthBlocks * heightBlocks * ((blockSize * bpp) / 8);
        curMip.data = dataBytePtr;
        dataBytePtr += curMip.size;
    }
    GLIML_ASSERT(dataBytePtr == ((const unsigned char*)data) + byteSize);
    
    // and done
    return true;
}
