#include "Slider.h"

const int tickSize = 5;

Slider::Slider(string l, float _min, float _max, int _pX, int _pY, int w, int h) :
Control(_pX, _pY, w, h), label(l), dragging(false)
{
	defaultValue = 0.0f;
	current = NULL;
	min = _min;
	max = _max;
}
void	Slider::setValue(float *value)
{
	current = value;
	if (!current)
		defaultValue = *current;
}
bool	Slider::updateControl(MouseState &state)
{
	Control::updateControl(state);
	int x = state.x;
	int y = state.y;

	if (inside)
	{
		if (state.LeftButtonDown)
		{
			dragging = true;
		}
		if (state.RightButtonDown)
		{
			*current = defaultValue;
		}
	}

	if (!(state.LeftButtonDown))
	{
		dragging = false;
	}
	if (dragging)
	{
		*current = (float)(x - posX) / (float)width * (max - min) + min;

		if (*current > max)
		{
			*current = max;
		}

		if (*current < min)
		{
			*current = min;
		}
	}
	return dragging;
}
void	Slider::drawControl(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);
	glColor4f(.7f, .7f, .7f, .8f);
	glBegin(GL_QUADS);
	glVertex2d(posX + width, posY);
	glVertex2d(posX, posY);
	glVertex2d(posX, posY + height);
	glVertex2d(posX + width, posY + height);
	glEnd();

	float lineWidth;
	if (inside)
	{
		glColor4f(.2f, .2f, .7f, .8f);
		lineWidth = 2.0f;
	}
	else
	{
		glColor4f(.2f, .2f, .7f, .5f);
		lineWidth = 1.0f;
	}
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex2d(posX + width, posY + lineWidth / 2);
	glVertex2d(posX, posY + lineWidth / 2);

	glVertex2d(posX, posY + height - lineWidth / 2);
	glVertex2d(posX + width, posY + height - lineWidth / 2);

	glVertex2d(posX + width - lineWidth / 2, posY + lineWidth);
	glVertex2d(posX + width - lineWidth / 2, posY + height - lineWidth);

	glVertex2d(posX + lineWidth / 2, posY + lineWidth);
	glVertex2d(posX + lineWidth / 2, posY + height - lineWidth);

	glEnd();

	int currentX = (int)((*current - min) / (max - min)*(width - tickSize) + posX);
	int test = *current;
	glColor4f(.3f, .3f, 1.0f, .5f);
	glBegin(GL_QUADS);
	glVertex2d(currentX + tickSize, posY);
	glVertex2d(currentX, posY);
	glVertex2d(currentX, posY + height);
	glVertex2d(currentX + tickSize, posY + height);
	glEnd();

	glColor4f(.7f, .7f, .7f, 1.0f);
	iGLEngine->drawText(posX + 2, posY + 2, label.data());
}
string	Slider::getType(void)
{
	return "slider";
}