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

	m_sniperZoomMode = false;

    m_textureQuad = QuadModel(1,1);

	
	char* filename = "gui/gui_renderer_data.json";

	Value vContent = utl::readJsonFileToVector(filename);
	const Array& vArray = vContent.get_array();

	string path = "gui_shaders/";

	Renderer::initRendererWrapper(vArray, &r_texture, "r_texture", path);
	Renderer::initRendererWrapper(vArray, &r_coloredRect, "r_coloredRect", path);
	Renderer::initRendererWrapper(vArray, &r_texturedRect, "r_texturedRect", path);
	Renderer::initRendererWrapper(vArray, &r_listBoxItemHighlight, "r_listBoxItemHighlight", path);

	Control::r_coloredRect = r_coloredRect;
	Control::r_texturedRect = r_texturedRect;
	Control::r_listBoxItemHighlight = r_listBoxItemHighlight;



	Renderer::initRendererWrapper(vArray, &r_sniperScopeView, "r_sniperScopeView", path);

	m_sniperScopeViewTextureId = utl::loadTexture("Assets/sniper_view.png");
	/*
	r_sniperScopeView.enableShader();
		r_sniperScopeView.setData(R_SNIPER_SCOPE_VIEW::u_sniperScopeViewTexture, 0, GL_TEXTURE_2D, m_sniperScopeViewTextureId);
	r_sniperScopeView.disableShader();
	*/


	r_texture.printDataPairs();
	Control::r_coloredRect.printDataPairs();
	Control::r_texturedRect.printDataPairs();
	Control::r_listBoxItemHighlight.printDataPairs();
	r_sniperScopeView.printDataPairs();

	utl::debug("GUI manager initing");
}






void GUIManager::initGUIRenderingSetup()
{
	setupRenderToScreen(0, 0, m_screenWidth, m_screenHeight);
}

void GUIManager::setupRenderToScreen(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}



void GUIManager::renderSnipeScopeView(GLuint sceneTextureId)
{
	r_sniperScopeView.enableShader();
	r_sniperScopeView.setData(R_SNIPER_SCOPE_VIEW::u_sniperScopeViewTexture, 0, GL_TEXTURE_2D, m_sniperScopeViewTextureId);
	r_sniperScopeView.setData(R_SNIPER_SCOPE_VIEW::u_sceneTexture, 1, GL_TEXTURE_2D, sceneTextureId);

	m_GUIPipeline.pushMatrix();
		m_GUIPipeline.translate(0, 0, 0);
		m_GUIPipeline.scale(m_screenWidth, m_screenHeight, 1.0);

		r_sniperScopeView.setUniLocs(m_GUIPipeline);
		m_textureQuad.render();
		m_GUIPipeline.popMatrix();
	r_sniperScopeView.disableShader();
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
	setupRenderToScreen(x, y, width, height);
	r_texture.enableShader();
	r_texture.setData(R_TEXTURE::u_texture, 0, GL_TEXTURE_2D, textureId);

    m_GUIPipeline.pushMatrix();
        m_GUIPipeline.translate(x, y, 0);
        m_GUIPipeline.scale(width, height, 1.0);

		r_texture.setUniLocs(m_GUIPipeline);
        m_textureQuad.render();
    m_GUIPipeline.popMatrix();
	r_texture.disableShader();
}


void GUIManager::renderTexture(GLuint TextureId, GLuint FboTarget, Rect rect)
{
    renderTexture(TextureId, FboTarget, rect.x, rect.y, rect.w, rect.h);
}



void GUIManager::addGUIComponent(Control* control)
{
	control->setID(m_GUIComponentsID);
	m_GUIComponents.push_back(control);
}


void GUIManager::updateAndRender(MouseState mouseState)
{
    for(int i=0; i<m_GUIComponents.size(); i++)
    {
		if (m_sniperZoomMode && (i == m_horAimIndex || i == m_verAimIndex))
		{
			continue;
		}

		Control* control = m_GUIComponents[i];
		control->update(mouseState);
		control->render();
	
	}
}


void GUIManager::setSniperZoomMode(bool b)
{
	m_sniperZoomMode = b;
}

void GUIManager::setHorAimIndex(int index)
{
	m_horAimIndex = index;
}

void GUIManager::setVerAimIndex(int index)
{
	m_verAimIndex = index;
}


/*
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

	r_textureRenderer.setUniLocs(m_GUIPipeline);
	m_textureQuad.render();
	m_GUIPipeline.popMatrix();
	r_textureRenderer.disableShader();
}

void GUIManager::renderTextureSingle(GLuint TextureId, GLuint FboTarget, Rect rect)
{

}
*/



