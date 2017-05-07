#include "gui_manager.h"


void GUIManager::init(int screenWidth, int screenHeight)
{
	m_fpsLabel = NULL;
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
	Renderer::initRendererWrapper(vArray, &r_depthTexture, "r_depthTexture", path);
	Renderer::initRendererWrapper(vArray, &r_coloredRect, "r_coloredRect", path);
	Renderer::initRendererWrapper(vArray, &r_texturedRect, "r_texturedRect", path);
	Renderer::initRendererWrapper(vArray, &r_listBoxItemHighlight, "r_listBoxItemHighlight", path);
	Renderer::initRendererWrapper(vArray, &r_text, "r_text", path);

	Control::r_coloredRect = r_coloredRect;
	Control::r_texturedRect = r_texturedRect;
	Control::r_listBoxItemHighlight = r_listBoxItemHighlight;
	Control::m_textEngine.r_textRenderer = r_text;

	Renderer::initRendererWrapper(vArray, &r_sniperScopeView, "r_sniperScopeView", path);

	m_sniperScopeViewTextureId = utl::loadTexture("Assets/sniper_view.png");
	/*
	r_sniperScopeView.enableShader();
		r_sniperScopeView.setData(R_SNIPER_SCOPE_VIEW::u_sniperScopeViewTexture, 0, GL_TEXTURE_2D, m_sniperScopeViewTextureId);
	r_sniperScopeView.disableShader();
	*/


	int xOffset = 55;
	int yOffset = 570;

	int BAR_WIDTH = 60;
	int BAR_HEIGHT = 10;

	m_HPBar = new Bar(xOffset, yOffset, BAR_WIDTH, BAR_HEIGHT, GREEN, "icon_hp.png");

	xOffset = 175;
	m_armorBar = new Bar(xOffset, yOffset, BAR_WIDTH, BAR_HEIGHT, GRAY, "icon_armor.png");

	xOffset = 700;
	m_ammoBar = new Bar(xOffset, yOffset, BAR_WIDTH, BAR_HEIGHT, GRAY, "icon_ammo.png");

	xOffset = 0; yOffset = 0;
	m_fpsLabel = new Label("90", xOffset, yOffset, 50, 50, GRAY);

	int aimWidth = 20;
	int aimHeight = 20;

	int aimX = utl::SCREEN_WIDTH / 2 - aimWidth / 2;
	int aimY = utl::SCREEN_HEIGHT / 2;

	utl::debug("aimX", aimX);
	utl::debug("aimY", aimY);
	m_horiAim = new Label("", aimX, aimY - 1, aimWidth, 2, GREEN);

	aimX = utl::SCREEN_WIDTH / 2;
	aimY = utl::SCREEN_HEIGHT / 2 - aimHeight / 2;

	utl::debug("aimX", aimX);
	utl::debug("aimY", aimY);

	m_vertAim = new Label("", aimX - 1, aimY, 2, aimHeight, GREEN);

	addGUIComponent(m_HPBar);
	addGUIComponent(m_armorBar);
	addGUIComponent(m_ammoBar);
	addGUIComponent(m_fpsLabel);
	addGUIComponent(m_horiAim);
	addGUIComponent(m_vertAim);
	




	/*
	r_texture.printDataPairs();
	Control::r_coloredRect.printDataPairs();
	Control::r_texturedRect.printDataPairs();
	Control::r_listBoxItemHighlight.printDataPairs();
	r_sniperScopeView.printDataPairs();





	/// set shader
	Shader* s;

	/// r_TextRenderer
	s = new Shader("/gui_shaders/text.vs", "/gui_shaders/text.fs");
	r_textRenderer.addShader(s);
	r_textRenderer.addDataPair(RENDER_PASS1, "u_texture",   DP_INT);
	r_textRenderer.addDataPair(RENDER_PASS1, "u_color",     DP_VEC3);




	*/

	utl::debug("GUI manager initing");
}



/*
	if (m_healthBarGUI != NULL)
	{
		m_healthBarGUI->computeForegroundWidth(m_curHP);
	}
	
	if (m_armorBarGUI != NULL)
	{
		m_armorBarGUI->computeForegroundWidth(m_curArmor);
	}

	if (m_ammoBarGUI != NULL)
	{
		m_ammoBarGUI->computeForegroundWidth(m_curHP);
	}
*/
void GUIManager::setHP(int HP)
{
	if (m_HPBar != NULL)
	{
		m_HPBar->computeForegroundWidth(HP);
	}
}

void GUIManager::setArmor(int armor)
{
	if (m_armorBar != NULL)
	{
		m_armorBar->computeForegroundWidth(armor);
	}
}

void GUIManager::setAmmo(int ammo)
{
	if (m_ammoBar != NULL)
	{
		m_ammoBar->computeForegroundWidth(ammo);
	}
}


void GUIManager::setFPS(int fps)
{
	if (m_fpsLabel != NULL)
	{
		m_fpsLabel->setText(utl::intToStr(fps));
	}
}


void GUIManager::setSniperZoomMode(bool b)
{
	m_sniperZoomMode = b;
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
	renderTexture(textureId, fboTarget, 0, 0, m_screenWidth, m_screenHeight, r_texture);
}

void GUIManager::renderDepthTextureFullScreen(GLuint textureId)
{
	renderDepthTextureFullScreen(textureId, RENDER_TO_SCREEN);
}

void GUIManager::renderDepthTextureFullScreen(GLuint textureId, GLuint fboTarget)
{
	renderTexture(textureId, fboTarget, 0, 0, m_screenWidth, m_screenHeight, r_depthTexture);
}

void GUIManager::renderTexture(GLuint textureId, int x, int y, int width, int height, Renderer& r)
{
	renderTexture(textureId, RENDER_TO_SCREEN, x, y, width, height, r);
}

void GUIManager::renderTexture(GLuint textureId, GLuint fboTarget, int x, int y, int width, int height, Renderer& r)
{
	setupRenderToScreen(x, y, width, height);
	r.enableShader();
	r.setData(R_TEXTURE::u_texture, 0, GL_TEXTURE_2D, textureId);

    m_GUIPipeline.pushMatrix();
        m_GUIPipeline.translate(x, y, 0);
        m_GUIPipeline.scale(width, height, 1.0);

		r.setUniLocs(m_GUIPipeline);
        m_textureQuad.render();
    m_GUIPipeline.popMatrix();
	r.disableShader();
}


void GUIManager::renderTexture(GLuint TextureId, GLuint FboTarget, Rect rect)
{
    renderTexture(TextureId, FboTarget, rect.x, rect.y, rect.w, rect.h, r_texture);
}



void GUIManager::addGUIComponent(Control* control)
{
	control->setID(m_GUIComponentsID);
	m_GUIComponents.push_back(control);
}


int GUIManager::getNumGUIComponent()
{
	return m_GUIComponents.size();
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



