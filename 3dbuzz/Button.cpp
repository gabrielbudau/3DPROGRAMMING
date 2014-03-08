#include "Button.h"


Button::Button(string l, int _posX, int _posY, int w, int h) :
	Control(_posX, _posY, w, h), 
	label(l), 
	down(false)
{}
Button::~Button()
{

}
bool	Button::updateControl(MouseState &state)
{
	Control::updateControl(state);
	if (inside)
	{
		if (state.LeftButtonDown){
			down = true;
		}
		else if (down)
		{
			down = false;
			return true;
		}
	}
	return false;
}
void	Button::drawControl(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);
	glColor4f(.7f, .7f, .7f, .8f);
	glBegin(GL_QUADS);
	glVertex2d(posX + width,posY);
	glVertex2d(posX, posY);
	glVertex2d(posX, posY + height);
	glVertex2d(posX + width, posY + height);
	glEnd();

	float lineWidth;
	if (inside)
	{
		glColor4f(.2f, .2f, .7f, .8f);
		lineWidth = 5.0f;
	}
	else
	{
		glColor4f(.2f, .2f, .7f, .5f);
		lineWidth = 2.0f;
	}
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex2d(posX + width, posY + lineWidth/2);
	glVertex2d(posX, posY + lineWidth / 2);

	glVertex2d(posX, posY + height - lineWidth / 2);
	glVertex2d(posX + width, posY + height - lineWidth / 2);

	glVertex2d(posX + width - lineWidth / 2, posY + lineWidth);
	glVertex2d(posX + width - lineWidth / 2, posY + height - lineWidth);

	glVertex2d(posX + lineWidth / 2, posY + lineWidth);
	glVertex2d(posX + lineWidth / 2, posY + height - lineWidth);


	//glVertex2d(posX + width, posY);
	glEnd();

	glColor4f(.7f, .7f, .7f, 1.0f);
	int textX = posX + (width - iGLEngine->getTextWidth(label.data()))/2;
	int textY = posY + (height - iGLEngine->getTextHeight(label.data()))/2;
	iGLEngine->drawText(textX, textY, label.data());
}
string	Button::getType(void)
{
	return "button";
}
