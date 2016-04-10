#include "gui_manager.h"


void GUIManager::init(int screenWidth, int screenHeight)
{
    m_GUIComponentsFlags = 0;

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // set m_GUIPipeline
    m_GUIPipeline.setMatrixMode(PROJECTION_MATRIX);
    m_GUIPipeline.loadIdentity();
    m_GUIPipeline.ortho(0, m_screenWidth, m_screenHeight, 0, -1, 1);

    m_GUIPipeline.setMatrixMode(MODEL_MATRIX);
    m_GUIPipeline.loadIdentity();


    m_textureQuad = QuadModel(1,1);




    Shader* s;

    /// r_textureRenderer
    s = new Shader("texture.vs", "texture.fs");
    r_textureRenderer.addShader(s);
    r_textureRenderer.addDataPair(RENDER_PASS1, "u_texture",    DP_INT);
}


void GUIManager::initGUIRenderingSetup()
{

    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}



void GUIManager::renderTextureFullScreen(GLuint textureId)
{
	renderTextureFullScreen(textureId, RENDER_TO_SCREEN);
}

void GUIManager::renderTextureFullScreen(GLuint textureId, GLuint fboTarget)
{
	renderTexture(textureId, fboTarget, 0, 0, m_screenWidth, m_screenHeight);
}

void GUIManager::renderTexture(GLuint textureId, int x, int y, int width, int height)
{
	renderTexture(textureId, RENDER_TO_SCREEN, x, y, width, height);
}

void GUIManager::renderTexture(GLuint textureId, GLuint fboTarget, int x, int y, int width, int height)
{
    glViewport(0, 0, m_screenWidth, m_screenHeight);

    r_textureRenderer.enableShader(RENDER_PASS1);
    r_textureRenderer.setData(RENDER_PASS1, "u_texture", 0, GL_TEXTURE_2D, textureId);

    m_GUIPipeline.pushMatrix();
        m_GUIPipeline.translate(x, y, 0);
        m_GUIPipeline.scale(width, height, 1.0);

        r_textureRenderer.loadUniformLocations(m_GUIPipeline, RENDER_PASS1);
        m_textureQuad.render();
    m_GUIPipeline.popMatrix();
    r_textureRenderer.disableShader(RENDER_PASS1);
}


void GUIManager::renderTexture(GLuint TextureId, GLuint FboTarget, Rect rect)
{
    renderTexture(TextureId, FboTarget, rect.x, rect.y, rect.w, rect.h);
}


void GUIManager::renderTextureSingle(GLuint TextureId, int x, int y, int width, int height)
{

}

void GUIManager::renderTextureSingle(GLuint TextureId, GLuint FboTarget, int x, int y, int width, int height)
{
    r_textureRenderer.enableShader();
    r_textureRenderer.setData(RENDER_PASS1, "u_texture", 0, GL_TEXTURE_2D, TextureId);

    m_GUIPipeline.pushMatrix();
        m_GUIPipeline.translate(x, y, 0);
        m_GUIPipeline.scale(width, height, 1.0);

        r_textureRenderer.loadUniformLocations(m_GUIPipeline);
        m_textureQuad.render();
    m_GUIPipeline.popMatrix();
    r_textureRenderer.disableShader();
}

void GUIManager::renderTextureSingle(GLuint TextureId, GLuint FboTarget, Rect rect)
{

}


void GUIManager::updateAndRender(MouseState mouseState)
{
    for(int i=0; i<m_GUIComponents.size(); i++)
    {
        Control* control = m_GUIComponents[i];
        control->update(mouseState);
		control->render();
    }
}


void GUIManager::addGUIComponent(Control* control)
{
    control->setID(m_GUIComponentsID);
    m_GUIComponents.push_back(control);
}
