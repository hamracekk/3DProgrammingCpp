#include "Application.h"

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
		Application app {};
		app.Run();
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