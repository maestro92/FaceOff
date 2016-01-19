#include "text_engine.h"


TextEngine::TextEngine()
{

}

TextEngine::TextEngine(string font, int fontPixelSize, int screenWidth, int screenHeight)
{

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // set m_GUIPipeline
    m_pipeline.setMatrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();
    m_pipeline.ortho(0, m_screenWidth, m_screenHeight, 0, -1, 1);

    m_pipeline.setMatrixMode(MODEL_MATRIX);
    m_pipeline.loadIdentity();

    m_quad = QuadModel(1,1);


    /// init freetype font
    string fontPath = "Assets/Fronts/" + font;

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        utl::debug("ERROR::FREETYPE: Could not init FreeType Library");

    FT_Face face;
    if (FT_New_Face(ft, "Assets/Fonts/arial.ttf", 0, &face))
        utl::debug("ERROR::FREETYPE: Failed to load font");

    m_initFontPixelSize = fontPixelSize;
    FT_Set_Pixel_Sizes(face, 0, fontPixelSize);
    if(FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        utl::debug("Could not load character 'X'\n");
    }

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    m_maxDescenderHeight = 0;

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use

        int height = face->glyph->bitmap.rows;
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, height),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        m_characters.insert(std::pair<GLchar, Character>(c, character));

        float descenderHeight = (character.size.y - character.bearing.y);
        m_maxDescenderHeight = max(m_maxDescenderHeight, descenderHeight);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    /// set shader
    Shader* s;

    /// r_TextRenderer
    s = new Shader("/gui_shaders/text.vs", "/gui_shaders/text.fs");
    r_textRenderer.addShader(s);
    r_textRenderer.addDataPair(RENDER_PASS1, "u_texture",   DP_INT);
    r_textRenderer.addDataPair(RENDER_PASS1, "u_color",     DP_VEC3);
}




float TextEngine::getTextWidth(string text, float fontPixelSize)
{
    float scale = fontPixelSize/m_initFontPixelSize;
    float width = 0;
    for (int i=0; i<text.size(); i++)
    {
        char c = text[i];
        Character ch = m_characters[c];

        float xpos = width + ch.bearing.x * scale;
        float w = ch.size.x * scale;

        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        width += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    return width;
}



float TextEngine::getMaxDecenderHeight(float fontPixelSize)
{
    float scale = fontPixelSize/m_initFontPixelSize;
    float result = m_maxDescenderHeight * scale;
    return result;
}


void TextEngine::render(string text, float x, float y, float fontPixelSize, glm::vec3 color)
{
    float scale = fontPixelSize/m_initFontPixelSize;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    r_textRenderer.enableShader();
        r_textRenderer.setData(RENDER_PASS1, "u_color", color);

        for (int i=0; i<text.size(); i++)
        {
            char c = text[i];
            Character ch = m_characters[c];

            float xpos = x + ch.bearing.x * scale;
            float ypos = y - ch.bearing.y * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            m_pipeline.pushMatrix();

                m_pipeline.translate(xpos, ypos, 0);
                m_pipeline.scale(w, h, 0);

                r_textRenderer.setData(RENDER_PASS1, "u_texture", 0, GL_TEXTURE_2D, ch.textureID);
                r_textRenderer.loadUniformLocations(m_pipeline, RENDER_PASS1);

                m_quad.render();

            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
            m_pipeline.popMatrix();
        }

    r_textRenderer.disableShader();
    glDisable(GL_BLEND);
}





void TextEngine::render(string text, float x, float y, float fontPixelSize, glm::vec3 color, vector<int> lineBreaks)
{
    float scale = fontPixelSize/m_initFontPixelSize;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float offsetY = y;
    float offsetX = x;
    int j = 0;

    r_textRenderer.enableShader();
        r_textRenderer.setData(RENDER_PASS1, "u_color", color);

        for (int i=0; i<text.size(); i++)
        {
            char c = text[i];
            Character ch = m_characters[c];

            if(j < lineBreaks.size() && i == lineBreaks[j])
            {
                offsetY += fontPixelSize;
                offsetX = x;
                j++;
                continue;
            }

            float xpos = offsetX + ch.bearing.x * scale;
            float ypos = offsetY - ch.bearing.y * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            m_pipeline.pushMatrix();

                m_pipeline.translate(xpos, ypos, 0);
                m_pipeline.scale(w, h, 0);

                r_textRenderer.setData(RENDER_PASS1, "u_texture", 0, GL_TEXTURE_2D, ch.textureID);
                r_textRenderer.loadUniformLocations(m_pipeline, RENDER_PASS1);

                m_quad.render();

            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            offsetX += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
            m_pipeline.popMatrix();
        }

    r_textRenderer.disableShader();
    glDisable(GL_BLEND);
}





void TextEngine::lineWrappedGreedyAlgo(string text)
{
    vector<string> words = getWords(text);

    int width = 20;
    int spaceLeft = width;
    int spaceWidth = 1;

    bool first = true;
    for(int i=0; i<words.size(); i++)
    {
        int size = words[i].size();

        if(size > spaceLeft)
        {
            cout << endl;
            spaceLeft = width - size;
            cout << words[i];
            first = false;
        }
        else
        {
            if(first)
            {
                cout << words[i];
                first = false;
            }
            else
                cout << " " << words[i];
            spaceLeft = spaceLeft - (size + spaceWidth);

        }
    }
    cout << endl;
}

void TextEngine::lineWrappedDynamicAlgo(string text)
{
    vector<string> words = getWords(text);
    int n = words.size();
    int lineWidth = 20;

    /// this is 2D cost map (really is diagnol)
    vector<vector<int>> lineCosts(n+1, vector<int>(n+1, INT_MAX));

    for(int y = 1; y <= n; y++)
    {
        int cost = lineWidth - words[y-1].size();
        lineCosts[y][y] = cost * cost * cost;
        for(int x = y+1; x <= n; x++)
        {
            cost -= (words[x-1].size() + 1);
            if(cost < 0)
                break;
            else
                lineCosts[y][x] = cost * cost * cost;
        }
    }

    lineCosts[n][n] = 0;
    utl::debug<int>("lineCosts", lineCosts);

    vector<int> startPositions(n+1, 0);

    vector<int> minCosts(n+1, 0);
    for(int y = 1; y <= n; y++)
    {
        minCosts[y] = INT_MAX;
        for(int x = 1; x <= y; x++)
        {
            /// if from a previous word to a current word
            /// the cost is INT_MAX, we skip
            if(lineCosts[x][y] == INT_MAX)
                continue;

            if(minCosts[x-1] + lineCosts[x][y] < minCosts[y])
            {
                minCosts[y] = minCosts[x-1] + lineCosts[x][y];
                startPositions[y] = x;
            }
        }
    }

    vector<int> endPositions(n+1, 0);

    for(int i = startPositions.size()-1; i > 0 ;i--)
    {
        int cur = i;
        int start = startPositions[i];
        while( start != i)
            i--;
        endPositions[i] = cur;
    }

    utl::debug<int>("endPositions", endPositions);

    for(int i = 0; i<words.size(); i++)
    {
        cout << words[i];
        int end = endPositions[i+1];
        while( (i + 1)<end)
        {
            i++;
            cout << " " << words[i] ;
        }
        cout << endl;
    }

}





void TextEngine::lineWrappedDynamicAlgo1(string text)
{
	/*
    vector<string> words = getWords(text);
    int n = words.size();
    int lineWidth = 6;
    int l[n];

    for(int i=0; i<words.size(); i++)
        l[i] = words[i].size();


    int INF = 100;
    // For simplicity, 1 extra space is used in all below arrays

    // extras[i][j] will have number of extra spaces if words from i
    // to j are put in a single line
    int extras[n+1][n+1];
    int lc[n+1][n+1];

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            extras[i][j] = INF;
            lc[i][j] = INF;
        }
    }


    // lc[i][j] will have cost of a line which has words from
    // i to j


    // c[i] will have total cost of optimal arrangement of words
    // from 1 to i
    int c[n+1];

    // p[] is used to print the solution.
    int p[n+1];

    int i, j;

    // calculate extra spaces in a single line.  The value extra[i][j]
    // indicates extra spaces if words from word number i to j are
    // placed in a single line
    // the extra -1 is for the "space"
    for (int y = 1; y <= n; y++)
    {
        extras[y][y] = lineWidth - l[y-1];
        for (int x = y+1; x <= n; x++)
            extras[y][x] = extras[y][x-1] - l[x-1] - 1;
    }


    for (i = 0; i <= n; i++)
    {
        for (j = 0; j <= n; j++)
            cout << extras[i][j] << "           ";
        cout << endl;
    }


    cout << endl << endl;

    // Calculate line cost corresponding to the above calculated extra
    // spaces. The value lc[i][j] indicates cost of putting words from
    // word number i to j in a single line
    for (int y = 1; y <= n; y++)
    {
        for (int x = y; x <= n; x++)
        {
            if (extras[y][x] < 0)
                lc[y][x] = INF;
            else if (x == n && extras[y][x] >= 0)
                lc[y][x] = 0;
            else
                lc[y][x] = extras[y][x]*extras[y][x];
        }
    }


    for (int y = 0; y <= n; y++)
    {
        for (int x = 0; x <= n; x++)
            cout << lc[y][x] << "           ";
        cout << endl;
    }


    // Calculate minimum cost and find minimum cost arrangement.
    //  The value c[j] indicates optimized cost to arrange words
    // from word number 1 to j.
    c[0] = 0;
    for (j = 1; j <= n; j++)
    {
        c[j] = INF;
        for (i = 1; i <= j; i++)
        {
//            if (c[i-1] != INF && lc[i][j] != INF && (c[i-1] + lc[i][j] < c[j]))
            if (lc[i][j] != INF && (c[i-1] + lc[i][j] < c[j]))
            {
                c[j] = c[i-1] + lc[i][j];
                p[j] = i;
            }
        }
    }


    for (int y = 0; y <= n; y++)
    {
        cout << p[y] << " ";
    }
    cout << endl;

//    printSolution(p, n);
*/
}





LineBreakInfo TextEngine::computeLineBreakInfo(string text, float fontPixelSize, int linePixelWidth)
{
    vector<string> words = getWords(text);
    int n = words.size();

    float spacePixelWidth = getTextWidth(" ", fontPixelSize);

    vector<vector<int>> lineCosts = computeLineCosts(words, fontPixelSize, n, linePixelWidth, spacePixelWidth);
    vector<int> startIndices = computeLineBreakStartIndices(lineCosts, n);

    vector<int> endPositions(n+1, 0);
    for(int i = startIndices.size()-1; i > 0 ;i--)
    {
        int cur = i;
        int start = startIndices[i];
        while( start != i)
            i--;
        endPositions[i] = cur;
    }

    /// compute LineBreakInfo
    vector<int> lineBreaks;
    int curLineWidth = 0, maxLineWidth = 0;
    int charIndex = 0;

    for(int i = 0; i<words.size(); i++)
    {
        charIndex += (words[i].size()+1);
        curLineWidth = getTextWidth(words[i], fontPixelSize);

        int end = endPositions[i+1];
        while( (i+1) < end)
        {
            i++;
            charIndex += (words[i].size()+1);
            curLineWidth += (spacePixelWidth + getTextWidth(words[i], fontPixelSize));

        }
        lineBreaks.push_back(charIndex-1);
        maxLineWidth = max(maxLineWidth, curLineWidth);
    }

    if(!lineBreaks.empty())
        lineBreaks.pop_back();

    return LineBreakInfo(lineBreaks.size()+1, lineBreaks, maxLineWidth);
}

vector<vector<int>> TextEngine::computeLineCosts(vector<string> words, float fontPixelSize, int n, int linePixelWidth, float spacePixelWidth)
{
    vector<vector<int>> lineCosts(n+1, vector<int>(n+1, INT_MAX));
    for(int y = 1; y <= n; y++)
    {
        float wordPixelWidth = getTextWidth(words[y-1], fontPixelSize);

        int cost = linePixelWidth - wordPixelWidth;
        lineCosts[y][y] = cost * cost * cost;

        for(int x = y+1; x <= n; x++)
        {
            wordPixelWidth = getTextWidth(words[x-1], fontPixelSize);

            cost -= (wordPixelWidth + spacePixelWidth);
            if(cost < 0)
                break;
            else
                lineCosts[y][x] = cost * cost * cost;
        }
    }
    lineCosts[n][n] = 0;
    return lineCosts;
}

vector<int> TextEngine::computeLineBreakStartIndices(vector<vector<int>> lineCosts, int n)
{
    vector<int> startIndices(n+1, 0);
    vector<int> minCosts(n+1, 0);
    for(int y = 1; y <= n; y++)
    {
        minCosts[y] = INT_MAX;
        for(int x = 1; x <= y; x++)
        {
            /// if from a previous word to a current word
            /// the cost is INT_MAX, we skip
            if(lineCosts[x][y] == INT_MAX)
                continue;

            if(minCosts[x-1] + lineCosts[x][y] < minCosts[y])
            {
                minCosts[y] = minCosts[x-1] + lineCosts[x][y];
                startIndices[y] = x;
            }
        }
    }
    return startIndices;
}


vector<string> TextEngine::getWords(string text)
{
    vector<string> words;

    bool inWord = false;
    int start = 0;
    for(int i=0; i<text.size(); i++)
    {
        char c = text[i];

        if(inWord)
        {
            if(c == ' ')
            {
                int size = i - start;
                string subString = text.substr(start, size);
                inWord = false;
                words.push_back(subString);
            }
        }
        else
        {
            if(c != ' ')
            {
                start = i;
                inWord = true;
            }
        }
    }

    if(inWord)
    {
        int size = text.size() - start;
        string subString = text.substr(start, size);
        words.push_back(subString);
    }
    return words;
}




int TextEngine::printSolution (int p[], int n)
{
    int k;
    if (p[n] == 1)
        k = 1;
    else
        k = printSolution (p, p[n]-1) + 1;
    cout << "Line number " << k << ": From word no. " << p[n] << " to " << n << endl;
    return k;
}

/*
void TextEngine::render(string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
 //   glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    r_textRenderer.enableShader();
        r_textRenderer.setData(RENDER_PASS1, "u_color", color);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // Iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = m_characters[*c];

            GLfloat xpos = x + ch.bearing.x * scale;
            GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

            GLfloat w = ch.size.x * scale;
            GLfloat h = ch.size.y * scale;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
            };

            r_textRenderer.setData(RENDER_PASS1, "u_texture", 0, GL_TEXTURE0, ch.textureID);
            r_textRenderer.loadUniformLocations(m_pipeline, RENDER_PASS1);



            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

    r_textRenderer.disableShader();

    glDisable(GL_BLEND);
}
*/
