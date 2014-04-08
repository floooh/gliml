//------------------------------------------------------------------------------
//  gliml.inl
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
inline context::context() :
target(0),
isCompressed(false),
is2D(false),
is3D(false),
internalFormat(0),
format(0),
type(0),
numFaces(0) {
    int faceIndex;
    for (faceIndex = 0; faceIndex < MaxNumFaces; faceIndex++) {
        face& curFace = this->faces[i];
        curFace.target = 0;
        curFace.numMipmaps = 0;
        int mipIndex;
        for (mipIndex = 0; mipIndex < MaxNumMipmaps; mipIndex++) {
            mipmap& curMipmap = curFace.mipmaps[mipIndex];
            curMipmap.width = 0;
            curMipmap.height = 0;
            curMipmap.depth = 0;
            curMipmap.size  = 0;
            curMipmap.data = 0;
        }
    }
}

//------------------------------------------------------------------------------
inline bool
context::load(const void* data, int byteSize) {
    // FIXME!
    return false;
}

//------------------------------------------------------------------------------
inline GLenum
context::texture_target() const {
    return this->target;
}

//------------------------------------------------------------------------------
inline bool
context::is_compressed() const {
    return this->isCompressed;
}

//------------------------------------------------------------------------------
inline bool
context::is_2d() const {
    return this->is2D;
}

//------------------------------------------------------------------------------
inline bool
context::is_3d() const {
    return this->is3D;
}

//------------------------------------------------------------------------------
inline int
context::num_faces() const {
    return this->num_faces;
}

//------------------------------------------------------------------------------
inline int
context::num_mipmaps(int face_index) const {
    GLIML_ASSERT((face_index >= 0) && (face_index < this->num_faces));
    return this->faces[face_index].numMipmaps;
}

//------------------------------------------------------------------------------
inline GLenum
context::image_target(int face_index) const {
    GLIML_ASSERT((face_index >= 0) && (face_index < this->num_faces));
    return this->faces[face_index].target;
}

//------------------------------------------------------------------------------
inline GLint
context::image_internal_format() const {
    return this->internalFormat;
}

//------------------------------------------------------------------------------
inline GLsizei
context::image_width(int face_index, int mip_index) const {
    GLIML_ASSERT((face_index >= 0) && (face_index < this->num_faces));
    GLIML_ASSERT((mip_index >= 0) && (mip_index < this->faces[face_index].numMipmaps));
    return this->faces[face_index].mipmaps[mip_index].width;
}

//------------------------------------------------------------------------------
inline GLsizei
context::image_height(int face_index, int mip_index) const {
    GLIML_ASSERT((face_index >= 0) && (face_index < this->num_faces));
    GLIML_ASSERT((mip_index >= 0) && (mip_index < this->faces[face_index].numMipmaps));
    return this->faces[face_index].mipmaps[mip_index].height;
}

//------------------------------------------------------------------------------
inline GLsizei
context::image_depth(int face_index, int mip_index) const {
    GLIML_ASSERT((face_index >= 0) && (face_index < this->num_faces));
    GLIML_ASSERT((mip_index >= 0) && (mip_index < this->faces[face_index].numMipmaps));
    return this->faces[face_index].mipmaps[mip_index].depth;
}

//------------------------------------------------------------------------------
inline GLenum
context::image_format() const {
    return this->format;
}

//------------------------------------------------------------------------------
inline GLenum
context::image_type() const {
    return this->type;
}

//------------------------------------------------------------------------------
inline GLsizei
context::image_size(int face_index, int mip_index) const {
    GLIML_ASSERT((face_index >= 0) && (face_index < this->num_faces));
    GLIML_ASSERT((mip_index >= 0) && (mip_index < this->faces[face_index].numMipmaps));
    return this->faces[face_index].mipmaps[mip_index].size;
}

//------------------------------------------------------------------------------
inline const GLvoid*
context::image_data(int face_index, int mip_index) const {
    GLIML_ASSERT((face_index >= 0) && (face_index < this->num_faces));
    GLIML_ASSERT((mip_index >= 0) && (mip_index < this->faces[face_index].numMipmaps));
    return this->faces[face_index].mipmaps[mip_index].data;
}
