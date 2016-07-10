#include "Label.h"

Label::Label()
{

}

Label::Label(string text, int x, int y, int width, int height, glm::vec3 color) :
          Control(text, x, y, width, height, color)
{

}

int Label::getType()
{
    return LABEL;
}

/*
void Label::render()
{
    Control::r_coloredRectRenderer.enableShader();

        Control::r_coloredRectRenderer.setData(RENDER_PASS1, "u_color", m_rectColor);
        updatePipeline(&Control::r_coloredRectRenderer);
        m_quadModel.render();
        Control::m_textEngine.render(m_text, m_textStartingXs[0], m_textStartingYs[0], m_font.size, m_font.color, m_lineBreakInfos[0].lineBreaks);

    Control::r_coloredRectRenderer.disableShader();
}
*/


void Label::render()
{
	/*
	if (m_rectBgType == COLORED)
	{
		Control::r_coloredRectRenderer.enableShader();
			Control::r_coloredRectRenderer.setData(RENDER_PASS1, "u_color", m_rectColor);
			updatePipeline(&Control::r_coloredRectRenderer);
			m_quadModel.render();
		Control::r_coloredRectRenderer.disableShader();

		Control::m_textEngine.render(m_text, m_textStartingXs[0], m_textStartingYs[0], m_font.size, m_font.color, m_lineBreakInfos[0].lineBreaks);
	}
	else
	{
		Control::r_texturedRectRenderer.enableShader();
			Control::r_texturedRectRenderer.setData(RENDER_PASS1, "u_texture", 0, GL_TEXTURE_2D, m_rectTexture);
			updatePipeline(&Control::r_texturedRectRenderer);
			m_quadModel.render();
		Control::r_texturedRectRenderer.disableShader();

		Control::m_textEngine.render(m_text, m_textStartingXs[0], m_textStartingYs[0], m_font.size, m_font.color, m_lineBreakInfos[0].lineBreaks);
	}
	*/
}


