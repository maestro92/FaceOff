
#include "skybox.h"

static float angle = 0;

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

    m_staticCubeMapID = utl::loadCubemapTexture(files);

	/*
    Shader* s;

    s = new Shader("skybox.vs", "skybox.fs");
    r_skybox.addShader(s);
    r_skybox.addDataPair("u_cubeMapTextureID",  DP_INT);
	*/
}


void SkyBox::render(Pipeline& m_pipeline, Renderer* r)
{
	
    glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	r->enableShader();
	r->setData(R_SKYBOX::u_cubeMapTextureID, 0, GL_TEXTURE_CUBE_MAP, m_staticCubeMapID);

    m_pipeline.pushMatrix();
        m_pipeline.translate(m_position);
	//	m_pipeline.rotate(angle, 0.0f, 1.0f, 0.0f);
		m_pipeline.scale(m_scale);

        r->setUniLocs(m_pipeline);
        m_cubeModel.render();
    m_pipeline.popMatrix();
    r->disableShader();

	angle += 0.001;
	if (angle > 360)
		angle -= 360;
	
	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

