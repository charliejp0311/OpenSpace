/*****************************************************************************************
*                                                                                       *
* OpenSpace                                                                             *
*                                                                                       *
* Copyright (c) 2014-2017                                                               *
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

#ifndef __OPENSPACE_MODULE_ROVER_TERRAIN_RENDERER___ROVERPATHFILEREADER___H__
#define __OPENSPACE_MODULE_ROVER_TERRAIN_RENDERER___ROVERPATHFILEREADER___H__

#include <ghoul/misc/dictionary.h>
#include <modules/roverterrainrenderer/filehandler/subsite.h>

namespace openspace {

class RoverPathFileReader {
public:
    struct TextureInformation{
        std::vector<std::string> fileNames;
        std::vector<PointCloudInfo> cameraInfoVector;
    };

    static std::vector<std::shared_ptr<Subsite>> extractAllSubsites(const ghoul::Dictionary dictionary);
    static std::vector<std::shared_ptr<Subsite>> extractSubsitesWithModels(const ghoul::Dictionary dictionary);

private:
    static std::string convertString(const std::string sitenr, const std::string type);
    static std::shared_ptr<TextureInformation> extractTextureInfo(const std::string absoluteFilePath);
    static glm::dmat4 extractRotationMatrix(std::string filename);
};

} // namespace openspace

#endif // __OPENSPACE_MODULE_ROVER_TERRAIN_RENDERER___ROVERPATHFILEREADER___H__