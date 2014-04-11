//------------------------------------------------------------------------------
//  gliml_dds.inl
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool
is_dds(const void* data, unsigned int byteSize) {
    if (byteSize > sizeof(dds_header)) {
        const dds_header* hdr = (const dds_header*) data;
        return hdr->dwMagicFourCC == ' SDD';
    }
    return false;
}

//------------------------------------------------------------------------------
bool
context::load_dds(const void* data, unsigned int byteSize) {
    GLIML_ASSERT(gliml::is_dds(data, byteSize));
    this->clear();
    
    const dds_header* hdr = (const dds_header*) data;
    const unsigned char* dataBytePtr = (const unsigned char*) hdr;
    dataBytePtr += sizeof(dds_header);

    // cubemap?
    bool isCubeMap = false;
    if (GLIML_DDSF_CUBEMAP & hdr->dwCaps2) {
        this->target = GLIML_GL_TEXTURE_2D;
        this->is2D = true;
        this->is3D = false;
        this->numFaces = 6;
        isCubeMap = true;
    }
    // 3D texture?
    else if ((GLIML_DDSF_VOLUME & hdr->dwCaps2) && (hdr->dwDepth > 0)) {
        this->target = GLIML_GL_TEXTURE_3D;
        this->is2D = false;
        this->is3D = true;
        this->numFaces = 1;
    }
    // 2D texture
    else {
        this->target = GLIML_GL_TEXTURE_2D;
        this->is2D = true;
        this->is3D = false;
        this->numFaces = 1;
    }
    
    // image format
    int bytesPerElement = 0;
    if (hdr->ddspf.dwFlags & GLIML_DDSF_FOURCC) {
        this->isCompressed = true;
        switch (hdr->ddspf.dwFourCC) {
            case GLIML_FOURCC_DXT1:
                this->format = this->internalFormat = GLIML_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                bytesPerElement = 8;
                break;
            case GLIML_FOURCC_DXT3:
                this->format = this->internalFormat = GLIML_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                bytesPerElement = 16;
                break;
            case GLIML_FOURCC_DXT5:
                this->format = this->internalFormat = GLIML_GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                bytesPerElement = 16;
                break;
            default:
                this->errorCode = GLIML_ERROR_INVALID_COMPRESSED_FORMAT;
                return false;
        }
    }
    else if ((hdr->ddspf.dwFlags & GLIML_DDSF_RGBA) && (hdr->ddspf.dwRGBBitCount == 32)) {
        this->format = GLIML_GL_BGRA;
        this->internalFormat = GLIML_GL_RGBA8;
        this->type = GLIML_GL_UNSIGNED_BYTE;
        bytesPerElement = 4;
    }
    else if ((hdr->ddspf.dwFlags & GLIML_DDSF_RGB) && (hdr->ddspf.dwRGBBitCount == 32)) {
        this->format = GLIML_GL_BGRA;
        this->internalFormat = GLIML_GL_RGBA8;
        this->type = GLIML_GL_UNSIGNED_BYTE;
        bytesPerElement = 4;
    }
    else if ((hdr->ddspf.dwFlags & GLIML_DDSF_RGB) && (hdr->ddspf.dwRGBBitCount == 24)) {
        this->format = GLIML_GL_BGR;
        this->internalFormat = GLIML_GL_RGB8;
        this->type = GLIML_GL_UNSIGNED_BYTE;
        bytesPerElement = 3;
    }
    else if (hdr->ddspf.dwRGBBitCount == 8) {
        this->format = GLIML_GL_LUMINANCE;
        this->internalFormat = GLIML_GL_RED;
        this->type = GLIML_GL_UNSIGNED_BYTE;
        bytesPerElement = 1;
    }
    
    // FIXME: what about 16-bit textures?
    
    // for each face...
    int faceIndex;
    for (faceIndex = 0; faceIndex < this->numFaces; faceIndex++) {
        face& curFace = this->faces[faceIndex];
        if (isCubeMap) {
            switch (faceIndex) {
                // FIXME: RIGHT ORDER!, e.g. Y must be swapped
                case 0:     curFace.target = GLIML_GL_TEXTURE_CUBE_MAP_POSITIVE_X; break;
                case 1:     curFace.target = GLIML_GL_TEXTURE_CUBE_MAP_NEGATIVE_X; break;
                case 2:     curFace.target = GLIML_GL_TEXTURE_CUBE_MAP_POSITIVE_Y; break;
                case 3:     curFace.target = GLIML_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y; break;
                case 4:     curFace.target = GLIML_GL_TEXTURE_CUBE_MAP_POSITIVE_Z; break;
                default:    curFace.target = GLIML_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; break;
            }
        }
        else {
            curFace.target = this->target;
        }
        curFace.numMipmaps = hdr->dwMipMapCount;
        
        // for each mipmap
        unsigned int mipIndex;
        for (mipIndex = 0; mipIndex < hdr->dwMipMapCount; mipIndex++) {
            face::mipmap& curMip = curFace.mipmaps[mipIndex];
            
            // mipmap dimensions
            int w = hdr->dwWidth >> mipIndex;
            if (w <= 0) w = 1;
            int h = hdr->dwHeight >> mipIndex;
            if (h <= 0) h = 1;
            int d = hdr->dwDepth >> mipIndex;
            if (d <= 0) d = 1;
            curMip.width = w;
            curMip.height = h;
            curMip.depth = d;
            
            // mipmap byte size
            if (this->isCompressed) {
                curMip.size = ((w+3)/4) * ((h+3)/4) * d * bytesPerElement;
            }
            else {
                curMip.size = w * h * d * bytesPerElement;
            }
            
            // set and advance surface data pointer
            curMip.data = dataBytePtr;
            dataBytePtr += curMip.size;
        }
    }
    GLIML_ASSERT(dataBytePtr == ((const unsigned char*)data) + byteSize);
    
    // ...and we're done
    return true;
}
