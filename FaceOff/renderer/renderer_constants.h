#ifndef RENDERER_CONSTANT_H_
#define RENDERER_CONSTANT_H_


enum R_FULL_COLOR
{
	u_color = 0
};

enum R_FULL_TEXTURE
{
	u_texture = 0
};

enum R_PLAYER_TARGET
{
	u_texture = 0
};

enum R_FIRE_WORK_EFFECT_UPDATE
{
	u_deltaTimeMillis = 0,
	u_time,
	u_randomTexture,
	u_launcherLifeTime,
	u_shellLifeTime,
	u_secondaryShellLifeTime
};

enum R_SMOKE_EFFECT_UPDATE
{
	u_deltaTimeMillis = 0,
	u_time,
	u_randomTexture,
	u_launcherLifeTime,
	u_shellLifeTime,
	u_secondaryShellLifeTime
};

enum R_SMOKE_EFFECT_RENDER
{
	u_time = 0,
	u_fadeRate,
	u_billBoardSize,
	u_centerPosition,
	u_texture
};

#endif