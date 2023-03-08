#include "WindowWrapper.h"

Window::WindowCore Window::WindowCore::windowsCore;

/// <summary>
/// Constructor for WindowCore
/// </summary>
Window::WindowCore::WindowCore() noexcept : instance_(GetModuleHandle(NULL))
{
	WNDCLASSEXA winClassEx = { 0 };
	winClassEx.cbSize = sizeof(winClassEx); // Size of the class
	winClassEx.style = CS_OWNDC; // Window has its own device context
	winClassEx.lpfnWndProc = StaticWindowMessageHandlerStart; // Procedure to handle windows messages
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

Window::Window(int width, int height, std::string&& name) noexcept : window_width_(width), window_height_(height)
{
	handle_ = CreateWindowA(WindowCore::GetName(), // Name of the class
		name.c_str(), // Name of the window
		WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER | WS_SYSMENU, // style of the Window
		topCorner, leftCorner, //Where to spwan the left top corner of the window
		width, height, //Width x Height
		NULL, // Handle to parent window
		NULL, // Menu handle
		WindowCore::GetInstance(), // Handle to the instance
		this); // Passing pointer to self -> this is connection to windows api msghandler

	//Now we display the window
	ShowWindow(handle_, SW_SHOW);
};

/// <summary>
/// Destructor for Window (destroying window)
/// </summary>
Window::~Window()
{
	DestroyWindow(handle_);
};

/// <summary>
/// Static Set-Up window procedure function (which is passed to WINCLASSEX), which handles window messages
/// </summary>
/// <param name="hHandle"> is a handle to the window</param>
/// <param name="msg">is the message code; for example, the WM_SIZE message indicates the window was resized.</param>
/// <param name="wParam">Contains additional data that pertains to the message. The exact meaning depends on the message code.</param>
/// <param name="lParam">Contains additional data that pertains to the message. The exact meaning depends on the message code.</param>
/// <returns>Result of the message processing</returns>
LRESULT __stdcall Window::StaticWindowMessageHandlerStart(HWND hHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE) // there we will use Window pointer passed to the CreatWindowA
	{
		//Extracting window class from lParam
		const CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* const pWindow = static_cast<Window*>(createStruct->lpCreateParams);

		//Storing pointer to Window in user data so we link Winodw API to window class
		SetWindowLongPtr(hHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		//Now we will set message procedure to be that one from Window class
		SetWindowLongPtr(hHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::CallWindowClassMessageHandler));
		//From now called window procedure will be CallWindowClassMessageHandler
		//forwarding to window class -> calling window procedure
		return pWindow->CustomWindowProc(hHandle, msg, wParam, lParam);
	}
	return DefWindowProc(hHandle, msg, wParam, lParam); // else use default window procedure
}

/// <summary>
/// Static redirecting window procedure function to our customWindowProc (which is passed to WINCLASSEX), which handles window messages
/// </summary>
/// <param name="hHandle"> is a handle to the window</param>
/// <param name="msg">is the message code; for example, the WM_SIZE message indicates the window was resized.</param>
/// <param name="wParam">Contains additional data that pertains to the message. The exact meaning depends on the message code.</param>
/// <param name="lParam">Contains additional data that pertains to the message. The exact meaning depends on the message code.</param>
/// <returns>Result of the message processing</returns>
LRESULT __stdcall Window::CallWindowClassMessageHandler(HWND hHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//First we will get pointer to our window from user data that we used for storing pointer to window in previous call
	Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hHandle, GWLP_USERDATA));
	//forwarding wo our procedure
	return pWindow->CustomWindowProc(hHandle, msg, wParam, lParam);
}

/// <summary>
/// Our custom Window procedure function, which handles window messages
/// </summary>
/// <param name="hHandle"> is a handle to the window</param>
/// <param name="msg">is the message code; for example, the WM_SIZE message indicates the window was resized.</param>
/// <param name="wParam">Contains additional data that pertains to the message. The exact meaning depends on the message code.</param>
/// <param name="lParam">Contains additional data that pertains to the message. The exact meaning depends on the message code.</param>
/// <returns>Result of the message processing</returns>
LRESULT Window::CustomWindowProc(HWND hHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	unsigned int key = static_cast<unsigned char>(wParam); // in wParam is tored information about key state
	const POINTS point = MAKEPOINTS(lParam); // lParam contains mouse coordinates

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69); // If message is of type of CLOSE -> than exit application, wParam = 69 -> exit code
		return 0;
		break;
	case WM_KILLFOCUS:
		keyboard.Cleare(); // If window losw focus we cleare keyboard current states
		break;
	case WM_KEYDOWN: // This is applied only for no system key (like alt , f1 .. )
	case WM_SYSKEYDOWN: // This is applied only for system key
		if ((lParam & bit30) || keyboard.IsAutoRepeat())
		{
			keyboard.OnKeyPressEvent(key); // handling Pressed key event
		}
		break;
	case WM_KEYUP:
		keyboard.OnKeyReleaseEvent(key); // Handling Released key event
		break;
	case WM_CHAR: // handling text input
		keyboard.OnCharEvent(key);
		break;
	/// <summary>
	/// Here we are going to handle mouse messages
	/// </summary>
	case WM_LBUTTONDOWN:
		mouse.OnLeftPressedEvent(point.x, point.y);
		break;
	case WM_LBUTTONUP:
		mouse.OnLeftReleaseEvent(point.x, point.y);
		break;
	case WM_RBUTTONDOWN:
		mouse.OnRightPressedEvent(point.x, point.y);
		break;
	case WM_RBUTTONUP:
		mouse.OnRightReleaseEvent(point.x, point.y);
		break;
	case WM_MOUSEMOVE:
		if (point.x >= 0 && point.x < window_width_ 
			&& point.y >= 0 && point.y < window_height_) // if mouse is not inside of the window
		{
			mouse.OnMouseMoveEvent(point.x, point.y);
			if (!mouse.GetMouseIsInWindow()) // if was not inside of the window
			{
				SetCapture(handle_); // setting capture for our window
				mouse.OnEnterWindowEvent();
			}
		}
		else
		{
			if( mouse.GetLeftPressed() || mouse.GetRightPressed()) // outside of the window but still pressed some button
				mouse.OnMouseMoveEvent(point.x, point.y);
			else // both buttons are up
			{
				ReleaseCapture(); // ele we release capture on mouse
				mouse.OnLeaveWindowEvent();
			}
		}
		break;
	case WM_MOUSEHWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) // in this case wParam conatins information about mouse scroll
			mouse.OnScrollUpEvent(point.x, point.y);
		else if(GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			mouse.OnScrollDownEvent(point.x, point.y);
		break;
	}
	return DefWindowProc(hHandle, msg, wParam, lParam);
}
