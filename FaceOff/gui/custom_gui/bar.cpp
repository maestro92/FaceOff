#include "bar.h"





Bar::Bar()
{


}

Bar::Bar(int x, int y, int width, int height, glm::vec3 barColor, string iconFileName)
{
	background.setRect(x, y, width, height);
	background.setColor(BLACK);

	int diff = 4;
	int halfDiff = diff / 2;


	m_maxForegroundWidth = width - diff;


	m_widthValueRatio = (float)m_maxForegroundWidth / 100.0f;

	foreground.setRect(x + halfDiff, y + halfDiff, width - diff, height - diff);
	foreground.setColor(barColor);



	background.setFont(0, BLACK);
	background.setText("");

	foreground.setFont(0, BLACK);
	foreground.setText("");

	
	int iconSide = height + 25;
	int gap = 2;
	icon.setRect(x - iconSide - 2, y - 12, iconSide, iconSide);
	icon.setRectBgType(TEXTURED);
	string path = "./Assets/" + iconFileName;
	icon.setTexture(utl::loadTexture(path, true));

	icon.setFont(0, BLACK);
	icon.setText("");
}

int Bar::getType()
{
	return Control::HEALTH_BAR;
}

void Bar::render()
{
	background.render();
	foreground.render();
	icon.render();
}



void Bar::computeForegroundWidth(int hp)
{
	int w = (int)(hp * m_widthValueRatio);
	foreground.setWidth(w);

//	cout << "hp " << hp << endl;
//	cout << "w " << w << endl;
}


void Bar::customRender()
{

}

