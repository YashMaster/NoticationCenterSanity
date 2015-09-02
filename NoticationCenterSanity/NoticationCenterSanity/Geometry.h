#pragma once
#include <Windows.h>

namespace Geometry
{

	//Returns true if @pt is within @rect
	static inline bool IsInRect(POINT pt, RECT rect, bool edgesCount = true)
	{
		if (edgesCount)
		{
			if (rect.left <= pt.x && pt.x <= rect.right)
			{
				if (rect.top <= pt.y && pt.y <= rect.bottom)
				{
					return true;
				}
			}
		}
		else
		{
			if (rect.left < pt.x && pt.x < rect.right)
			{
				if (rect.top < pt.y && pt.y < rect.bottom)
				{
					return true;
				}
			}
		}
		return false;
	}
}