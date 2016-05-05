/*****************************************************************************************
*                                                                                       *
* OpenSpace                                                                             *
*                                                                                       *
* Copyright (c) 2014-2016                                                               *
*                                                                                       *
* Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
* software and associated documentation files (the "Software"), to deal in the Software *
* without restriction, including without limitation the rights to use, copy, modify,    *
* merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
* permit persons to whom the Software is furnished to do so, subject to the following   *
* conditions:                                                                           *
*                                                                                       *
* The above copyright notice and this permission notice shall be included in all copies *
* or substantial portions of the Software.                                              *
*                                                                                       *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
* PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
* OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
****************************************************************************************/

#ifndef __TILE_PROVIDER_H__
#define __TILE_PROVIDER_H__

#include "gdal_priv.h"

#include <openspace/engine/downloadmanager.h>

#include <ghoul/logging/logmanager.h>
#include <ghoul/filesystem/filesystem.h> // absPath
#include <ghoul/opengl/texture.h>
#include <ghoul/io/texture/texturereader.h>

#include <modules/globebrowsing/geodetics/geodetic2.h>
#include <modules/globebrowsing/other/lrucache.h>
#include <modules/globebrowsing/other/concurrentjobmanager.h>
#include <modules/globebrowsing/other/gdaldataconverter.h>

//////////////////////////////////////////////////////////////////////////////////////////
//									TILE PROVIDER									    //
//////////////////////////////////////////////////////////////////////////////////////////

namespace openspace {

    using namespace ghoul::opengl;

    /**
        Provides tiles through GDAL datasets which can be defined with xml files
        for example for wms.
    */
    class TileProvider {
    public:
        TileProvider(
            const std::string& fileName,
            int tileCacheSize,
            int minimumPixelsize);
        ~TileProvider();

        std::shared_ptr<Texture> getTile(GeodeticTileIndex tileIndex);
        std::shared_ptr<Texture> getTemporaryTexture();

        void prerender();

    private:

        friend class TextureTileLoadJob;

        /**
            Fetches all the needeed texture data from the GDAL dataset.
        */
        std::shared_ptr<UninitializedTextureTile> getUninitializedTextureTile(
            const GeodeticTileIndex& tileIndex);
        
        /**
            Creates an OpenGL texture and pushes the data to the GPU.
        */
        std::shared_ptr<Texture> initializeTexture(
            std::shared_ptr<UninitializedTextureTile> uninitedTexture);

        LRUCache<HashKey, std::shared_ptr<Texture>> _tileCache;

        const std::string _filePath;

        static bool hasInitializedGDAL;
        GDALDataset* _gdalDataSet;

        // Converters are needed for all different data types since they are templated.
        GdalDataConverter<GLubyte>   _uByteConverter;
        GdalDataConverter<GLushort> _uShortConverter;
        GdalDataConverter<GLshort>  _shortConverter;
        GdalDataConverter<GLuint>   _uIntConverter;
        GdalDataConverter<GLint>    _intConverter;
        GdalDataConverter<GLfloat>  _floatConverter;
        GdalDataConverter<GLdouble> _doubleConverter;

        ConcurrentJobManager<UninitializedTextureTile> _tileLoadManager;

        std::shared_ptr<Texture> _tempTexture;
        int _minimumPixelSize;
    };

}  // namespace openspace

namespace openspace {

    using namespace ghoul::opengl;

    struct TextureTileLoadJob : public Job<UninitializedTextureTile> {
        TextureTileLoadJob(TileProvider * tileProvider, const GeodeticTileIndex& tileIndex)
            : _tileProvider(tileProvider)
            , _tileIndex(tileIndex) {

        }

        virtual ~TextureTileLoadJob() { }

        virtual void execute() {
            _uninitedTexture = _tileProvider->getUninitializedTextureTile(_tileIndex);
        }

        virtual std::shared_ptr<UninitializedTextureTile> product() {
            return _uninitedTexture;
        }


    private:
        GeodeticTileIndex _tileIndex;
        TileProvider * _tileProvider;
        std::shared_ptr<UninitializedTextureTile> _uninitedTexture;
    };
}

#endif  // __TILE_PROVIDER_H__