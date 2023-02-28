#ifndef WINDOW_WRAPPER_H
#define WINDOW_WRAPPER_H

#include "Includer.h"

class Window
{
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
public:
	Window(int width, int height, std::string& name) noexcept;
	~Window();
	Window& operator=(const Window&) = delete;
private:
	LRESULT CALLBACK CustomWindowProc(HWND hHandle, UINT msg, WPARAM wParam, LPARAM lParam);
	size_t width_; // width of the window
	size_t height_; // height of the window
	HWND handle_; // Handle to the window
	//Constants describing where should window spawn
	const size_t leftCorner = 200;
	const size_t topCorner = 200;

};

#endif //! WINDOW_WRAPPER_H
