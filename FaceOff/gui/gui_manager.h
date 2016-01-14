#ifndef GUI_MANAGER_H_
#define GUI_MANAGER_H_

#include "pipeline.h"
#include "utility.h"
#include "Rect.h"
#include "Slider.h"
#include "list_box.h"
#include "button.h"
#include "gol_model.h"
#include "control.h"
#include "renderer.h"

class GUIManager
{
    public:
        void init(int paletteX, int paletteY,
                  int paletteWidth, int paletteHeight,
                  int screenWidth, int screenHeight);
        GLuint getGUIPaletteTexture();



        Rect m_paletteRect;
        void initGUIRenderingSetup();


        int getGOLModelListBoxIndex();


        void renderTextureFullScreen(GLuint TextureId);
        void renderTextureFullScreen(GLuint TextrureId, GLuint FboTarget);
        void renderTexture(GLuint TextureId, int x, int y, int width, int height);
        void renderTexture(GLuint TextureId, GLuint FboTarget, int x, int y, int width, int height);
        void renderTexture(GLuint TextureId, GLuint FboTarget, Rect rect);

        void renderTextureSingle(GLuint TextureId, int x, int y, int width, int height);
        void renderTextureSingle(GLuint TextureId, GLuint FboTarget, int x, int y, int width, int height);
        void renderTextureSingle(GLuint TextureId, GLuint FboTarget, Rect rect);

        void renderGUI();

        Renderer  r_textureRenderer;

        void updateAndRender(MouseState mouseState);
        void renderGUIComponents();
        void addGUIComponent(Control* control);

    private:
        int m_GUIComponentsID;
        int m_GUIComponentsFlags;
        vector<Control*> m_GUIComponents;
        ListBox* m_GOLModelListBox;

        QuadModel m_textureQuad;
        GLuint m_GUIPaletteTexture;
        Pipeline m_GUIPipeline;
        int m_screenWidth;
        int m_screenHeight;
};

#endif
