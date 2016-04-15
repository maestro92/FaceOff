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

#include "custom_gui/bar.h"

class GUIManager
{
    public:
        void init(int screenWidth, int screenHeight);

		void initGUIRenderingSetup();
		void renderTextureFullScreen(GLuint textureId);
		void renderTextureFullScreen(GLuint textureId, GLuint fboTarget);
		void renderTexture(GLuint textureId, int x, int y, int width, int height);
		void renderTexture(GLuint textureId, GLuint fboTarget, int x, int y, int width, int height);
		void renderTexture(GLuint textureId, GLuint fboTarget, Rect rect);

		void renderTextureSingle(GLuint textureId, int x, int y, int width, int height);
		void renderTextureSingle(GLuint textureId, GLuint fboTarget, int x, int y, int width, int height);
		void renderTextureSingle(GLuint textureId, GLuint fboTarget, Rect rect);

        void renderGUI();

        Renderer  r_textureRenderer;

        void updateAndRender(MouseState mouseState);
        void renderGUIComponents();
        void addGUIComponent(Control* control);

    private:
        int m_GUIComponentsID;
        int m_GUIComponentsFlags;
        vector<Control*> m_GUIComponents;

        QuadModel m_textureQuad;
        Pipeline m_GUIPipeline;
        int m_screenWidth;
        int m_screenHeight;
};

#endif
