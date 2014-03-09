#ifndef LISTBOX_H
#define LISTBOX_H

#include "Control.h"
#include <vector>
using std::vector;

class ListBox:public Control
{
public:
	ListBox(int posX, int posY, int width, int height);
	~ListBox();

	void addItem(string item);
	void removeItem(int index);
	void setCurrent(int index);

	int getIndex(void);
	int getCount(void);

	virtual bool updateControl(MouseState &state);
	virtual void drawControl(void);
	virtual string getType(void);

protected:
	int				index;
	vector<string>	items;
};


#endif