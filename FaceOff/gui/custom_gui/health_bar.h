#ifndef HEALTH_BAR_H_
#define HEALTH_BAR_H_

#include "control.h"
#include "label.h"

using namespace std;


class HealthBar : public Control
{

	public:

		Label background;
		Label foreground;

		HealthBar();
		HealthBar(int x, int y, int width, int height, glm::vec3 healthColor);
		virtual int getType();
		void render();
		void customRender();

		void computeForegroundWidth(int hp);

		int m_maxForegroundWidth;

		float m_widthHPRatio;
		//		int m_value;

};


#endif