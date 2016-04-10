#include "list_box.h"



ListBox::ListBox()
{
}

ListBox::ListBox(string text, int x, int y, int width, int height,
                 glm::vec3 color, glm::vec3 itemRectColor, int colNum, std::function<void()> callBack) :
                Control(text, x, y, width, height, color)
{
    m_colNum = colNum;

    m_curIndex = 0;
    m_curIndexX = 0;
    m_curIndexY = 0;

    m_curRowNum = 0;
    m_curColNum = 0;

    m_itemWidth = (float)((float)width / (float)m_colNum);
    m_itemHeight = 75;

    setColors(color, itemRectColor);
    m_funcCallBack = callBack;
}

/*
void ListBox::setContent(vector<GOLModel*> models)
{
    int count = models.size();
    for(int i=0; i<count; i++)
    {
        addItem(models[i]->getName(), glm::vec2(models[i]->m_thumbnailWidth, models[i]->m_thumbnailHeight), WHITE, models[i]->getThumbnailTexture());
    }
}
*/

void ListBox::addItem(string text, glm::vec2 textureSize, glm::vec3 color, GLuint texID)
{
    int index = m_items.size();

    int offset_x = m_rect.x + m_curColNum * m_itemWidth;
    int offset_y = m_rect.y + m_curRowNum * m_itemHeight;

    Rect itemRect(offset_x, offset_y, m_itemWidth, m_itemHeight);

    Rect modelRect;
    modelRect.x = itemRect.x + (itemRect.w - textureSize.x) * 0.5;
    modelRect.y = itemRect.y + (itemRect.h - textureSize.y) * 0.33;
    modelRect.w = textureSize.x;
    modelRect.h = textureSize.y;


    ListBoxItem item(text, textureSize, itemRect, modelRect, color, texID);
    m_items.push_back(item);


    if(m_curColNum == m_colNum-1)
    {
        m_curRowNum++;
        m_curColNum = 0;
    }
    else
        m_curColNum++;
}



void ListBox::removeItem(int index)
{
/// vector for loop vs iterator
/// http://stackoverflow.com/questions/131241/why-use-iterators-instead-of-array-indices

    int i=0;
    for(vector<ListBoxItem>::iterator it = m_items.begin(); it != m_items.end(); it++)
    {
        if(i==index)
        {
            m_items.erase(it);
            break;
        }
        i++;
    }

    /// if the last item is the item being removed

    if(index >= (int) m_items.size())
        m_curIndex = (int)m_items.size() - 1;
}

void ListBox::setItemFont(float fontSize, glm::vec3 color)
{
    m_itemFont.size = fontSize;
    m_itemFont.color = color;
}




/*
void ListBox::setItemsTextLayout(ListBoxItem& item, int xLayoutFlag, int yLayoutFlag)
{

}
*/

void ListBox::setItemsTextLayout(int xLayoutFlag, int yLayoutFlag)
{
    for(int i=0; i<m_items.size(); i++)
    {
        string text = m_items[i].text;
        Rect rect = m_items[i].rect;
        rect.y += (rect.h / 2);
        rect.h = rect.h / 2;
        m_items[i].lineBreakInfo = m_textEngine.computeLineBreakInfo(text, m_itemFont.size, rect.w);

        switch(xLayoutFlag)
        {
            case LEFT_ALIGNED:
                m_items[i].textStartingX = rect.x + m_itemFont.size * 0.05;
                break;

            case CENTER:
                m_items[i].textStartingX = computeCenteredTextStartingX(m_items[i].lineBreakInfo.maxWidth, rect.w, rect.x);
                break;

            default:
                utl::debug("Error in Control::setTextLayout xLayoutFlag");
                exit(1);
                break;
        }

        switch(yLayoutFlag)
        {
            case TOP_ALIGNED:
                m_items[i].textStartingY = rect.y - m_itemFont.size + m_textEngine.getMaxDecenderHeight(m_itemFont.size);
                break;

            case CENTER:
                m_items[i].textStartingY = computeCenteredTextStartingY(m_items[i].lineBreakInfo.lines * m_itemFont.size, m_itemFont.size,
                                                                       rect.h, rect.y + m_itemFont.size);
                break;

            default:
                utl::debug("Error in Control::setTextLayout yLayoutFlag");
                exit(1);
                break;
        }
    }
}

/*
void ListBox::removeItem(int index)
{
/// vector for loop vs iterator
/// http://stackoverflow.com/questions/131241/why-use-iterators-instead-of-array-indices

    int i=0;
    for(vector<string>::iterator it = m_items.begin(); it != m_items.end(); it++)
    {
        if(i==index)
        {
            m_items.erase(it);
            break;
        }
        i++;
    }

    /// if the last item is the item being removed

    if(index >= (int) m_items.size())
        m_curIndex = (int)m_items.size() - 1;
}
*/

void ListBox::setCurrent(int index)
{
    m_curIndex = index;
}

int ListBox::getIndex()
{
    return m_curIndex;
}

int ListBox::getCount()
{
    return (int)m_items.size();
}

void ListBox::setColors(glm::vec3 rectColor, glm::vec3 itemRectColor)
{
    m_rectColor = rectColor;
    m_itemRectColor = itemRectColor;
}

bool ListBox::update(MouseState & state)
{
    Control::update(state);

    int x = state.m_pos.x;
    int y = Control::m_screenHeight - state.m_pos.y;

    if( m_isInside && state.m_leftButtonDown)
    {
        int x_index = (x - m_rect.x) / m_itemWidth;
        int y_index = (y - m_rect.y) / m_itemHeight;

        bool bx = x_index >= 0 && x_index < m_colNum;
        bool by = y_index >= 0 && y_index < ( (int)m_items.size() / m_colNum) ;


        if( bx && by )
        {
            m_curIndex = y_index * m_colNum + x_index;

            m_curIndexX = x_index;
            m_curIndexY = y_index;
            m_funcCallBack();
            return true;
        }
    }
    return false;
}


void ListBox::render()
{

	Renderer* r = &Control::r_texturedRectRenderer;
	Renderer* hlr = &Control::r_listBoxHighlightRenderer;
	for (int y = 0; y < m_curRowNum; y++)
	{
		for (int x = 0; x < m_colNum; x++)
		{
			int i = y * m_colNum + x;

			if (i == m_items.size())
				break;

			/*
			// render texture
			int offset_x = m_rect.x + x * m_itemWidth;
			int offset_y = m_rect.y + y * m_itemHeight;

			Rect itemRect(offset_x, offset_y, m_itemWidth, m_itemHeight);
			*/
			Rect itemRect = m_items[i].rect;
			if (i == m_curIndex && m_curIndex >= 0)
			{
				float gap = 2;
				hlr->enableShader();
				hlr->setData(RENDER_PASS1, "u_x1", itemRect.x + gap);
				hlr->setData(RENDER_PASS1, "u_x2", itemRect.x + itemRect.w - gap);

				hlr->setData(RENDER_PASS1, "u_y1", Control::toGUICoord(itemRect.y + itemRect.h - gap));
				hlr->setData(RENDER_PASS1, "u_y2", Control::toGUICoord(itemRect.y + gap));

				updatePipeline(hlr, itemRect);
				m_quadModel.render();
				hlr->disableShader();
			}

			r->enableShader();
			Rect tempRect = m_items[i].modelRect;

			r->setData(RENDER_PASS1, "u_texture", 0, GL_TEXTURE_2D, m_items[i].textureID);
			updatePipeline(r, tempRect);
			m_quadModel.render();

			r->disableShader();

			// render text
			Control::m_textEngine.render(m_items[i].text, m_items[i].textStartingX, m_items[i].textStartingY,
				m_itemFont.size, m_itemFont.color, m_items[i].lineBreakInfo.lineBreaks);
		}
	}

    /*
    Control::r_coloredRectRenderer.enableShader();
        Control::r_coloredRectRenderer.setData(RENDER_PASS1, "u_color", m_rectColor);
        updatePipeline(&Control::r_coloredRectRenderer);
        m_quadModel.render();

        // render the itemRectBox
        if( m_curIndex >= 0)
        {
            int offset_x = m_rect.x + m_curIndexX * m_itemWidth;
            int offset_y = m_rect.y + m_rect.h - ((m_curIndexY + 1) * m_itemHeight);

            Rect itemRect(offset_x, offset_y, m_itemWidth, m_itemHeight);
            Control::r_coloredRectRenderer.setData(RENDER_PASS1, "u_color", m_itemRectColor);
            updatePipeline(&Control::r_coloredRectRenderer, itemRect);
            m_quadModel.render();
        }

    Control::r_coloredRectRenderer.disableShader();


    for(int y = 0; y < m_curRowNum; y++)
    {
        for(int x = 0; x < m_colNum; x++)
        {
            int index = y * m_colNum + x;
            if( index == m_items.size())
            {
                break;
            }
            else
            {
                int offset_x = m_rect.x + x * m_itemWidth;
                int offset_y = m_rect.y + m_rect.h - ((y + 1) * m_itemHeight) + 10;

                Control::m_textEngine.render(m_items[index].m_text, offset_x, offset_y, 0.4f, glm::vec3(0.5, 0.8f, 0.2f));
            }
        }
    }
    */
}



/*
void ListBox::customRender()
{
    Renderer* r = &Control::r_texturedRectRenderer;
    Renderer* hlr = &Control::r_listBoxHighlightRenderer;

        for(int y = 0; y < m_curRowNum; y++)
        {
            for(int x = 0; x < m_colNum; x++)
            {
                int index = y * m_colNum + x;

                if( index == m_items.size())
                    break;

                int offset_x = m_rect.x + x * m_itemWidth;
                int offset_y = m_rect.y + m_rect.h - ((y + 1) * m_itemHeight);
                Rect itemRect(offset_x, offset_y, m_itemWidth, m_itemHeight);


                if (index == m_curIndex && m_curIndex >= 0)
                {
                    float gap = 2;
                    hlr->enableShader();
                        hlr->setData(RENDER_PASS1, "u_x1", itemRect.x + gap);
                        hlr->setData(RENDER_PASS1, "u_x2", itemRect.x + itemRect.w - gap);

                        hlr->setData(RENDER_PASS1, "u_y1", itemRect.y + gap);
                        hlr->setData(RENDER_PASS1, "u_y2", itemRect.y + itemRect.h - gap);

                    updatePipeline(hlr, itemRect);
                    m_quadModel.render();
                    hlr->disableShader();
                }

                r->enableShader();
                    Rect tempRect = getListBoxItemModelRect(itemRect, index);

                    r->setData(RENDER_PASS1, "u_texture", 0, GL_TEXTURE0, m_items[index].m_textureID);
                    updatePipeline(r, tempRect);
                    m_quadModel.render();

                r->disableShader();

                // render text
                offset_x = m_rect.x + x * m_itemWidth;
                offset_y = m_rect.y + m_rect.h - ((y + 1) * m_itemHeight) + 10;

                Control::m_textEngine.render(m_items[index].m_text, offset_x, offset_y, 0.4f, glm::vec3(0.5, 0.8f, 0.2f));
            }h
        }
}
*/


int ListBox::getType()
{
    return LIST_BOX;
}

