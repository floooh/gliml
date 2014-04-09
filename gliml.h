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

// see GL headers
#define GLIML_GL_TEXTURE_2D 0x0DE1
#define GLIML_GL_TEXTURE_3D 0x806F
#define GLIML_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GLIML_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GLIML_GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#define GLIML_GL_BGR 0x80E0
#define GLIML_GL_BGRA 0x80E1
#define GLIML_GL_RGB8 0x8051
#define GLIML_GL_RGBA8 0x8058
#define GLIML_GL_RED 0x1903
#define GLIML_GL_LUMINANCE 0x1909
#define GLIML_GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GLIML_GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GLIML_GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GLIML_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GLIML_GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GLIML_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GLIML_GL_UNSIGNED_BYTE 0x1401

//------------------------------------------------------------------------------
namespace gliml {

/// these typedefs are inside the gliml namespace, but GL compatible
typedef unsigned int    GLenum;
typedef unsigned char   GLboolean;
typedef unsigned int    GLbitfield;
typedef void            GLvoid;
typedef signed char     GLbyte;         /* 1-byte signed */
typedef short           GLshort;        /* 2-byte signed */
typedef int             GLint;          /* 4-byte signed */
typedef unsigned char   GLubyte;        /* 1-byte unsigned */
typedef unsigned short  GLushort;       /* 2-byte unsigned */
typedef unsigned int    GLuint;         /* 4-byte unsigned */
typedef int             GLsizei;        /* 4-byte signed */
typedef float           GLfloat;        /* single precision float */
typedef float           GLclampf;       /* single precision float in [0,1] */
typedef double          GLdouble;       /* double precision float */
typedef double          GLclampd;       /* double precision float in [0,1] */
    
#ifndef GLIML_NO_DDS
/// test if image data is DDS
bool is_dds(const void* data, unsigned int size);
#endif
    
class context {
public:
    /// default constructor
    context();
    /// destructor
    ~context();
    
    #ifndef GLIML_NO_DDS
    /// load DDS image data into context
    bool load_dds(const void* data, unsigned int size);
    #endif
    /// get detailed error code if load returns false
    int error() const;
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
    /// clear the object
    void clear();

    static const int MaxNumFaces = 6;
    static const int MaxNumMipmaps = 16;
    
    int errorCode;
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
        } mipmaps[MaxNumMipmaps];
    } faces[MaxNumFaces];
};

#define GLIML_SUCCESS (0)
#define GLIML_ERROR_INVALID_COMPRESSED_FORMAT (1)

#include "gliml.inl"
#ifndef GLIML_NO_DDS
#include "gliml_dds.h"
#endif

} // namespace gliml
//------------------------------------------------------------------------------
