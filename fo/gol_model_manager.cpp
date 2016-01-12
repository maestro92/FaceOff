
#include "gol_model_manager.h"

/*
    The SquareModel is in Grid coordinates
*/



void GOLModelManager::init(int gs)
{
    m_GOLModels.push_back(new GOLModel("Dot", generateSquareGridModel(1), gs));
    m_GOLModels.push_back(new GOLModel("Square Outline", generateSquareOutlineGridModel(5), gs));
    m_GOLModels.push_back(new GOLModel("Glider", readModelFromFile("gol_glider.txt"), gs));
    m_GOLModels.push_back(new GOLModel("Lightweight Spaceship", readModelFromFile("gol_lightweight_spaceship.txt"), gs));
    m_GOLModels.push_back(new GOLModel("Blinker", readModelFromFile("gol_blinker.txt"), gs));
    m_GOLModels.push_back(new GOLModel("Beacon", readModelFromFile("gol_beacon.txt"), gs));
    m_GOLModels.push_back(new GOLModel("Beehive", readModelFromFile("gol_beehive.txt"), gs));
    m_GOLModels.push_back(new GOLModel("Loaf", readModelFromFile("gol_loaf.txt"), gs));

}

string GOLModelManager::getFilePath(string filename)
{
    string path("./Assets/Models/");
    path = path + filename;
    return path;
}


TextureDataBuffer GOLModelManager::readModelFromFile(string filename)
{
    string path = getFilePath(filename);
 //   Utility::debug("path is", path);


    ifstream file(path);
    string str;

    int rows = 0;
    int cols = 0;
    while(getline(file, str))
    {
 //       Utility::debug(str);
        cols = str.size();
        rows++;
    }
    Utility::debug("cols, rows is", glm::vec2(rows, cols));
    TextureDataBuffer buffer = Utility::createEmptyBuffer(cols, rows);

    int y = 0;
    ifstream file1(path);
    while(getline(file1, str))
    {
        for(int x=0; x<str.size(); x++)
        {
            char c = str[x];
    //        Utility::debug("c is", (c == '#'));

            buffer[y][x][0] = 255;
            buffer[y][x][1] = (c == '#') ? 0 : 255;
            buffer[y][x][2] = (c == '#') ? 0 : 255;
            buffer[y][x][3] = (c == '#') ? 0 : 255;
        }
        y++;
    }

    return buffer;
}


TextureDataBuffer GOLModelManager::generateSquareGridModel(int size)
{
    TextureDataBuffer gridData = Utility::createEmptyBuffer(size, size);

    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            gridData[y][x][0] = 255;
            gridData[y][x][1] = 0;
            gridData[y][x][2] = 0;
            gridData[y][x][3] = 0;
        }
    }
    return gridData;
}

/*
    The SquareOutlineModel is in Grid coordinates
*/
TextureDataBuffer GOLModelManager::generateSquareOutlineGridModel(int size)
{
    TextureDataBuffer gridData = Utility::createEmptyBuffer(size, size);

    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            if(y == 0 || y == size-1 || x == 0 || x == size-1)
            {
                gridData[y][x][0] = 255;
                gridData[y][x][1] = 0;
                gridData[y][x][2] = 0;
                gridData[y][x][3] = 0;
            }
            else
            {
                gridData[y][x][0] = 255;
                gridData[y][x][1] = 255;
                gridData[y][x][2] = 255;
                gridData[y][x][3] = 255;
            }
        }
    }
    return gridData;
}


GOLModel* GOLModelManager::getDefaultGOLModel()
{
    if(m_GOLModels.size()<=0 || m_GOLModels[0] == NULL)
    {
        Utility::debug("Error in GOLModelManager getDefaultGOLModel");
        exit(1);
    }
    return m_GOLModels[0];
}

GOLModel* GOLModelManager::getModel(int index)
{
    if(index >= m_GOLModels.size())
    {
        Utility::debug("Error in GOLModelManager getModel");
        exit(1);
    }
    return m_GOLModels[index];
}

vector<GOLModel*> GOLModelManager::getModels()
{
    return m_GOLModels;
}

