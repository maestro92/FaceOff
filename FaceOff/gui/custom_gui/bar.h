#ifndef BAR_H_
#define BAR_H_

#include "control.h"
#include "label.h"

using namespace std;


class Bar : public Control
{

	public:
		Label background;
		Label foreground;
		Label icon;
		Label text;

		Bar();
		Bar(int x, int y, int width, int height, glm::vec3 barColor, string iconFileName);
		virtual int getType();
		void render();
		void customRender();

		void computeForegroundWidth(int hp);

		int m_maxForegroundWidth;

		float m_widthValueRatio;
		//		int m_value;

};


#endif