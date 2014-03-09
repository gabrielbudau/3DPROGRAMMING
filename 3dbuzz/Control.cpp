#include "Control.h"

list<Control *> Control::controls;

Control::Control(int _posX, int _posY, int _width, int _height)
{
	controls.push_back(this);
	setPosition(_posX, _posY);
	setSize(_width, _height);
}
Control:: ~Control()
{

}
bool	Control::updateControl(MouseState &state)
{
	int x = state.x;
	int y = state.y;

	inside = false;
	if (x >= posX && x <= posX + width && y >= posY && y <= posY + height)
	{
		inside = true;
	}
	return false;
}

void Control::setPosition(int x, int y)
{
	posX = x;
	posY = y;
}
void Control::setSize(int w, int h)
{
	width = w;
	height = h;
}
int Control::getWidth()
{
	return width;
}
int Control::getHeight()
{
	return height;
}

Control*  addControl(Control* control)
{
	static int lastX = 5;
	static int lastY = 5;
	control->setPosition(lastX, lastY);
	lastY += control->getHeight() + 5;

	return control;
}