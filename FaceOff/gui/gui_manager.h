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

#include "gui_renderer_constants.h"
#include "custom_gui/bar.h"
#include "renderer_constants.h"

class GUIManager
{
    public:
        void init(int screenWidth, int screenHeight);

		void initGUIRenderingSetup();
		void setupRenderToScreen(int x, int y, int width, int height);
		void renderTextureFullScreen(GLuint textureId);
		void renderTextureFullScreen(GLuint textureId, GLuint fboTarget);
		void renderTexture(GLuint textureId, int x, int y, int width, int height);
		void renderTexture(GLuint textureId, GLuint fboTarget, int x, int y, int width, int height);
		void renderTexture(GLuint textureId, GLuint fboTarget, Rect rect);

		void renderSnipeScopeView(GLuint sceneTextureId);
		/*
		void renderTextureSingle(GLuint textureId, int x, int y, int width, int height);
		void renderTextureSingle(GLuint textureId, GLuint fboTarget, int x, int y, int width, int height);
		void renderTextureSingle(GLuint textureId, GLuint fboTarget, Rect rect);
		*/


		void setHorAimIndex(int index);
		void setVerAimIndex(int index);
		void setSniperZoomMode(bool b);

		void initRenderer(const Array arr, Renderer* r, string name);
		Object findRendererObject(const Array arr, string name);
		void initRenderer(const Object obj, Renderer* r);



        void renderGUI();

		Renderer r_texture;
		Renderer r_coloredRect;
		Renderer r_texturedRect;
		Renderer r_listBoxItemHighlight;
		Renderer r_sniperScopeView;

		GLuint m_sniperScopeViewTextureId;

        void updateAndRender(MouseState mouseState);
        void renderGUIComponents();
        void addGUIComponent(Control* control);
		int getNumGUIComponent();

    private:
        int m_GUIComponentsID;
        int m_GUIComponentsFlags;
        vector<Control*> m_GUIComponents;

		bool m_sniperZoomMode;
		int m_horAimIndex;
		int m_verAimIndex;

        QuadModel m_textureQuad;
        Pipeline m_GUIPipeline;
        int m_screenWidth;
        int m_screenHeight;
};

#endif
