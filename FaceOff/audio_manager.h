#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include <al.h>
#include <alc.h>

#include<iostream>

using namespace std;

/*
http://mrl.nyu.edu/~dzorin/igf06/lecture10/OpenAL_install.txt

http://www.gamedev.net/page/resources/_/technical/game-programming/a-guide-to-starting-with-openal-r2008

http://kcat.strangesoft.net/openal-tutorial.html

ftp://ftp.ecs.csus.edu/clevengr/165/handouts/OpenALInstallation.pdf

https://www.openal.org/documentation/OpenAL_Programmers_Guide.pdf

http://www.edenwaith.com/products/pige/tutorials/openal.php#download

https://www.youtube.com/watch?v=ptjIf_9CKGE&ebc=ANyPxKpwHVNOmotBVQXS5_GBGHNhKkc231vyswpvg6hTVmuqVFetoggyI-EkiLMw2FsAuXzh7bceUxONQ8gFdKfl0X1rdGZb8Q

*/
/*
#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };

Aluint  buffer[NUM_BUFFERS];
Aluint  source[NUM_SOURCES];
ALuint  environment[NUM_ENVIRONMENTS];

ALsizei size, freq;
ALenum  format;
ALvoid  *data;

class AudioManager
{

	
	public:
	
	
		int init()
		{
			//Now OpenAL needs to be initialized 
			ALCdevice *device;                                                          //Create an OpenAL Device
			ALCcontext *context;                                                        //And an OpenAL Context
			
			
			// First the standard open-device, create-context, set-context.. 
			device = alcOpenDevice(NULL);
			if (!device)
			{
				fprintf(stderr, "Oops\n");
				return 1;
			}

			context = alcCreateContext(device, NULL);
			alcMakeContextCurrent(context);

			if (!context)
			{
				fprintf(stderr, "Oops2\n");
				return 1;
			}


			alListenerfv(AL_POSITION, listenerPos);
			alListenerfv(AL_VELOCITY, listenerVel);
			alListenerfv(AL_ORIENTATION, listenerOri);



			alGetError(); // clear any error messages

			// Generate buffers, or else no sound will happen!
			alGenBuffers(NUM_BUFFERS, buffer);

			if (alGetError() != AL_NO_ERROR)
			{
				printf("- Error creating buffers !!\n");
				exit(1);
			}
			else
			{
				printf("init() - No errors yet.");
			}

			alutLoadWAVFile("a.wav", &format, &data, &size, &freq);
			alBufferData(buffer[0], format, data, size, freq);
			alutUnloadWAV(format, data, size, freq);

			alGetError(); /* clear error */
			alGenSources(NUM_SOURCES, source);

			if (alGetError() != AL_NO_ERROR)
			{
				printf("- Error creating sources !!\n");
				exit(2);
			}
			else
			{
				printf("init - no errors after alGenSources\n");
			}


			alSourcef(source[0], AL_PITCH, 1.0f);
			alSourcef(source[0], AL_GAIN, 1.0f);
			alSourcefv(source[0], AL_POSITION, source0Pos);
			alSourcefv(source[0], AL_VELOCITY, source0Vel);
			alSourcei(source[0], AL_BUFFER, buffer[0]);
			alSourcei(source[0], AL_LOOPING, AL_TRUE);


		}

};
*/

#endif




