#ifndef WIN_MOUSE_H
#define WIN_MOUSE_H

#include <queue>

using namespace std;

/// <summary>
/// Enum class describing different muouse states
/// </summary>
enum class MouseState { LEFTPRESSED, LEFTRELEASED, RIGHTPRESSED, 
	                    RIGHTRELEASED, SCROLLUP, SCROLLDOWN, 
	                    MOUSEMOVE, NOTDEFINED , ENTERWINDOW, 
					    LEAVEWINDOW
					  };

/// <summary>
/// Class wrapping mouse event
/// </summary>
class MouseEvent
{
public:
	inline MouseEvent() noexcept : state_(MouseState::NOTDEFINED), x_position_(0), y_position_(0),
	                               left_pressed_(false), right_pressed_(false) {};
	inline MouseEvent(MouseState state, size_t x_pos, size_t y_pos, bool left_press, bool right_press) noexcept 
		: state_(state), x_position_(x_pos), y_position_(y_pos),
		  left_pressed_(left_press), right_pressed_(right_press) {};
	/// <summary>
	/// Getters for private MouseEvent members
	/// </summary>
	inline MouseState GetState() const noexcept { return state_; };
	inline std::pair<size_t, size_t> GetPosition() const { return make_pair(x_position_, y_position_); };
	inline bool GetLeftPressed() const noexcept { return left_pressed_; };
	inline bool GetRightPressed() const noexcept { return right_pressed_; };
private:
	MouseState state_; // Which state of mouse describe event
	size_t x_position_; // x coordinate of mouse (from left top corner)
	size_t y_position_; // y coordinate of mouse (from left top corner)
	bool left_pressed_;
	bool right_pressed_;
};

class WinMouse
{
	friend class Window;
public:
	/// <summary>
	/// Singleton approach
	/// </summary>
	WinMouse() = default;
	WinMouse(WinMouse&) = delete; // we have one mouse (no copy consturctor)
	WinMouse& operator=(const WinMouse&) = delete; // we have one mouse (no copy assign)
	/// <summary>
	/// Getters for private WinMouse members
	/// </summary>
	MouseEvent GetNextEvent() noexcept;
	inline int GetXPostion() const noexcept { return x_position_; };
	inline int GetYPostion() const noexcept { return y_position_; };
	inline bool GetLeftPressed() const noexcept { return left_pressed_; };
	inline bool GetRightPressed() const noexcept { return right_pressed_; };
	inline bool GetMouseIsInWindow() const noexcept { return in_window_; }
	inline bool BufferIsEmpty() const noexcept { return eventQueue_.empty(); };
	inline void Cleare() noexcept { eventQueue_ = queue<MouseEvent>(); }; // Cleraring the queue
private:
	void OnMouseMoveEvent(int x_pos, int y_pos) noexcept;
	void OnLeftPressedEvent(int x_pos, int y_pos) noexcept;
	void OnRightPressedEvent(int x_pos, int y_pos) noexcept;
	void OnLeftReleaseEvent(int x_pos, int y_pos) noexcept;
	void OnRightReleaseEvent(int x_pos, int y_pos) noexcept;
	void OnScrollUpEvent(int x_pos, int y_pos) noexcept;
	void OnScrollDownEvent(int x_pos, int y_pos) noexcept;
	void OnEnterWindowEvent() noexcept;
	void OnLeaveWindowEvent() noexcept;
	void CutBuffer();
private:
	static constexpr unsigned int buffer_size_ = 16u; // fixed maximal size of queue
	int x_position_; // current x coordinate of mouse (from left top corner)
	int y_position_; // current x coordinate of mouse (from left top corner)
	bool left_pressed_ = false; // current press state of left mouse button
	bool right_pressed_ = false; // current press state ofright mouse button
	bool in_window_ = false; // is mouse inside of the window ?
	std::queue<MouseEvent> eventQueue_; // queue for mouse events
};

#endif // !WIN_MOUSE_H

