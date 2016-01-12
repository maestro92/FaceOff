#ifndef GOL_MODEL_MANAGER_H_
#define GOL_MODEL_MANAGER_H_

#include "utility.h"
#include "gol_model.h"

class GOLModelManager
{
    public:
        void init(int gs);
        TextureDataBuffer generateSquareGridModel(int gs);
        TextureDataBuffer generateSquareOutlineGridModel(int gs);
        TextureDataBuffer readModelFromFile(string filename);

        GOLModel* getDefaultGOLModel();
        GOLModel* getModel(int index);
        vector<GOLModel*> getModels();

    private:
        vector<GOLModel*> m_GOLModels;
        string getFilePath(string filename);
};


#endif // GOL_MODEL_MANAGER_H_
