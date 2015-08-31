#pragma once
#include <stdio.h>
#include <Windows.h>

class Geometry
{
private:

protected:

public:

	//Compares two RECTs. Returns true if they're equal. False if they aren't. 
	static inline bool Equals(const RECT *r1, const RECT *r2)
	{
		if (r1->bottom == r2->bottom &&
			r1->left == r2->left &&
			r1->top == r2->top &&
			r1->right == r2->right)
		{
			return true;
		}
		return false;
	}

	//Base times height fool
	static double GetArea(const RECT *r1)
	{
		return double(r1->right - r1->left) * (r1->bottom - r1->top);
	}

	//Returns a "left-top-right-bottom rect" from a the (x, y) and (width, hight)
	static RECT ToRect(int x, int y, int width, int height)
	{
		RECT ret;
		ret.left = x;
		ret.top = y;
		ret.right = x + width;
		ret.bottom = y + height;
		return ret;
	}

	//Trims a rect to fit within a boundry rect. It does this by simply shrinking the offending sides. Note: @inner will be changed!
	static RECT *Trim(RECT *inner, const RECT *outer)
	{
		if (inner->left < outer->left)
			inner->left = outer->left;

		if (inner->top < outer->top)
			inner->top = outer->top;

		if (inner->right > outer->right)
			inner->right = outer->right;

		if (inner->bottom > outer->bottom)
			inner->bottom = outer->bottom;

		return inner;
	}

	//Returns true if @pt is within @rect
	static inline bool IsInRect(POINT *pt, RECT *rect)
	{
		//Check arguements
		if (rect == NULL || pt == NULL)
		{
			printf("IsInRect: an argument was null...\n");
			return false;
		}

		if (rect->left <= pt->x && pt->x <= rect->right)
		{
			if (rect->top <= pt->y && pt->y <= rect->bottom)
			{
				return true;
			}
		}
		return false;
	}

	//Creates a box
	static inline RECT CreateBox()
	{
		return{ LONG_MAX, LONG_MAX, LONG_MIN, LONG_MIN };
	}

	//Extends @box such that @pt would fit inside it. Returns false if @box or @pt is NULL or anything else goes wrong. 
	static inline bool AddToBox(RECT *box, const POINT *pt)
	{
		//Check arguements
		if (box == NULL || pt == NULL)
		{
			printf("AddToBox: an argument was null...\n");
			return false;
		}

		if (pt->x < box->left)
			box->left = pt->x;

		if (pt->x > box->right)
			box->right = pt->x;

		if (pt->y < box->top)
			box->top = pt->y;

		if (pt->y > box->bottom)
			box->bottom = pt->y;

		return true;
	}

	//Finds the center of @in.
	static inline POINT FindCenter(RECT *in)
	{
		POINT center = { 0, 0 };
		center.x = in->left + (in->right / 2 - in->left / 2);
		center.y = in->top + (in->bottom / 2 - in->top / 2);
		return center;
	}

	//Prints a POINT without adding a newline
	static inline void Print(POINT p)
	{
		printf("(%d, %d)", p.x, p.y);
	}

	//Prints a RECT without adding a newline
	static inline void Print(RECT r)
	{
		printf("Rect: %4d %4d %4d %4d\n", r.left, r.top, r.right, r.bottom);
	}

};




