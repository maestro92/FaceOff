#include "Control.h"

TextEngine Control::m_textEngine;
Pipeline Control::m_pipeline;
QuadModel Control::m_quadModel;// = QuadModel(1,1);
Renderer Control::r_coloredRectRenderer;
Renderer Control::r_texturedRectRenderer;
Renderer Control::r_listBoxHighlightRenderer;
int Control::m_screenWidth;
int Control::m_screenHeight;

Control::Control() : Control("Martin :)", 0,0,1,1, BLUE)
{}

Control::Control(string text, int x, int y, int width, int height, glm::vec3 color)
{
    m_text = text;
    setRect(x,y,width,height);
    m_isInside = false;
    m_rectColor = color;
    m_rectTexture = -1;
   // m_funcCallBack = NULL;

    m_font.color = glm::vec3(0.5, 0.8f, 0.2f);
    m_font.size = 35;

    m_lineBreakInfos.resize(1);
    m_textStartingXs.resize(1);
    m_textStartingYs.resize(1);

    setTextLayout(false, CENTER, CENTER);
}

Control::~Control()
{

}

void Control::init(string font, int size, int screenWidth, int screenHeight)
{
    m_textEngine = TextEngine("", size, screenWidth, screenHeight);
    m_quadModel = QuadModel(1,1);

    Shader* s;

    /// r_coloredRect and r_texturedRect
    s = new Shader("/gui_shaders/colored_rect.vs", "/gui_shaders/colored_rect.fs");
    r_coloredRectRenderer.addShader(s);
    r_coloredRectRenderer.addDataPair(RENDER_PASS1, "u_color", DP_VEC3);

    s = new Shader("/gui_shaders/textured_rect.vs", "/gui_shaders/textured_rect.fs");
    r_texturedRectRenderer.addShader(s);
    r_texturedRectRenderer.addDataPair(RENDER_PASS1, "u_texture",    DP_INT);


    s = new Shader("/gui_shaders/list_box_item_highlight.vs", "/gui_shaders/list_box_item_highlight.fs");
    r_listBoxHighlightRenderer.addShader(s);
    r_listBoxHighlightRenderer.addDataPair(RENDER_PASS1, "u_x1",    DP_FLOAT);
    r_listBoxHighlightRenderer.addDataPair(RENDER_PASS1, "u_x2",    DP_FLOAT);
    r_listBoxHighlightRenderer.addDataPair(RENDER_PASS1, "u_y1",    DP_FLOAT);
    r_listBoxHighlightRenderer.addDataPair(RENDER_PASS1, "u_y2",    DP_FLOAT);

    /// http://stackoverflow.com/questions/25416820/set-origin-to-top-left-corner-of-screen-in-opengl-es-2
    /// to set the top left (0,0), we invert bot with top in the ortho matrix

    m_pipeline.setMatrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();
    m_pipeline.ortho(0, screenWidth, screenHeight, 0, -1, 1);

    m_pipeline.setMatrixMode(MODEL_MATRIX);
    m_pipeline.loadIdentity();

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
}

void Control::setID(int& ID)
{
    m_id = ID;
    ID++;
}

void Control::setText(string text)
{
    m_text = text;
}


void Control::setTexture(GLuint id)
{
    m_rectTexture = id;
}

void Control::setColor(glm::vec3 color)
{
    m_rectColor = color;
}

void Control::setFont(int size, glm::vec3 color)
{
    m_font.size = size;
    m_font.color = color;
}

void Control::setRect(int x, int y, int w, int h)
{
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = w;
    m_rect.h = h;
}

bool Control::update(MouseState & state)
{
    int x = state.m_pos.x;
    int y = m_screenHeight - state.m_pos.y;

  //  utl::debug("x, y", glm::vec2(x, y));
    m_isInside = false;

    if( (x >= m_rect.x && x <= m_rect.x + m_rect.w) &&
        (y >= m_rect.y && y <= m_rect.y + m_rect.h) )
    {
        m_isInside = true;
    }

    return false;
}

bool Control::update(MouseState & state, unsigned int& groupFlag)
{
    bool flag = update(state);
    if(flag)
    {
        groupFlag = groupFlag | ( 1 << m_id);
     //   utl::debug()
     //   std::bitset<32> x(groupFlag);
     //   cout << x << endl;
    }
    else
    {
        groupFlag = groupFlag & (~( 1 << m_id));
     //   std::bitset<32> x(groupFlag);
     //   cout << x << endl;
    }
    return flag;
}

void Control::setTextLayout(bool setLineBreakFlag, int xLayoutFlag, int yLayoutFlag)
{
    if(setLineBreakFlag)
    {
        m_lineBreakInfos[0] = m_textEngine.computeLineBreakInfo(m_text, m_font.size, m_rect.w);
    }

    switch(xLayoutFlag)
    {
        case LEFT_ALIGNED:
            m_textStartingXs[0] = m_rect.x + m_font.size * 0.05;
            break;

        case CENTER:
            if(setLineBreakFlag)
                m_textStartingXs[0] = computeCenteredTextStartingX(m_lineBreakInfos[0].maxWidth, m_rect.w, m_rect.x);
            else
                m_textStartingXs[0] = computeCenteredTextStartingX(m_text, m_font.size, m_rect.w, m_rect.x);
            break;

        default:
            utl::debug("Error in Control::setTextLayout xLayoutFlag");
            exit(1);
            break;
    }

    switch(yLayoutFlag)
    {
        case TOP_ALIGNED:
            m_textStartingYs[0] = m_rect.y + m_font.size - m_textEngine.getMaxDecenderHeight(m_font.size) + m_font.size * 0.05;
            break;

        case CENTER:
            if(setLineBreakFlag)
                m_textStartingYs[0] = computeCenteredTextStartingY(m_lineBreakInfos[0].lines * m_font.size, m_font.size,
                                                                   m_rect.h, m_rect.y + m_font.size);

            else
                m_textStartingYs[0] = computeCenteredTextStartingY(m_text, m_font.size, m_rect.h, m_rect.y + m_font.size);

            break;

        default:
            utl::debug("Error in Control::setTextLayout yLayoutFlag");
            exit(1);
            break;
    }
}


void Control::setRectTextLayout(LineBreakInfo& lineBreakInfo, float& startingX, float& startingY,
                                string text, float fontSize, Rect rect, bool setLineBreakFlag, int xLayoutFlag, int yLayoutFlag)
{
    if(setLineBreakFlag)
    {
        lineBreakInfo = m_textEngine.computeLineBreakInfo(text, fontSize, rect.w);
    }

    switch(xLayoutFlag)
    {
        case LEFT_ALIGNED:
            startingX = rect.x + fontSize * 0.05;
            break;

        case CENTER:
            if(setLineBreakFlag)
                startingX = computeCenteredTextStartingX(lineBreakInfo.maxWidth, rect.w, rect.x);
            else
                startingX = computeCenteredTextStartingX(text, fontSize, rect.w, rect.x);
            break;

        default:
            utl::debug("Error in Control::setTextLayout xLayoutFlag");
            exit(1);
            break;
    }


    switch(yLayoutFlag)
    {
        case TOP_ALIGNED:
            startingY = rect.y - fontSize + m_textEngine.getMaxDecenderHeight(fontSize);
            break;

        case CENTER:
            if(setLineBreakFlag)
                startingY = computeCenteredTextStartingY(lineBreakInfo.lines * fontSize, fontSize,
                                                                   rect.h, rect.y + fontSize);

            else
                startingY = computeCenteredTextStartingY(text, fontSize, rect.h, rect.y + fontSize);

            break;

        default:
            utl::debug("Error in Control::setTextLayout yLayoutFlag");
            exit(1);
            break;
    }
}

float Control::computeCenteredTextStartingX(float textWidth, float rectWidth, float offsetX)
{
    float diff = (rectWidth - textWidth)/2;
    return offsetX + diff;
}

float Control::computeCenteredTextStartingX(string text, float fontSize, float rectWidth, float offsetX)
{
    float w = m_textEngine.getTextWidth(text, fontSize);
    float diff = (rectWidth - w)/2;
    return offsetX + diff;
}

/// this computes the y axis baseline
float Control::computeCenteredTextStartingY(float textHeight, float fontSize, float rectHeight, float offsetY)
{
    float decenderHeight = m_textEngine.getMaxDecenderHeight(fontSize);
    float diff = (rectHeight - textHeight)/2;
    return offsetY - decenderHeight + diff;
}

float Control::computeCenteredTextStartingY(string text, float fontSize, float rectHeight, float offsetY)
{
    float decenderHeight = m_textEngine.getMaxDecenderHeight(fontSize);
    float diff = (rectHeight - fontSize)/2;
    return offsetY - decenderHeight + diff;
}


void Control::updatePipeline(Renderer* r)
{
    updatePipeline(r, m_rect);
}

void Control::updatePipeline(Renderer* r, Rect rect)
{
    m_pipeline.pushMatrix();
        m_pipeline.translate(rect.x, rect.y, 0);
        m_pipeline.scale(rect.w, rect.h, 1.0);
        r->loadUniformLocations(m_pipeline, RENDER_PASS1);
    m_pipeline.popMatrix();
}

void Control::customRender()
{

}


void Control::emptyOnClick()
{
    utl::debug("In " + m_text + " the empty OnClick Func");
}


/*
void Control::renderColored()
{
    render(&Control::r_coloredRectRenderer, m_rect);
}

void Control::renderColoredSingle()
{
    renderSingle(&Control::r_coloredRectRenderer, m_rect);
}

void Control::renderColored(Rect rect)
{
    render(&Control::r_coloredRectRenderer, rect);
}

void Control::renderColoredSingle(Rect rect)
{
    renderSingle(&Control::r_coloredRectRenderer, rect);
}



void Control::renderTextured()
{
    render(&Control::r_texturedRectRenderer, m_rect);
}

void Control::renderTexturedSingle()
{
    renderSingle(&Control::r_texturedRectRenderer, m_rect);
}

void Control::renderTextured(Rect rect)
{
    render(&Control::r_texturedRectRenderer, rect);
}

void Control::renderTexturedSingle(Rect rect)
{
    renderSingle(&Control::r_texturedRectRenderer, rect);
}
*/





