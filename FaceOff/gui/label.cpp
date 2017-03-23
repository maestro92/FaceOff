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
void Label::render(Renderer* r)
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
	Renderer* r;
	if (m_rectBgType == COLORED)
	{
		r = &Control::r_coloredRect;

		r->enableShader();
			r->setData(R_COLORED_RECT::u_color, m_rectColor);
			updatePipeline(r);
			m_quadModel.render();
		r->disableShader();
	}
	else
	{

		r = &Control::r_texturedRect;
		
		r->enableShader();
			r->setData(R_TEXTURED_RECT::u_texture, 0, GL_TEXTURE_2D, m_rectTexture);
			updatePipeline(r);
			m_quadModel.render();
		r->disableShader();
	}


	if (hasText())
	{
		// Control::m_textEngine.render(m_text, m_textStartingXs[0], m_textStartingYs[0], m_font.size, m_font.color, m_lineBreakInfos[0].lineBreaks);
		Control::m_textEngine.render(m_text, m_textStartingXs[0], m_textStartingYs[0], m_font.size, m_font.color);
	}

}


