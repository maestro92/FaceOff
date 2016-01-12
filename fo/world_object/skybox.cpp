
#include "skybox.h"


SkyBox::SkyBox()
{
    string files[6];
    string path = "./Assets/Images/Skybox pictures/";
    files[0] = path + "skybox_left.jpg";
    files[1] = path + "skybox_back.jpg";
    files[2] = path + "skybox_top.jpg";
    files[3] = path + "skybox_bottom.jpg";
    files[4] = path + "skybox_right.jpg";
    files[5] = path + "skybox_front.jpg";
    init(files);
}


SkyBox::SkyBox(string* files)
{
    init(files);
}


void SkyBox::init(string* files)
{
    m_cubeModel = CubeModel();

    m_staticCubeMapID = Utility::loadCubemapTexture(files);

    Shader* s;

    s = new Shader("skybox.vs", "skybox.fs");
    r_skybox.addShader(s);
    r_skybox.addDataPair("u_cubeMapTextureID",  DP_INT);
}


void SkyBox::render(Pipeline& m_pipeline)
{
    glDisable(GL_DEPTH_TEST);

    r_skybox.enableShader();
    r_skybox.setData("u_cubeMapTextureID", 0, GL_TEXTURE_CUBE_MAP, m_staticCubeMapID);

    m_pipeline.pushMatrix();
        m_pipeline.translate(m_position);
        r_skybox.loadUniformLocations(m_pipeline);
        m_cubeModel.render();
    m_pipeline.popMatrix();
    r_skybox.disableShader();


    glEnable(GL_DEPTH_TEST);
}

