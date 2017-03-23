#ifndef GUI_RENDERER_CONSTANT_H_
#define GUI_RENDERER_CONSTANT_H_

// http://stackoverflow.com/questions/23288934/c-how-to-have-same-enum-members-name-in-different-enum-names-without-getting-e


namespace R_TEXTURE
{
	enum E
	{
		u_texture = 0
	};
}

namespace R_COLORED_RECT
{
	enum E
	{
		u_color = 0
	};
}

namespace R_TEXTURED_RECT
{
	enum E
	{
		u_texture = 0
	};
}

namespace R_LIST_BOX_ITEM_HIGHLIGHT
{
	enum E
	{
		u_x1 = 0,
		u_x2,
		u_y1,
		u_y2
	};
}

namespace R_SNIPER_SCOPE_VIEW
{
	enum E
	{
		u_sniperScopeViewTexture = 0,
		u_sceneTexture
	};
}

namespace R_TEXT
{
	enum E
	{
		u_texture = 0,
		u_color
	};
}

#endif