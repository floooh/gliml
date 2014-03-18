gliml
=====

Minimalistic image loader library for GL projects:

- header only, just include a header and optionally set a few defines to enable or disable features
- doesn't use C++ excpeption, RTTI, STL, strings or file functions
- doesn't call into GL, doesn't include GL headers
- no dynamic memory allocation if image file format allows it
- overridable assert and memory allocation functions

Basic usage:

1. load file data into memory
2. create empty **gliml::context** object 
3. call **gliml::load()** to 'convert' the file data into the gliml::context object
4. setup a GL texture using the data in the gliml::context object
5. call **gliml::release()** to free any dynamically allocated memory

Sample code (WIP):

```cpp
#include "gliml/gliml.h"

void main() {

    // load file into memory (gliml doesn't have any file I/O functions)
    std::ifstream file("my_texture.dds", std::ios::in | std::ios::binary);
    fil.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(size);
    file.read(buffer.data(), size);
    file.close();
    
    // now extract the file data into GL data using gliml, gliml tries
    // to determine the file format from the image data
    gliml::context ctx;
    if (gliml::load(ctx, &buffer.front(), size)) {
        
        // create a GL texture
        GLuint gltx;
        glGenTextures(1, &gltx);
        glBindTexture(ctx.texture_target(), gltx);

        // set desired texture params
        glTexParameteri(ctx.texture_target(), GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(ctx.texture_target(), GL_TEXTURE_WRAP_T, GL_REPEAT);
        if (ctx.is_3d()) {
            glTexParameteri(ctx.texture_target(), GL_TEXTURE_WRAP_R, GL_REPEAT);
        }
        glTexParameteri(ctx.texture_target(), GL_MAG_FILTER, GL_LINEAR);
        if (ctx.num_mips() > 0) {
            glTexParameteri(ctx.texture_target(), GL_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        else {
            glTexParameteri(ctx.texture_target(), GL_MIN_FILTER, GL_LINEAR);
        }
  
        // for each (cube-map) face...
        for (int face_index = 0; face_index < ctx.num_faces(); face_index++) {
            // for each mip-map level
            for (int mip_index = 0; mip_index < ctx.num_mips(face); mip_index++) {
                if (ctx.is_compressed()) {
                    // compressed
                    if (ctx.is_2d()) {
                        // compressed 2D or cube texture
                        glCompressedTexImage2D(ctx.image_target(face_index),
                                               mip_index,
                                               ctx.image_internal_format(),
                                               ctx.image_width(mip_index),
                                               ctx.image_height(mip_index),
                                               0,
                                               ctx.image_size(mip_index),
                                               ctx.image_data(face_index, mip_index));
                    }
                    else {
                        // compressed 3D texture
                        assert(ctx.is_3d());
                        glCompressedTexImage3D(ctx.image_target(face_index),
                                               mip_index,
                                               ctx.image_internal_format(),
                                               ctx.image_width(mip_index),
                                               ctx.image_height(mip_index),
                                               ctx.image_depth(mip_index),
                                               0,
                                               ctx.image_size(mip_index),
                                               ctx.image_data(face_index, mip_index));
                    }
                }
                else {
                    // uncompressed
                    if (ctx.is_2d()) {
                        // 2D or CUBE texture
                        glTexImage2D(ctx.image_target(face_index),
                                     mip,
                                     ctx.image_internal_format(),
                                     ctx.image_width(mip_index),
                                     ctx.image_height(mip_index),
                                     0,
                                     ctx.image_format(),
                                     ctx.image_type(),
                                     ctx.image_data(face_index, mip_index));
                    }
                    else {
                        // 3D texture
                        assert(ctx.is_3d());
                        glTexImage3D(ctx.image_target(face_index),
                                     mip,
                                     ctx.image_internal_format(),
                                     ctx.image_width(mip_index),
                                     ctx.image_height(mip_index),
                                     ctx.image_depth(mip_index),
                                     0,
                                     ctx.image_format(),
                                     ctx.image_type(),
                                     ctx.image_data(face_index, mip_index));
                    }
                }
            } // for mip...
        } // for face...

        // release dynamically allocated data (if any)
        gliml::release(ctx);
    }
}
```
