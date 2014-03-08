#ifndef BUTTON_H
#define BUTTON_H

#include "Control.h"

class Button:public Control
{
public:
	Button(string label, int _posX, int _posY, int w, int h);
	~Button();

	virtual bool updateControl(MouseState &state);
	virtual void drawControl(void);
	virtual string getType(void);

protected:
	bool down;
	string label;
};
#endif
