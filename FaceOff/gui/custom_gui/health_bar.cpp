#include "health_bar.h"





HealthBar::HealthBar()
{


}

HealthBar::HealthBar(int x, int y, int width, int height, glm::vec3 healthColor)
{
	background.setRect(x, y, width, height);
	background.setColor(BLACK);

	int diff = 4;
	int halfDiff = diff / 2;


	m_maxForegroundWidth = width - diff;


	m_widthHPRatio = (float)m_maxForegroundWidth / 100.0f;

	foreground.setRect(x + halfDiff, y + halfDiff, width - diff, height - diff);
	foreground.setColor(healthColor);



	background.setFont(0, BLACK);
	background.setText("");

	foreground.setFont(0, BLACK);
	foreground.setText("");

}

int HealthBar::getType()
{
	return Control::HEALTH_BAR;
}

void HealthBar::render()
{
	background.render();
	foreground.render();
}



void HealthBar::computeForegroundWidth(int hp)
{

	int w = (int)(hp * m_widthHPRatio);
	foreground.setWidth(w);
}


void HealthBar::customRender()
{

}

