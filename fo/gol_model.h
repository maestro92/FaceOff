#ifndef GOL_MODEL_H_
#define	GOL_MODEL_H_

#include "utility.h"


class GOLModel
{
    public:
        GOLModel();
        GOLModel(string name, TextureDataBuffer gData, int gs);

        string getName();
        GLuint getTexture();
        GLuint getThumbnailTexture();

        int m_width;
        int m_height;

        GLuint m_patternTexture;
        GLuint m_thumbnailTexture;

        int m_thumbnailWidth;
        int m_thumbnailHeight;

    private:
        static TextureDataBuffer gridDataToPixelData(TextureDataBuffer gridData, int gs);
        string m_name;

        int m_gridSize;
        int m_gridWidth;
        int m_gridHeight;


};


#endif
