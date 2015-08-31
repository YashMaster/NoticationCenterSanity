#pragma once
#include "C.h"
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>


#define SWAP (a, b, temp) temp = a; a = b; b = temp; 

/*enum CoordinateTypesEnum
{
COODINATE_RESOLUTION,
COODINATE_SIZE,
COODINATE_TOUCH_PANEL,
COODINATE_TYPE_COUNT,
};
typedef enum CoordinateTypesEnum CoordinateType;

char *CoordinateTypeToStr [] =
{
"COORDINATE_RESOLUTION",
"COORDINATE_SIZE",
"COORDINATE_TOUCH_PANEL",
"COORDINATE_TYPE_COUNT",
};
//I don't know if I'm ever going to finish this function. It's pretty stupid actually.
POINT Convert(CoordinateType inputType, CoordinateType outputType, POINT input)
{
POINT output;

return output;
}*/



class CoordinateConverter
{

private:
	DEVMODE devmode;
	HDC screen;

protected:

public:
	POINT ResolutionMaxes, DefaultResolutionMaxes; //Unit is in Pixels. 
	POINT ScreenSizeMaxes, DefaultScreenSizeMaxes; //Unit is in HydraMeters, Millicentimeters, or Tinimeters, which is 1E-5 Meters.
	POINT TouchPanelMaxes, DefaultTouchPanelMaxes; //This is based on whatever we get from the touch panel. Hopefully it doesn't change. OHHH that might be it... 
	POINT MouseInputMaxes; //This is usually just 16 bit int maxes: [65535, 65535]
	DWORD Orientation;	   //Orientation since the last refresh. Determines if the screen is landscape, portrait, landscape flipped, or portrait flipped. 	


	CoordinateConverter()
	{
		//Preallocate this For Orientation
		ZeroMemory(&devmode, sizeof(DEVMODE));
		devmode.dmSize = sizeof(DEVMODE);
		devmode.dmFields = DM_DISPLAYORIENTATION;

		//Preget handle for device caps. 
		screen = GetDC(NULL);

		//Mouse input maxes don't change so we can assign them here. 
		MouseInputMaxes.x = UINT16_MAX; // 65535;
		MouseInputMaxes.y = UINT16_MAX; // 65535;
	}

	//Get all the data necessary. 
	void Initialize(POINT TouchPanel)
	{
		//Screen Resolution
		this->ResolutionMaxes.x = GetSystemMetrics(SM_CXSCREEN);
		this->ResolutionMaxes.y = GetSystemMetrics(SM_CYSCREEN);

		//Physical Screen Size
		this->ScreenSizeMaxes.x = GetDeviceCaps(screen, HORZSIZE) * 100;
		this->ScreenSizeMaxes.y = GetDeviceCaps(screen, VERTSIZE) * 100;

		//TouchPanel Resolution - this is the weird one. 0, 0 is the top left corner in the DEFAULT orientation ALWAYS. and it will always be that corner, regardless of orientation. 
		//this->TouchPanelMaxes = TouchPanel;
		this->DefaultTouchPanelMaxes = TouchPanel;

		//Orientation
		this->Orientation = GetOrientation();

		//Assign the Default Maxes
		if (this->Orientation == DMDO_90 ||
			this->Orientation == DMDO_270)
		{
			this->DefaultResolutionMaxes = { this->ResolutionMaxes.y, this->ResolutionMaxes.x };
			this->DefaultScreenSizeMaxes = { this->ScreenSizeMaxes.y, this->ScreenSizeMaxes.x };
			this->TouchPanelMaxes = { this->DefaultTouchPanelMaxes.y, this->DefaultTouchPanelMaxes.x }; //This is weird, because touchpanel alwas returns the "default value" so in this case we want to return the  
		}
		//Otherwise, just assign the default values. 
		else
		{
			this->DefaultResolutionMaxes = this->ResolutionMaxes;
			this->DefaultScreenSizeMaxes = this->ScreenSizeMaxes;
			//this->DefaultTouchPanelMaxes = this->TouchPanelMaxes;
			this->TouchPanelMaxes = this->DefaultTouchPanelMaxes;
		}


		printf("TouchPanel Maxes: %ix%i\n", this->TouchPanelMaxes.x, this->TouchPanelMaxes.y);
		printf("Screen Size: (%imm X %imm) \n", this->ScreenSizeMaxes.x, this->ScreenSizeMaxes.y);
		printf("Screen Resolution: %ix%i\n", this->ResolutionMaxes.x, this->ResolutionMaxes.y);
		printf("Default - TouchPanel Maxes: %ix%i\n", this->DefaultTouchPanelMaxes.x, this->DefaultTouchPanelMaxes.y);
		printf("Default - Screen Size: (%imm X %imm) \n", this->DefaultScreenSizeMaxes.x, this->DefaultScreenSizeMaxes.y);
		printf("Default - Screen Resolution: %ix%i\n", this->DefaultResolutionMaxes.x, this->DefaultResolutionMaxes.y);


	}

	//Refreshes the necessary data points so we can adjust for screen orientation changes. 
	void Refresh()
	{
		/*//Screen Resolution
		this->ResolutionMaxes.x = GetSystemMetrics(SM_CXSCREEN);
		this->ResolutionMaxes.y = GetSystemMetrics(SM_CYSCREEN);
		printf("Screen Resolution: %ix%i\n", this->ResolutionMaxes.x, this->ResolutionMaxes.y);

		//Physical Screen Size
		//screen = GetDC(NULL);
		this->ScreenSizeMaxes.x = GetDeviceCaps(screen, HORZSIZE) * 100;
		this->ScreenSizeMaxes.y = GetDeviceCaps(screen, VERTSIZE) * 100;
		printf("Screen Size: (%imm X %imm) \n", this->ScreenSizeMaxes.x, this->ScreenSizeMaxes.y);

		//Touchpanel
		printf("TouchPanel Maxes: %ix%i\n", this->TouchPanelMaxes.x, this->TouchPanelMaxes.y);*/

		//Orientation
		EnumDisplaySettingsEx(NULL, ENUM_CURRENT_SETTINGS, &devmode, EDS_RAWMODE);
		this->Orientation = devmode.dmDisplayOrientation;

		//If orientation is flipped, swap default values. 
		if (this->Orientation == DMDO_90 ||
			this->Orientation == DMDO_270)
		{
			this->ResolutionMaxes = { this->DefaultResolutionMaxes.y, this->DefaultResolutionMaxes.x };
			this->ScreenSizeMaxes = { this->DefaultScreenSizeMaxes.y, this->DefaultScreenSizeMaxes.x };
			this->TouchPanelMaxes = { this->DefaultTouchPanelMaxes.y, this->DefaultTouchPanelMaxes.x };
		}
		//Otherwise, just assign the default values. 
		else
		{
			this->ResolutionMaxes = this->DefaultResolutionMaxes;
			this->ScreenSizeMaxes = this->DefaultScreenSizeMaxes;
			this->TouchPanelMaxes = this->DefaultTouchPanelMaxes;
		}

		printf("Orientation: %d \n", this->Orientation);
		printf("Screen Resolution: %ix%i\n", this->ResolutionMaxes.x, this->ResolutionMaxes.y);
		printf("Screen Size: (%imm X %imm) \n", this->ScreenSizeMaxes.x, this->ScreenSizeMaxes.y);
		printf("TouchPanel Maxes: %ix%i\n", this->TouchPanelMaxes.x, this->TouchPanelMaxes.y);
	}

	//Gets the display orientation as fast as possible!
	DWORD GetOrientation()
	{
		//Hopefully this isn't necessary.
		//ZeroMemory(&devmode, sizeof(DEVMODE));
		//devmode.dmSize = sizeof(DEVMODE);
		//devmode.dmFields = DM_DISPLAYORIENTATION;

		//Refresh display orientation
		EnumDisplaySettingsEx(NULL, ENUM_CURRENT_SETTINGS, &devmode, EDS_RAWMODE);

		return this->Orientation = devmode.dmDisplayOrientation;
	}

	//Optimized conversion from TouchPanel coordinates to ScreenSize coordinates.
	void TouchPanelToScreenSize(LONG *x, LONG *y)
	{
		//double xRatio = ((double) ScreenSizeMaxes.x) / ((double) TouchPanelMaxes.x);
		//double yRatio = ((double) ScreenSizeMaxes.y) / ((double) TouchPanelMaxes.y);

		//printf("AJTouchPanel: (%4d, %4d) ", *x, *y);

		*x = LONG(((double)*x) * (((double)ScreenSizeMaxes.x) / ((double)TouchPanelMaxes.x)));
		*y = LONG(((double)*y) * (((double)ScreenSizeMaxes.y) / ((double)TouchPanelMaxes.y)));

		//printf("ScreenSize: (%5d, %5d) ", *x, *y);
		//printf("Resolution: (%4d, %4d) ", Convert(ScreenSizeMaxes, ResolutionMaxes, POINT{ *x, *y }).x, Convert(ScreenSizeMaxes, ResolutionMaxes, POINT{ *x, *y }).y);
		//printf("\n");

		return;
	}

	//Physical - Reorient 
	//TouchPanel - Do not Reorient
	//Resolution - Reorient
	//Mouse - I don't know... 


	//Adjusts for orientation... This should be called before any further coordinate conversion. 
	int AdjustTouchOrientation(LONG *x, LONG *y)
	{
		LONG tX = *x;
		LONG tY = *y;


		switch (this->Orientation)
		{
		case DMDO_DEFAULT:
			//Nothing needs to be adjusted here. 
			//	0 degrees [default]:
			// 	(x, y) -> (x, y)
			//	(1, 2) -> (1, 2)
			//
			//   (0, 0) ******************* (xMax, 0)
			//          *                 *
			//          * @               *
			//          *                 *
			//          *                 *
			//          *                 *
			//(0, yMax) ******************* (xMax, yMax)

			//Do Nothing...
			//adjusted->x = x;
			//adjusted->y = y;
			break;

		case DMDO_90:
			//  90 degrees:
			//	(x, y) -> (xMax - x, y)
			//	(1, 1) -> (4096 - 1, 1)
			//
			//increase in x means decrease in y
			//decrease in x means increase in y
			//x = 0 -> y = 0
			//
			//	(x, y) -> (maxes.y - y, x)
			//	(1, 2) -> (4096 - 2, 1)
			//
			//   (0, yMax) *************** (0, 0)
			//             *         @   *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//(xMax, yMax) *************** (xMax, 0)
			*x = DefaultTouchPanelMaxes.y - tY;
			*y = tX;
			break;

		case DMDO_180:
			*x = DefaultTouchPanelMaxes.x - tX;
			*y = DefaultTouchPanelMaxes.y - tY;
			break;

		case DMDO_270:
			*x = tY;
			*y = DefaultTouchPanelMaxes.x - tX;
			break;

		default:
			printf("WTF orientation are you using?\n");
			return -1;
		}


		//printf("OGTouchPanel: (%4d, %4d) ", tX, tY);
		//printf("TouchPanel: (%4d, %4d) ", *x, *y);
		return 0;
	}


	int AdjustTouchOrientationOld(LONG *x, LONG *y)
	{
		LONG tX = *x;
		LONG tY = *y;


		switch (this->Orientation)
		{
		case DMDO_DEFAULT:
			//Nothing needs to be adjusted here. 
			//	0 degrees [default]:
			// 	(x, y) -> (x, y)
			//	(1, 2) -> (1, 2)
			//
			//   (0, 0) ******************* (xMax, 0)
			//          *                 *
			//          * @               *
			//          *                 *
			//          *                 *
			//          *                 *
			//(0, yMax) ******************* (xMax, yMax)

			//Do Nothing...
			//adjusted->x = x;
			//adjusted->y = y;
			break;

		case DMDO_90:
			//  90 degrees:
			//	(x, y) -> (xMax - x, y)
			//	(1, 1) -> (4096 - 1, 1)
			//
			//increase in x means decrease in y
			//decrease in x means increase in y
			//x = 0 -> y = 0
			//
			//	(x, y) -> (maxes.y - y, x)
			//	(1, 2) -> (4096 - 2, 1)
			//
			//   (0, yMax) *************** (0, 0)
			//             *         @   *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//(xMax, yMax) *************** (xMax, 0)


			*x = TouchPanelMaxes.y - tY;
			*y = tX;
			break;

		case DMDO_180:
			*x = TouchPanelMaxes.x - tX;
			*y = TouchPanelMaxes.y - tY;
			break;

		case DMDO_270:
			*x = tY;
			*y = TouchPanelMaxes.x - tX;
			break;

		default:
			printf("WTF orientation are you using?\n");
			return -1;
		}


		printf("OGTouchPanel: (%4d, %4d) ", tX, tY);
		printf("TouchPanel: (%4d, %4d) ", *x, *y);
		return 0;
	}




	// ********************** Static Functions ********************************** //
	static void Convert(POINT inputMaxes, POINT outputMaxes, POINT input, LONG *adjustedX, LONG *adjustedY)
	{
		double xRatio = ((double)outputMaxes.x) / ((double)inputMaxes.x);
		double yRatio = ((double)outputMaxes.y) / ((double)inputMaxes.y);

		*adjustedX = LONG(((double)input.x) * xRatio);
		*adjustedY = LONG(((double)input.y) * yRatio);
	}

	static POINT Convert(POINT inputMaxes, POINT outputMaxes, POINT input)
	{
		POINT output;

		double xRatio = ((double)outputMaxes.x) / ((double)inputMaxes.x);
		double yRatio = ((double)outputMaxes.y) / ((double)inputMaxes.y);

		output.x = LONG(((double)input.x) * xRatio);
		output.y = LONG(((double)input.y) * yRatio);

		//printf("Converting: [%d/%d, %d/%d] to [%d/%d, %d/%d]\n", input.x, inputMaxes.x, input.y, inputMaxes.y, output.x, outputMaxes.x, output.y, outputMaxes.y);
		//printf("Converting: [%d/%d(%.3f), %d/%d(%.3f)] to [%d/%d(%.3f), %d/%d(%.3f)]\n", input.x, inputMaxes.x, ((double) input.x / (double) inputMaxes.x), input.y, inputMaxes.y, ((double) input.y / (double) inputMaxes.y), output.x, outputMaxes.x, ((double) output.x / (double) outputMaxes.x), output.y, outputMaxes.y, ((double) output.y / (double) outputMaxes.y));

		return output;
	}

	static RECT Convert(POINT inputMaxes, POINT outputMaxes, RECT input)
	{
		RECT output;

		double xRatio = ((double)outputMaxes.x) / ((double)inputMaxes.x);
		double yRatio = ((double)outputMaxes.y) / ((double)inputMaxes.y);

		output.left = LONG(((double)input.left) * xRatio);
		output.top = LONG(((double)input.top) * yRatio);
		output.right = LONG(((double)input.right) * xRatio);
		output.bottom = LONG(((double)input.bottom) * yRatio);

		//printf("Converting: [%d/%d, %d/%d] to [%d/%d, %d/%d]\n", input.x, inputMaxes.x, input.y, inputMaxes.y, output.x, outputMaxes.x, output.y, outputMaxes.y);

		return output;
	}

	//Takes in @X and @Y and returns an @adjusted which rotates them accordingly
	static int AdjustForOrientation(POINT *adjusted, LONG x, LONG y, DWORD orientation, POINT maxes)
	{
		switch (orientation)
		{
		case DMDO_DEFAULT:
			//Nothing needs to be adjusted here. 
			//	0 degrees [default]:
			// 	(x, y) -> (x, y)
			//	(1, 2) -> (1, 2)
			//
			//   (0, 0) ******************* (xMax, 0)
			//          *                 *
			//          * @               *
			//          *                 *
			//          *                 *
			//          *                 *
			//(0, yMax) ******************* (xMax, yMax)

			adjusted->x = x;
			adjusted->y = y;
			break;

		case DMDO_90:
			//  90 degrees:
			//	(x, y) -> (xMax - x, y)
			//	(1, 1) -> (4096 - 1, 1)
			//
			//increase in x means decrease in y
			//decrease in x means increase in y
			//x = 0 -> y = 0
			//
			//	(x, y) -> (maxes.y - y, x)
			//	(1, 2) -> (4096 - 2, 1)
			//
			//   (0, yMax) *************** (0, 0)
			//             *         @   *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//(xMax, yMax) *************** (xMax, 0)
			//adjusted->x = maxes.y - y;
			adjusted->x = maxes.y - y;
			adjusted->y = x;
			break;

		case DMDO_180:
			adjusted->x = maxes.x - x;
			adjusted->y = maxes.y - y;
			break;

		case DMDO_270:
			adjusted->x = y;
			adjusted->y = maxes.x - x;
			break;

		default:
			printf("WTF orientation are you using?\n");
			adjusted->x = x;
			adjusted->y = y;
			return -1;
		}
		return 0;
	}

	static int AdjustForOrientation(LONG *adjustedX, LONG *adjustedY, LONG x, LONG y, DWORD orientation, POINT maxes)
	{
		switch (orientation)
		{
		case DMDO_DEFAULT:
			//Nothing needs to be adjusted here. 
			//	0 degrees [default]:
			// 	(x, y) -> (x, y)
			//	(1, 2) -> (1, 2)
			//
			//   (0, 0) ******************* (xMax, 0)
			//          *                 *
			//          * @               *
			//          *                 *
			//          *                 *
			//          *                 *
			//(0, yMax) ******************* (xMax, yMax)

			*adjustedX = x;
			*adjustedY = y;
			break;

		case DMDO_90:
			//  90 degrees:
			//	(x, y) -> (xMax - x, y)
			//	(1, 1) -> (4096 - 1, 1)
			//
			//increase in x means decrease in y
			//decrease in x means increase in y
			//x = 0 -> y = 0
			//
			//	(x, y) -> (maxes.y - y, x)
			//	(1, 2) -> (4096 - 2, 1)
			//
			//   (0, yMax) *************** (0, 0)
			//             *         @   *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//             *             *
			//(xMax, yMax) *************** (xMax, 0)
			//adjusted->x = maxes.y - y;
			*adjustedX = maxes.y - y;
			*adjustedY = x;
			break;

		case DMDO_180:
			*adjustedX = maxes.x - x;
			*adjustedY = maxes.y - y;
			break;

		case DMDO_270:
			*adjustedX = y;
			*adjustedY = maxes.x - x;
			break;

		default:
			printf("WTF orientation are you using?\n");
			*adjustedX = x;
			*adjustedY = y;
			return -1;
		}
		return 0;
	}


};



//Gets screen resolution. Note: The program incorporating this MUST be DPI aware. 
static POINT GetResolution()
{
	POINT res;
	res.x = GetSystemMetrics(SM_CXSCREEN);
	res.y = GetSystemMetrics(SM_CYSCREEN);
	return res;
}

//Returns screensize in Millimeters 
static POINT GetScreenSizeMM()
{
	POINT size;
	HDC screen = GetDC(NULL);
	size.x = GetDeviceCaps(screen, HORZSIZE);
	size.y = GetDeviceCaps(screen, VERTSIZE);
	return size;
}

//Returns Screen Orientation: DMDO_DEFAULT, DMDO_90, DMDO_180, or DMDO_270
DWORD GetOrientation()
{
	DEVMODE devmode;
	ZeroMemory(&devmode, sizeof(DEVMODE));
	devmode.dmSize = sizeof(DEVMODE);
	devmode.dmFields = DM_DISPLAYORIENTATION;

	//Check display orientation
	EnumDisplaySettingsEx(NULL, ENUM_CURRENT_SETTINGS, &devmode, EDS_RAWMODE);

	switch (devmode.dmDisplayOrientation)
	{
	case DMDO_DEFAULT:
		//printf("Default orientation\n");
		break;

	case DMDO_90:
		//printf("90* orientation\n");
		break;

	case DMDO_180:
		//printf("180* orientation\n");
		break;

	case DMDO_270:
		//printf("270* orientation\n");
		break;

	default:
		printf("WTF orientation are you using?\n");
		break;

	}
	return devmode.dmDisplayOrientation;
}

