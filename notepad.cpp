#include<iostream>
#include<conio.h>
#include <windows.h>
#include<fstream>
using namespace std;
struct node
{
	char letter;
	node* next;
	node* prev;
	node* up;
	node* down;
};
class TwoDDLLADT
{
	node* first, *cursor;
public:
	int x, y;
	TwoDDLLADT()
	{
		x = y = 0;
		first = new node;
		first->letter = '\0';
		first->next = first->prev = first->up = first->down = 0;
		cursor = first;
	}
	void Insert(char c)
	{
		x++;
		if (cursor->next == 0)
		{//insert at end
			cursor->next = new node;
			cursor->next->letter = c;
			cursor->next->prev = cursor;
			cursor = cursor->next;
			cursor->next = cursor->up = cursor->down = 0;
			node* tmp = cursor;
			tmp = tmp->prev;
			if (tmp->up != 0)
			{
				if (tmp->up->next->letter != '\n')
				{
					tmp = tmp->up;
					tmp = tmp->next;
					tmp->down = cursor;
					cursor->up = tmp;
				}
			}
		}
		else if (cursor->next != 0)
		{//insert in middle
			node* tmp;
			tmp = new node;
			tmp->letter = c;
			tmp->prev = cursor;
			tmp->next = cursor->next;
			cursor->next->prev = tmp;
			cursor->next = tmp;
			cursor = cursor->next;
			cursor->up = cursor->down = 0; //up down case
			if (tmp->next->letter == '\n')
			{
				node* tmp1;
				tmp1 = tmp;
				tmp1 = tmp1->prev;
				if (tmp1->up != 0)
				{
					tmp1 = tmp1->up;
					if (tmp1->next != 0 && tmp1->next->letter != '\n')
					{
						tmp1 = tmp1->next;
						tmp->up = tmp1;
						tmp1->down = tmp;
					}
				}
				tmp1 = tmp;
				tmp1 = tmp1->prev;
				if (tmp1->down != 0)
				{
					tmp1 = tmp1->down;
					if (tmp1->next != 0 && tmp1->next->letter != '\n')
					{
						tmp1 = tmp1->next;
						tmp->down = tmp1;
						tmp1->up = tmp;
					}
				}
			}
			else
			{
				node* tmp2, *tmp3;
				tmp2 = tmp3 = tmp;
				if (tmp->next->up != 0)
					tmp2 = tmp->next->up;
				else
					tmp2 = 0;
				if (tmp->next->down != 0)
					tmp3 = tmp->next->down;
				else
					tmp3 = 0;
				while (tmp != 0 && tmp->letter != '\n')
				{
					tmp->up = tmp2;
					if (tmp2 != 0)
					{
						tmp2->down = tmp;
						tmp2 = tmp2->next;
						if (tmp2 != 0)
							if (tmp2->letter == '\n')
								tmp2 = 0;
					}
					tmp->down = tmp3;
					if (tmp3 != 0)
					{
						tmp3->up = tmp;
						tmp3 = tmp3->next;
						if (tmp3 != 0)
							if (tmp3->letter == '\n')
								tmp3 = 0;
					}
					tmp = tmp->next;
				}
			}
		}
	}
	void Print()
	{
		node* tmp = first;
		tmp = tmp->next;
		while (tmp != 0)
		{
			cout << tmp->letter;
			tmp = tmp->next;
		}
	}
	void Save()
	{
		ofstream out;
		out.open("save.txt");
		node* tmp = first;
		tmp = tmp->next;
		while (tmp != 0)
		{
			out << tmp->letter;
			tmp = tmp->next;
		}
	}
	void Load()
	{
		ifstream fin;
		char tmp;
		node* tmpptr = first;
		while (first != 0)
		{
			first = first->next;
			delete tmpptr;
			tmpptr = first;
		}
		x = y = 0;
		first = new node;
		first->letter = '\0';
		first->down = first->up = first->next = first->prev = 0;
		cursor = first;
		fin.open("save.txt");
		while (!fin.eof())
		{
			fin.get(tmp);
			if (tmp == '\n')
				newLine();
			else Insert(tmp);
		}
		Delete();
	}
	bool Delete()
	{
		if (cursor->next == 0)
		{
			if (cursor->letter != '\0')
			{
				node* tmp = cursor;
				if (tmp->up != 0)
					tmp->up->down = 0;
				tmp->prev->next = 0;
				cursor = cursor->prev;
				delete tmp;
			}
			else return false;
		}
		else
		{
			if (cursor->letter == '\0')
				return false;
			node *tmp, *tmp1, *tmp2;
			if (cursor->letter == '\n')
			{
				node* del = cursor;
				tmp = cursor->up;
				if (cursor->down != 0)
					tmp2 = cursor->down;
				else tmp2 = 0;
				if (tmp->up != 0)
					tmp1 = tmp->up;
				else tmp1 = 0;
				cursor = cursor->prev;
				cursor->next = del->next;
				del->next->prev = cursor;
				delete del;
			}
			else
			{
				tmp = cursor;
				if (cursor->up != 0)
					tmp1 = cursor->up;
				else tmp1 = 0;
				if (cursor->down != 0)
					tmp2 = cursor->down;
				else tmp2 = 0;
				cursor = cursor->prev;
				cursor->next = tmp->next;
				tmp->next->prev = cursor;
				delete tmp;
				tmp = cursor->next;
				if (tmp->letter == '\n')
				{
					tmp = 0;
					if (cursor->up != 0)
						if (cursor->up->next->letter != '\n')
							cursor->up->next->down = 0;
					if (cursor->down != 0)
						if (cursor->down->next->letter != '\n')
							cursor->down->next->up = 0;
				}
			}
			while (tmp != 0)
			{
				if (tmp->up != 0)
					tmp->up->down = 0;
				if (tmp->down != 0)
					tmp->down->up = 0;
				tmp->up = tmp1;
				tmp->down = tmp2;
				if (tmp1 != 0)
				{
					tmp1->down = tmp;
					if (tmp1->next->letter != '\n')
					{
						tmp1 = tmp1->next;
					}
					else tmp1 = 0;
				}
				if (tmp2 != 0)
				{
					tmp2->up = tmp;
					if (tmp2->next != 0 && tmp2->next->letter != '\n')
					{
						tmp2 = tmp2->next;
					}
					else tmp2 = 0;
				}
				tmp = tmp->next;
				if (tmp != 0)
					if (tmp->letter == '\n')
						tmp = 0;
			}
		}
		if (x != 0)
			x--;
		else
		{
			y--;
			node* tmp = cursor;
			while (tmp->letter != '\n' && tmp->letter != '\0')
			{
				tmp = tmp->prev;
				x++;
			}
		}
		return true;
	}
	bool Up()
	{
		if (y == 0)
			return false;
		while (cursor->up == 0)
		{
			cursor = cursor->prev;
			x--;
		}
		cursor = cursor->up;
		y--;
		return true;
	}
	bool Down()
	{
		int tmp = 0;
		while (cursor->down == 0)
		{
			if (cursor->letter == '\n' || cursor->letter == '\0')
			{
				while (tmp != 0)
				{
					cursor = cursor->next;
					x++;
					tmp--;
				}
				return false;
			}
			cursor = cursor->prev;
			x--;
			tmp++;
		}
		cursor = cursor->down;
		y++;
		return true;
	}
	bool Left()
	{
		if (cursor->letter == '\0')
			return false;
		if (cursor->letter == '\n')
		{
			y--;
			cursor = cursor->up;
			while (cursor->next->letter != '\n')
			{
				cursor = cursor->next;
				x++;
			}
			return true;
		}
		x--;
		cursor = cursor->prev;
		return true;
	}
	bool Right()
	{
		if (cursor->next == 0)
			return false;
		cursor = cursor->next;
		x++;
		if (cursor->letter == '\n') {
			x = 0;
			y++;
		}
	}
	void newLine()
	{
		x = 0;
		y++;
		if (cursor->next == 0)
		{
			cursor->next = new node;
			cursor->next->letter = '\n';
			cursor->next->prev = cursor;
			cursor = cursor->next;
			cursor->next = cursor->up = cursor->down = 0;
			node* tmp = first, *tmp2 = first;
			while (tmp->down != 0)
				tmp = tmp->down;
			while (tmp2->next != 0)
				tmp2 = tmp2->next;
			tmp->down = tmp2;
			tmp2->up = tmp;
		}
		if (cursor->next != 0)
		{
			node *tmp1, *tmp2, *tmp;
			tmp = new node;
			tmp->letter = '\n';
			tmp->prev = cursor;
			tmp->next = cursor->next;
			cursor->next->prev = tmp;
			cursor->next = tmp;
			cursor = cursor->next;
			cursor->up = cursor->down = 0;
			tmp = tmp->next;
			while (tmp != 0 && tmp->letter != '\n')
			{
				if (tmp->up != 0)
				{
					tmp->up->down = 0;
					tmp->up = 0;
				}
				if (tmp->down != 0)
				{
					tmp->down->up = 0;
					tmp->down = 0;
				}
				tmp = tmp->next;
			}
			tmp = cursor;
			tmp1 = tmp2 = tmp->prev;

			while (tmp1->letter != '\n' && tmp1->letter != '\0')
				tmp1 = tmp1->prev;
			if (tmp1->down != 0)
				tmp2 = tmp1->down;
			else tmp2 = 0;
			while (tmp != 0 || tmp1 != 0 || tmp2 != 0)
			{
				if (tmp != 0)
				{
					tmp->up = tmp1;
					tmp->down = tmp2;
				}
				if (tmp1 != 0)
					tmp1->down = tmp;
				if (tmp2 != 0)
					tmp2->up = tmp;
				if (tmp != 0)
				{
					if (tmp->next == 0)
						tmp = 0;
					else if (tmp->next->letter == '\n')
						tmp = 0;
					else
						tmp = tmp->next;
				}
				if (tmp1 != 0)
				{
					if (tmp1->next == 0)
						tmp1 = 0;
					else if (tmp1->next->letter == '\n')
						tmp1 = 0;
					else
						tmp1 = tmp1->next;
				}
				if (tmp2 != 0)
				{
					if (tmp2->next == 0)
						tmp2 = 0;
					else if (tmp2->next->letter == '\n')
						tmp2 = 0;
					else
						tmp2 = tmp2->next;
				}
			}
		}
	}
	bool selectRight(node* &selectedtext)
	{
	}
	~TwoDDLLADT()
	{
		node* tmp = first;
		while (first != 0)
		{
			first = first->next;
			delete tmp;
			tmp = first;
		}
	}
};
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
int main()
{
	TwoDDLLADT notepad;
	node* selectedText = 0;
	while (1) {
		char c = _getch();
		if (c == -32)
		{
			c = _getch();
			if (c == 'H')
				notepad.Up();
			if (c == 'K')
				notepad.Left();
			if (c == 'M')
				notepad.Right();
			if (c == 'P')
				notepad.Down();
		}
		else if (c == 13)
			notepad.newLine();
		else if (c == '\b')
			notepad.Delete();
		else if (c == 19)
		{
			notepad.Save();
		}
		else if (c == 12)
		{
			notepad.Load();
		}
		else if (c == 17)
			return 0;
		else
			notepad.Insert(c);
		system("CLS");
		notepad.Print();
		gotoxy(notepad.x, notepad.y);
	}
	system("pause");
}
