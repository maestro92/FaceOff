#ifndef RENDERER_CONSTANT_H_
#define RENDERER_CONSTANT_H_

// http://stackoverflow.com/questions/23288934/c-how-to-have-same-enum-members-name-in-different-enum-names-without-getting-e


namespace R_FULL_COLOR
{
	enum E
	{
		u_color = 0
	};
}

namespace R_FULL_TEXTURE
{
	enum E
	{
		u_texture = 0
	};
}

namespace R_DYNAMIC_MODEL
{
	enum E
	{
		u_texture = 0
	};
}

namespace R_PLAYER_TARGET
{
	enum E
	{
		u_texture = 0
	};
}

namespace R_SKYBOX
{
	enum E
	{
		u_cubeMapTextureID = 0
	};
}

namespace R_FIRE_WORK_EFFECT_UPDATE
{
	enum E
	{
		u_deltaTimeMillis = 0,
		u_time,
		u_randomTexture,
		u_launcherLifeTime,
		u_shellLifeTime,
		u_secondaryShellLifeTime
	};
}

namespace R_SMOKE_EFFECT_UPDATE
{
	enum E
	{
		u_deltaTimeMillis = 0,
		u_time,
		u_randomTexture,
		u_launcherLifeTime,
		u_shellLifeTime,
		u_secondaryShellLifeTime
	};
}

namespace R_SMOKE_EFFECT_RENDER
{
	enum E
	{
		u_time = 0,
		u_fadeRate,
		u_angle,
		u_billBoardSize,
		u_texture,
		u_depthTexture,
		u_inverseScreenSize,
		u_zNear,
		u_zFar
	};
}

#endif