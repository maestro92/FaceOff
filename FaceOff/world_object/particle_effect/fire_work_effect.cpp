#include "fire_work_effect.h"

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

FireWorkEffect::FireWorkEffect()
{
	m_currVB = 0;
	m_currTFB = 1;
	m_isFirst = true;
	m_textureId = NULL;
	m_time = 0;

//	m_geometry = new Sphere();
}

FireWorkEffect::~FireWorkEffect()
{
	if (m_transformFeedback[0] != 0)
	{
		glDeleteTransformFeedbacks(2, m_transformFeedback);
	}

	if (m_particleBuffer[0] != 0)
	{
		glDeleteBuffers(2, m_particleBuffer);
	}
}


void FireWorkEffect::init()
{
	FireWorkParticle particles[MAX_PARTICLES];
	memset(particles, 0, sizeof(particles));

	// we set the first particle as a Laucher
	particles[0].type = PARTICLE_TYPE_LAUNCHER;
	particles[0].position = glm::vec3(0.0f, 0.0f, 0.0f);
	particles[0].velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	particles[0].lifetimeMillis = 0.0f;

	// making two transform feedback
	// we create two transformfeedback objects by calling glGenTransformFeedbacks
	glGenTransformFeedbacks(2, m_transformFeedback);

	// we create two buffers objects, one for each transform feedback objects
	glGenBuffers(2, m_particleBuffer);

	for (unsigned int i = 0; i < 2; i++)
	{
		// we start by binding GL_Transform_Feedback target to the transformfeedback object
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);

		// Next we bind the the corresponding buffer object to the GL_ARRAY_BUFFER
		// that makes it a regular vertex buffer and load the contents of the particle array into it.
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);

		// This makes this buffer a transform feedback buffer and places it as index zero
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[i]);
	}

	initRandomTexture(1000);

}


bool FireWorkEffect::initRandomTexture(int size)
{
	glm::vec3* pRandomData = new glm::vec3[size];
	for (unsigned int i = 0; i < size; i++)
	{
		pRandomData[i].x = utl::randFloat();
		pRandomData[i].y = utl::randFloat();
		pRandomData[i].z = utl::randFloat();
	}

	glGenTextures(1, &m_randomTextureId);
	glBindTexture(GL_TEXTURE_1D, m_randomTextureId);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0, GL_RGB, GL_FLOAT, pRandomData);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	delete[] pRandomData;


	// check OpenGL error
	GLenum err;
	if ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "random texture error: " << err << endl;
	}

	cout << "random texture very nice" << endl;

	return true;
}


void FireWorkEffect::update(Pipeline& p, Renderer* r)
{
	//   m_randomTexture.Bind(RANDOM_TEXTURE_UNIT);
	p.pushMatrix();

	r->setUniLocs(p);

	// We have another draw call later on that does that.
	// Calling glEnable() with the GL_RASTERIZER_DISCARD flag
	// tells the pipeline to discard all primitives before
	// they reach the rasterizer (but after the optional transform feedback stage).
	glEnable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currVB]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(FireWorkParticle), 0);								// type
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FireWorkParticle), (const GLvoid*)4);				// position
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(FireWorkParticle), (const GLvoid*)16);				// velocity
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(FireWorkParticle), (const GLvoid*)28);				// lifetime

	glBeginTransformFeedback(GL_POINTS);

	// first drawing function, since we know we're drawing one point
	if (m_isFirst)
	{
		glDrawArrays(GL_POINTS, 0, 1);
		m_isFirst = false;
	}
	else
	{
		glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currVB]);
	}

	glEndTransformFeedback();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glDisable(GL_RASTERIZER_DISCARD);

	p.popMatrix();
}

void FireWorkEffect::render(Pipeline& p, Renderer* r)
{
	p.pushMatrix();

	p.translate(m_position);
	p.addMatrix(m_rotation);
	p.scale(m_scale);


	r->setUniLocs(p);


	glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB]);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FireWorkParticle), (const GLvoid*)4);  // position

	glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currTFB]);

	glDisableVertexAttribArray(0);

	p.popMatrix();

	m_currVB = m_currTFB;
	m_currTFB = (m_currTFB + 1) & 0x1;

}




void FireWorkEffect::renderParticles()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB]);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FireWorkParticle), (const GLvoid*)4);  // position

	glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currTFB]);

	glDisableVertexAttribArray(0);
}


ParticleEffectType FireWorkEffect::getParticleEffectType()
{
	return FIRE_WORK;
}
