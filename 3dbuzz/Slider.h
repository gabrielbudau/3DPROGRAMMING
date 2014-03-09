#ifndef SLIDER_H
#define SLIDER_H

#include "Control.h"

class Slider:public Control
{
public:
	Slider(string label, float min, float max, int _pX, int _pY, int w, int h);
	void setValue(float *value);
	virtual bool updateControl(MouseState &state);
	virtual void drawControl(void);
	virtual string getType(void);
protected:
	float		defaultValue;
	float		min, max;
	float		*current;
	string		label;
	bool		dragging;
};

#endif
