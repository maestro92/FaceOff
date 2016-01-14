#include "gol_model.h"

GOLModel::GOLModel()
{

}

GOLModel::GOLModel(string name, TextureDataBuffer gData, int gs)
{
    m_name = name;
    m_gridHeight = gData.size();
    m_gridWidth = gData[0].size();

    m_height = m_gridHeight * gs;
    m_width = m_gridWidth * gs;

    m_thumbnailWidth = m_gridWidth * 5;
    m_thumbnailHeight = m_gridHeight * 5;

    m_gridSize = gs;

    m_patternTexture = Utility::loadTexture(gridDataToPixelData(gData, m_gridSize), GL_NEAREST);
    m_thumbnailTexture = Utility::loadTexture(gridDataToPixelData(gData, 5), GL_NEAREST);
}


TextureDataBuffer GOLModel::gridDataToPixelData(TextureDataBuffer gridData, int gs)
{
    int gh = gridData.size();
    int gw = gridData[0].size();

    int ph = gh * gs;
    int pw = gw * gs;

    TextureDataBuffer pixelData = Utility::createEmptyBuffer(pw, ph);

    for (int y = 0; y < gh; y++)
    {
        for (int x = 0; x < gw; x++)
        {
            int sy = y * gs;
            int sx = x * gs;

            int ey = (y + 1) * gs;
            int ex = (x + 1) * gs;

            for(int py = sy; py < ey; py++)
            {
                for(int px = sx; px < ex; px++)
                {
                    pixelData[py][px][0] = gridData[y][x][0];
                    pixelData[py][px][1] = gridData[y][x][1];
                    pixelData[py][px][2] = gridData[y][x][2];
                    pixelData[py][px][3] = gridData[y][x][3];
                }
            }
        }
    }
    return pixelData;
}



string GOLModel::getName()
{
    return m_name;
}

GLuint GOLModel::getTexture()
{
    return m_patternTexture;
}

GLuint GOLModel::getThumbnailTexture()
{
    return m_thumbnailTexture;
}
