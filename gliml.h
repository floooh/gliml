#pragma once
//------------------------------------------------------------------------------
/**
    gliml - GL image loader library

    gliml main header file. Include this file after the GL header, and 
    optionally define the following macros before the include:
    
    GLIML_ASSERT(x)     - your custom assert implementation, default is assert
    GLIML_NO_DDS        - don't include DDS support
    GLIML_NO_PVR        - don't include PVR support
*/

#ifndef GLIML_ASSERT
#define GLIML_ASSERT(x) assert(x)
#endif

#ifndef GL_VERSION_1_1
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
#endif

#ifndef GLIML_NO_DDS
#include "gliml_dds.h"
#endif

//------------------------------------------------------------------------------
namespace gliml {
    
class context {
public:
    /// default constructor
    context();
    /// destructor
    ~context();
    
    /// load image data into context
    bool load(const void* data, int size);
    /// get the texture target of context
    GLenum texture_target() const;
    /// return true if context contains a compressed texture
    bool is_compressed() const;
    /// return true if context contains a 2D texture
    bool is_2d() const;
    /// return true if context contains a 3D texture
    bool is_3d() const;
    /// get number of faces
    int num_faces() const;
    /// get number of mipmaps in a face
    int num_mipmaps(int face_index) const;
    /// get texture target for texture image function
    GLenum image_target(int face_index) const;
    /// get internal format for texture image function
    GLint image_internal_format() const;
    /// get width for texture image function
    GLsizei image_width(int face_index, int mip_index) const;
    /// get height for texture image function
    GLsizei image_height(int face_index, int mip_index) const;
    /// get depth for texture image function (3D textures)
    GLsizei image_depth(int face_index, int mip_index) const;
    /// get image format for texture image function
    GLenum image_format() const;
    /// get type for texture image function
    GLenum image_type() const;
    /// get image size for compressed texture image function
    GLsizei image_size(int face_index, int mip_index) const;
    /// get pointer to image data
    const GLvoid* image_data(int face_index, int mip_index) const;
    
private:
    static const int MaxNumFaces = 6;
    static const int MaxNumMipmaps = 16;

    GLenum target;
    bool isCompressed;
    bool is2D;
    bool is3D;
    GLint internalFormat;
    GLenum format;
    GLenum type;
    int numFaces;
    struct face {
        GLenum target;
        int numMipmaps;
        struct mipmap {
            GLsizei width;
            GLsizei height;
            GLsizei depth;
            GLsizei size;
            const GLvoid* data;
        } mipmaps[MaxNumMipMaps];
    } faces[MaxNumFaces];
};

#include "gliml.inl"

} // namespace gliml
//------------------------------------------------------------------------------
