#include "ListBox.h"
const int itemHeight = 18;

ListBox::ListBox(int _posX, int _posY, int w, int h):
Control(posX, posY, w, h), index(0)
{

}
ListBox::~ListBox()
{

}

void	ListBox::addItem(string item)
{
	items.push_back(item);
}
void	ListBox::removeItem(int index)
{
	int i = 0;
	for (vector<string>::iterator it = items.begin(); it != items.end(); i++)
	{
		if (i == index)
		{
			items.erase(it);
			break;
		}
		i++;
	}

	if (index >= (int)items.size())
		index = (int)items.size() - 1;
}
void	ListBox::setCurrent(int _index)
{
	index = _index;
}
int		ListBox::getIndex(void)
{
	return index;
}
int		ListBox::getCount(void)
{
	return ((int)items.size());
}
bool	ListBox::updateControl(MouseState &state)
{
	Control::updateControl(state);

	int x = state.x;
	int y = state.y;
	if (inside && state.LeftButtonDown)
	{
		int tempIndex = (y - posY) / itemHeight;
		if (tempIndex >= 0 && tempIndex < (int)items.size())
		{
			index = tempIndex;
			return true;
		}
	}
	return false;
}
void	ListBox::drawControl(void)
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

	//glVertex2d(posX + width, posY);
	glEnd();

	if (index >= 0)
	{
		glColor4f(0.3, 0.3, 1.0f, .5f);
		int currentY = posY + index * itemHeight;
		glBegin(GL_QUADS);
		glVertex2d(posX + width, currentY);
		glVertex2d(posX, currentY);
		glVertex2d(posX, currentY + itemHeight);
		glVertex2d(posX + width, currentY + itemHeight);
		glEnd();
	}

	glColor4f(0.7, 0.7, 0.7f, 1.0f);
	for (int i = 0; i < (int)items.size(); i++)
	{
		iGLEngine->drawText(posX + 2, posY + 2 + i*itemHeight, items[i].data());
	}

}
string	ListBox::getType(void)
{
	return "listbox";
}