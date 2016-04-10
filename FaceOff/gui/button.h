#ifndef BUTTON_H_
#define BUTTON_H_

#include "control.h"
#include <functional>
class Button : public Control
{
    public:
        Button();
        Button(string text, int x, int y, int width, int height,
               glm::vec3 bgColor, glm::vec3 highlightColor, glm::vec3 pressedColor,
               std::function<void()> callBack);


        /// http://stackoverflow.com/questions/4271245/why-do-i-get-no-matching-function-when-i-inherit-this-function
        using Control::update;

        virtual bool update(MouseState & state);
        virtual bool update(MouseState & state, unsigned int& groupFlag);

        void setTextures(GLuint bgTexId, GLuint highlightTexId, GLuint pressedTexId);
        void setColors(glm::vec3 bgColor, glm::vec3 highlightColor, glm::vec3 pressedColor);


        void render();
    //    void customRender();

        virtual int getType();


    protected:
        bool m_down;

        glm::vec3 m_highlightColor;
        glm::vec3 m_pressedColor;


        GLuint m_highlightTexture;
        GLuint m_pressedTexture;
        /// have three textures for it


};




#endif // EG_BUTTON_H


/*

class Button : public Control
{
    public:
        Button();
        Button(string text, int x, int y, int width, int height, FuncCallBack funcCallBack);
        Button(string text, int x, int y, int width, int height, FuncCallBack funcCallBack,
               glm::vec3 bgColor, glm::vec3 highlightColor, glm::vec3 pressedColor);

        /// http://stackoverflow.com/questions/4271245/why-do-i-get-no-matching-function-when-i-inherit-this-function
        using Control::update;

        virtual bool update(MouseState & state);
        virtual bool update(MouseState & state, unsigned int& groupFlag);

        void setTextures(GLuint bgTexId, GLuint highlightTexId, GLuint pressedTexId);
        void setColors(glm::vec3 bgColor, glm::vec3 highlightColor, glm::vec3 pressedColor);
        void setFuncCallBack(FuncCallBack funcCallBack);

        void render();
        void customRender();

        virtual int getType();

    protected:
        bool m_down;

        glm::vec3 m_highlightColor;
        glm::vec3 m_pressedColor;
        bool m_funcCalled;
        FuncCallBack m_funcCallBack;

        GLuint m_highlightTexture;
        GLuint m_pressedTexture;
        /// have three textures for it

};

*/
