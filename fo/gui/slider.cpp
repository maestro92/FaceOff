#include "Slider.h"

const int tickSize = 5;

Slider::Slider() : Slider("Martin :)", 0, 0, 100, 100, BLUE, 0, 10)
{

}

Slider::Slider(string text, int x, int y, int width, int height,
                     glm::vec3 color, float min, float max) : Control(text, x, y, width, height, color)
{
    m_defaultValue = 0.0f;
    m_current = NULL;
    m_maxValue = max;
    m_minValue = min;
    m_dragging = false;
    m_valueType = FLOAT_TYPE;

    m_sliderQuadRect.set(m_rect.x, m_rect.y, tickSize, m_rect.h);
}


void Slider::setColors(glm::vec3 rectColor, glm::vec3 sliderColor)
{
    m_rectColor = rectColor;
    m_sliderColor = sliderColor;
}

void Slider::setMaxValue(float max)
{
    m_maxValue = max;
}

void Slider::setMinValue(float min)
{
    m_minValue = min;
}

void Slider::setDefaultValue(float value)
{
    m_defaultValue = value;
}

void Slider::setValueType(int t)
{
    m_valueType = t;
}

void Slider::setValue(float *value)
{
    m_current = value;
    if (m_current != NULL)
        m_defaultValue = *m_current;
}

float Slider::getValue()
{
    return *m_current;
}

bool Slider::update(MouseState &state)
{
    Control::update(state);

    int x = state.m_pos.x;
    int y = state.m_pos.y;

    if(m_isInside == true)
    {
        if(state.m_leftButtonDown)
        {
            m_dragging = true;
        }

        if(state.m_rightButtonDown)
        {
            *m_current = m_defaultValue;
        }
    }

    if( !(state.m_leftButtonDown ))
        m_dragging = false;

    if( m_dragging )
    {
        *m_current = (float)(x - m_rect.x) * (m_maxValue - m_minValue)/ (float)m_rect.w + m_minValue;

        if(*m_current > m_maxValue)
            *m_current = m_maxValue;
        else if( *m_current < m_minValue )
            *m_current = m_minValue;
    }

    return m_dragging;
}




bool Slider::update(MouseState& state, unsigned int& groupFlag)
{
    if(groupFlag & ( 1 << m_id) || (groupFlag==0) )
    {
        bool flag = update(state);
//        cout << "Flag is " << flag << endl;

        if(flag == true)
        {
            groupFlag = groupFlag | ( 1 << m_id);
         //   Utility::debug()
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
    return false;

    /*
    if(groupFlag & ( 1 << m_id))
    {
        return false;
    }

    bool flag = update(state);
    if(flag)
    {
        groupFlag = groupFlag | ( 1 << m_id);
     //   Utility::debug()
        std::bitset<32> x(groupFlag);
        cout << x << endl;
    }
    else
    {
        groupFlag = groupFlag & (~( 1 << m_id));
        std::bitset<32> x(groupFlag);
        cout << x << endl;
    }
    return flag;
    */
}




bool Slider::update1(MouseState & state, unsigned int& groupFlag)
{
    if(groupFlag & ( 1 << m_id) || (groupFlag==0) )
    {
        bool flag = update(state);
//        cout << "Flag is " << flag << endl;

        if(flag == true)
        {
            groupFlag = groupFlag | ( 1 << m_id);
         //   Utility::debug()
      //      std::bitset<32> x(groupFlag);
     //       cout << x << endl;
        }
        else
        {
            groupFlag = groupFlag & (~( 1 << m_id));
     //       std::bitset<32> x(groupFlag);
       //     cout << x << endl;
        }
        return flag;
    }
    return false;
}


void Slider::render(Pipeline& p,
                    Renderer* r,
                    int RenderPassID)
{
    /*
    Control::render(m_pipeline, Renderer, RENDER_PASS1, m_rect, p_model);

    int offset_x = (int)((*m_current - m_minValue) / (m_maxValue - m_minValue) * (m_rect.w - tickSize) + m_rect.x);
    int offset_y = 0;

  //  Utility::debug("offset_x", offset_x);

    m_sliderQuadRect.x = offset_x;

  //  glDisable(GL_BLEND);
    p_model = &m_slider;
    Control::render(m_pipeline,
                        Renderer,
                        RENDER_PASS1, m_sliderQuadRect, p_model);



 //   glEnable(GL_BLEND);

 //   m_label = "NICE";
 //   offset_x = computeTextStartingX(m_label);
 //   offset_y = computeTextStartingY();
    offset_x = m_rect.x + 2;
    offset_y = m_rect.y + m_rect.h - 15 - 5;


    string s = "";

    if(m_valueType == INT_TYPE)
    {
        int count = (int)(*m_current);
        stringstream ss;
        ss << count;
        string cs = ss.str();
        s = m_text + ": " + cs;
    }
    else
    {
        float num = *m_current;
        ostringstream buff;
        buff << num;
        string cs = buff.str();
        s = m_text + ": " + cs;
    }

    // offset_y = m_rect.y + m_rect.h - Control::m_textEngine.fontSize - 2;
    Control::m_textEngine.render(m_pipeline, offset_x, offset_y, 15, s.c_str());
    */
}


void Slider::render()
{

}

int Slider::getType()
{
    return SLIDER;
}


bool Slider::getDraggingFlag()
{
    return m_dragging;
}
