#ifndef WINDOW_WRAPPER_H
#define WINDOW_WRAPPER_H

#include "Includer.h"
#include "KeyboardWin.h"
#include "WinMouse.h"

const constexpr int bit30 = 0x40000000; // 30th bit in lParam

class Window
{
public:
	Window(int width, int height, std::string&& name) noexcept;
	~Window();
	Window& operator=(const Window&) = delete;
private:
	/// <summary>
	/// This Class will be singleton (private constructor)
	/// </summary>
	class WindowCore
	{
	public:
		static const char* GetName() { return coreName_; };
		static HINSTANCE GetInstance() { return windowsCore.instance_; };
	private:
		WindowCore() noexcept;
		~WindowCore() noexcept;
		WindowCore(WindowCore&) = delete; // Delteing copy constructor (singleton)
		WindowCore& operator=(WindowCore&) = delete; // Deleteing copy assign operator (singleton)
		static constexpr const char* coreName_ = "Cpp3DPrograming";
		static WindowCore windowsCore; // static beacuse we want only single instance
		HINSTANCE instance_; // handle to the window
	};
private:
	static LRESULT WINAPI StaticWindowMessageHandlerStart(HWND hHandle, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI CallWindowClassMessageHandler(HWND hHandle, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK CustomWindowProc(HWND hHandle, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	HWND handle_; // Handle to the window
	//Constants describing where should window spawn
	const size_t window_width_;
	const size_t window_height_;
	const size_t leftCorner = 200;
	const size_t topCorner = 200;
public:
	WinKeyboard keyboard;
	WinMouse mouse;
};

#endif //! WINDOW_WRAPPER_H
