#ifndef LIST_BOX_H
#define LIST_BOX_H

#include <vector>
#include <gol_model.h>
#include "Rect.h"
#include "Control.h"

using namespace std;


struct ListBoxItem
{
    string text;
    glm::vec2 textureSize;
    glm::vec3 color;
    Rect rect;
    Rect modelRect;
    GLuint textureID;
    LineBreakInfo lineBreakInfo;
    float textStartingX;
    float textStartingY;

    ListBoxItem()
    { }

    ListBoxItem(string t, glm::vec2 texSize, Rect r, Rect mr, glm::vec3 c, GLuint tex)
    {
        text = t;
        textureSize = texSize;
        rect = r;
        modelRect = mr;
        color = c;
        textureID = tex;
    }
};

class ListBox : public Control
{
    public:
        ListBox();
        ListBox(string text, int x, int y, int width, int height,
                glm::vec3 color, glm::vec3 itemRectColor,
                int colNum, std::function<void()> callBack);

     //   void setContent(vector<GOLModel*> models);

        void addItem(string text, glm::vec2 textureSize, glm::vec3 color, GLuint texID);

        void removeItem(int index);
        void setCurrent(int index);


        void setColors(glm::vec3 rectColor, glm::vec3 itemRectColor);
        void setItemFont(float fontSize, glm::vec3 color);

        // void setItemTextLayout(ListBoxItem& item, int xLayoutFlag, int yLayoutFlag);
        void setItemsTextLayout(int xLayoutFlag, int yLayoutFlag);


        int getIndex();
        int getCount();

        virtual bool update(MouseState & state);

//        void render (Pipeline& p, Renderer* r);
        // virtual void renderTextured();
        void render();

        virtual int getType();
        vector<ListBoxItem> m_items;

    protected:
        int m_curIndex;
        int m_curIndexX;
        int m_curIndexY;

        int m_colNum;

        int m_curRowNum;
        int m_curColNum;

        float m_itemWidth;
        float m_itemHeight;

        glm::vec3 m_itemRectColor;
        Font m_itemFont;
};




#endif // EG_BUTTON_H
