#ifndef TEXT_ENGINE_H_
#define TEXT_ENGINE_H_

#include <unordered_map>

#include "utility\utility_debug.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "quad_model.h"
#include "renderer.h"

#include "gui_renderer_constants.h"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character
{
    GLuint textureID;   // ID handle of the glyph texture
    glm::ivec2 size;    // Size of glyph
    glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
    GLuint advance;    // Horizontal offset to advance to next glyph
};

struct Font
{
    glm::vec3 color;
    float size;
};

struct LineBreakInfo
{
    int lines;
    vector<int> lineBreaks;
    int maxWidth;
    int maxHeight;

    LineBreakInfo()
    {}

    LineBreakInfo(int l, vector<int> lb, int mw)
    {
        lines = l;
        lineBreaks = lb;
        maxWidth = mw;
    }
};


class TextEngine
{
    public:
        TextEngine();

        TextEngine(string font, int fontPixelSize, int screenWidth, int screenHeight);

        float getTextWidth(string text, float fontPixelSize);
        float getMaxDecenderHeight(float fontPixelSize);

        LineBreakInfo computeLineBreakInfo(string text, float fontPixelSize, int linePixelWidth);
        void render(string text, float x, float y, float fontPixelSize, glm::vec3 color);
        void render(string text, float x, float y, float fontPixelSize, glm::vec3 color, vector<int> lineBreaks);

        static vector<string> getWords(string text);
        vector<vector<int>> computeLineCosts(vector<string> words, float fontPixelSize, int n, int linePixelWidth, float spacePixelWidth);
        vector<int> computeLineBreakStartIndices(vector<vector<int>> lineCosts, int n);

        void lineWrappedGreedyAlgo(string text);
        void lineWrappedDynamicAlgo(string text);
        void lineWrappedDynamicAlgo1(string text);


        int printSolution (int p[], int n);




        unordered_map<GLchar, Character> m_characters;
        float m_globalYMax;
        float m_globalYMin;

        int m_initFontPixelSize;

		Renderer r_textRenderer;

    private:
        float m_maxDescenderHeight;
        QuadModel m_quad;
        Pipeline m_pipeline;

        int m_screenWidth;
        int m_screenHeight;



};


#endif


