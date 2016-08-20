#include "utility.h"


void utl::initGLEW()
{
    // initialize Glew

	utl::debug("initGLEW");
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      cout << "Error: %s\n" << glewGetErrorString(err) << endl;
    }
	utl::debug("finishing GLEW");
}


GLuint utl::createFBO()
{
    GLuint FBO;
    glGenFramebuffers(1, &FBO);
    return FBO;
}


void utl::bindFBO(GLuint target)
{


}


void utl::errorCheckFBO()
{
    int i=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i!=GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "XXXXXXXXXX Error: Framebuffer is not OK, status=" << i << endl;
	}
    else
        cout << "Framebuffer Okay" << endl;
}




GLuint utl::loadTexture(string filename, bool mipmapFlag)
{
	if (mipmapFlag)
		return loadTexture(filename, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP, mipmapFlag);
	else
		return loadTexture(filename, GL_LINEAR, GL_CLAMP, mipmapFlag);
}


GLuint utl::loadTexture(string filename, GLuint filteringParam, GLuint edgeParam, bool mipmapFlag)
{
    // cout << "Loading Texture " << filename << endl;

    SDL_Surface* img2 = loadSDLImage(filename);

    unsigned int num;
    glGenTextures(1,&num);

    // tell OpenGL we want to use this texture
    glBindTexture(GL_TEXTURE_2D,num);       //and use the texture, we have just generated
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img2->w, img2->h, 0, GL_RGBA,GL_UNSIGNED_INT_8_8_8_8, img2->pixels);        //we make the actual texture

	setTexture2DParams(GL_TEXTURE_2D, filteringParam, edgeParam, mipmapFlag);

    //we delete the image, we don't need it anymore
    glBindTexture(GL_TEXTURE_2D,0);
    SDL_FreeSurface(img2);
    return num;
}




// http://stackoverflow.com/questions/8767166/passing-a-2d-array-to-a-c-function
GLuint utl::loadTexture(vector<vector<vector<GLubyte>>> data, GLuint filteringParam, GLuint edgeParam, bool mipmapFlag)
{
    int h = data.size();
    int w = data[0].size();

	if (h <= 0 || w <= 0)
		return -1;

	GLubyte*** temp = new GLubyte**[h];
	for (int y = 0; y < h; y++)
	{
		temp[y] = new GLubyte*[w];
		for (int x = 0; x < w; x++)
			temp[y][x] = new GLubyte[4];
	}

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            temp[y][x][0] = (data[y][x][0]);
            temp[y][x][1] = (data[y][x][1]);
            temp[y][x][2] = (data[y][x][2]);
            temp[y][x][3] = (data[y][x][3]);
        }
    }

    unsigned int num;
    glGenTextures(1,&num);

    // tell OpenGL we want to use this texture
    glBindTexture(GL_TEXTURE_2D,num);       //and use the texture, we have just generated
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &temp[0][0][0]);        //we make the actual texture

	setTexture2DParams(GL_TEXTURE_2D, filteringParam, edgeParam, mipmapFlag);

    //we delete the image, we don't need it anymore
    glBindTexture(GL_TEXTURE_2D,0);
    return num;
}



GLuint utl::createNoiseTexture(int w, int h, int min, int max)
{
	return -1;
}


void utl::setTexture2DParams(GLuint target, GLuint filteringParam, GLuint edgeParam, bool mipmapFlag)
{
	if (mipmapFlag)
	{
		if (filteringParam != GL_NEAREST_MIPMAP_NEAREST &&
			filteringParam != GL_LINEAR_MIPMAP_NEAREST &&
			filteringParam != GL_NEAREST_MIPMAP_LINEAR &&
			filteringParam != GL_LINEAR_MIPMAP_LINEAR)
		{
			cout << "Wrong mipmap Texture filtering setting when loading" << endl;
			exit(1);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else

	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filteringParam);

	// if you comment these two lines out, you will see the edges of the cube
	glTexParameteri(target, GL_TEXTURE_WRAP_S, edgeParam);      //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
	glTexParameteri(target, GL_TEXTURE_WRAP_T, edgeParam);      //we do it for vertically and horizontally (previous line)
}





GLuint utl::createNewTexture(int w, int h)
{
    GLuint textureID;
    glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);

    setTextureParameters(w, h, GL_RGBA8, GL_RGBA);
    errorCheck();

    // unbind the texture
    glBindTexture(GL_TEXTURE_2D,0);
    return textureID;
}



TextureDataBuffer utl::createEmptyBuffer(int w, int h)
{
    TextureDataBuffer dataBuffer;
    dataBuffer.resize(h);
    for(int i=0; i<h; i++)
    {
        dataBuffer[i].resize(w);
        for(int j=0; j<w; j++)
            dataBuffer[i][j].resize(4);
    }
    return dataBuffer;
}



GLuint utl::createNewDepthTexture(int w, int h)
{
    GLuint textureID;

    glGenTextures(1,&textureID);
    /// glBindTexture: i'm binding this texture object as the current 2D texture
	glBindTexture(GL_TEXTURE_2D,textureID);

    setTextureParameters(w, h, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    errorCheck();

    // unbind the texture
    glBindTexture(GL_TEXTURE_2D,0);
    return textureID;
}



void utl::setTextureParameters(int w, int h, int internal_format, int format)
{
   	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}



GLuint utl::createNewCubemapTexture()
{
    /// need to pass in the pictures in the following order
    /// left, right, top, bottom, near, far
    GLuint tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    // set textures
    /// SCREEN_WIDTH and SCREEN_HEIGHT needs to be power of two
    for (int i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    /// this all applies to the one cubemap
    setCubemapTextureParameters();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return tex;
}



GLuint utl::loadCubemapTexture(string* filenames)
{
    /// need to pass in the pictures in the following order
    /// left, right, top, bottom, near, far
    GLuint tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    for(int i=0; i<6; i++)
    {
        SDL_Surface* cubemap_img = loadSDLImage(filenames[i]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,
                        GL_RGBA, cubemap_img->w, cubemap_img->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, cubemap_img->pixels);

        SDL_FreeSurface(cubemap_img);
    }

    /// this all applies to the one cubemap
    setCubemapTextureParameters();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return tex;
}



void utl::setCubemapTextureParameters()
{
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}



void utl::errorCheck()
{
    int i;
    i = glGetError();
    if(i!=0)
    {
        std::cout << "Error happened while loading the texture: " << i << std::endl;
    }
}



FrameBufferObject utl::createFrameBufferObject(int width, int height)
{
    FrameBufferObject pod;

    pod.depthTexture = createNewDepthTexture(width, height);

    // Create a FBO and attach the depth texture:
    glGenFramebuffers(1, &pod.FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, pod.FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pod.depthTexture, 0);

    pod.colorTexture = createNewTexture(width, height);

    // Attach the color buffer:
    GLuint colorbuffer;
    glGenRenderbuffers(1, &colorbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pod.colorTexture, 0);

    return pod;
}


DoubleFrameBufferObject utl::createDoubleFrameBufferObject(int width, int height)
{
    DoubleFrameBufferObject fbObj;
    fbObj.ping = createFrameBufferObject(width, height);
    fbObj.pong = createFrameBufferObject(width, height);
    return fbObj;
}


void utl::setupFrameBuffer()
{
    setupFrameBuffer(0);
}

void utl::setupFrameBuffer(GLuint target)
{
    glBindFramebuffer(GL_FRAMEBUFFER, target);
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
