#ifndef CONTROL_H
#define CONTROL_H

#include "GLEngine.h"
#include <list>
#include <string>
using std::list;
struct MouseState
{
	int LeftButtonDown;
	int RightButtonDown;
	int MiddleButtonDown;

	int x, y;

	MouseState():
		LeftButtonDown(0),
		RightButtonDown(0),
		MiddleButtonDown(0),
		x(0), y(0){}
};

class Control
{
protected:
	bool		inside;
	int			posX, posY;
	int			width, height;
private:
public:
	static list<Control *> controls;
public:
	Control(int _posX, int _posY, int _width, int _height);
	virtual ~Control();

	virtual bool updateControl(MouseState &state);
	virtual void drawControl(void) = 0;
	virtual string getType(void) = 0;

	void setPosition(int x, int y);
	void setSize(int w, int h);
	int getWidth();
	int getHeight();
};

Control*  addControl(Control* control);

#endif
