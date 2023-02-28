#include "WindowWrapper.h"

/// <summary>
/// Constructor for WindowCore
/// </summary>
Window::WindowCore::WindowCore() noexcept : instance_(GetModuleHandle(NULL))
{
	WNDCLASSEXA winClassEx = { 0 };
	winClassEx.cbSize = sizeof(winClassEx); // Size of the class
	winClassEx.style = CS_OWNDC; // Window has its own device context
	winClassEx.lpfnWndProc = DefWindowProcA; // Procedure to handle windows messages
	winClassEx.cbClsExtra = 0; // Extra  bytes allocation for Class
	winClassEx.cbWndExtra = 0; // Extra bytes allocation for every window
	winClassEx.hInstance = this->GetInstance();
	winClassEx.hCursor = NULL;
	winClassEx.lpszMenuName = NULL; // Menu name of the window
	winClassEx.hbrBackground = NULL; //Brush for the background (we will use our own)
	winClassEx.hIcon = NULL; //Icon for the window
	winClassEx.lpszClassName = coreName_;

	RegisterClassExA(&winClassEx); // Registering class for creating window
}

/// <summary>
/// Destructor for WindowCore class
/// </summary>
Window::WindowCore::~WindowCore()
{
	UnregisterClassA(coreName_, instance_); // unregistering window instance
}

Window::Window(int width, int height, std::string& name) noexcept
{
	handle_ = CreateWindowA(WindowCore::GetName(), // Name of the class
		name.c_str(), // Name of the window
		WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER | WS_SYSMENU, // style of the Window
		topCorner, leftCorner, //Where to spwan the left top corner of the window
		width, height, //Width x Height
		NULL, // Handle to parent window
		NULL, // Menu handle
		WindowCore::GetInstance(), // Handle to the instance
		this); // Passing pointer to self

	//Now we display the window
	ShowWindow(handle_, SW_SHOW);
};

/// <summary>
/// Destructor for Window (destroying window)
/// </summary>
Window::~Window()
{
	DestroyWindow(handle_);
}
