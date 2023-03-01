#include "WindowWrapper.h"

/// <summary>
/// Entry pointz of the program
/// </summary>
/// <param name="hInst">Handle to current instance of application</param>
/// <param name="hPrevInst">This param is always NULL</param>
/// <param name="cmdLine">Command line paramters in one string</param>
/// <param name="cmdShow">How should be windows shown ?</param>
/// <returns>Response code</returns>
int CALLBACK WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow)
{
	try
	{
		Window window(500, 500, "Some good stuff is here");

		//Messages handling
		MSG message;
		BOOL result;
		while (result = GetMessage( // Getting message from queue 
			&message, // pointer to massge to be filled
			NULL, //If NULL is specfidie we get all messages fro current thread
			0, 0) /*both 0 means that we want all messages in queue*/ > 0)
		{
			TranslateMessage(&message); // Translation for WM_CHAR messages
			DispatchMessage(&message); // This will send message to windowProcedure, which is handling messages
		}

		if (result == -1)
		{
			return -1;
		}

		return message.wParam;
	}
	catch (WindowException& e)
	{
		MessageBoxA(NULL, e.what(), "WinExcpetion", MB_OK);
	}
	catch (const exception& e)
	{
		MessageBoxA(NULL, e.what(), "Standard library exception", MB_OK);
	}
	catch (...)
	{
		MessageBoxA(NULL, "Some undefined Exception", "UndefException", MB_OK);
	}
	return -1;
}