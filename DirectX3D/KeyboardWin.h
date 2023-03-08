#ifndef KEYBOARD_WIN_H
#define KEYBOARD_WIN_H

#include <queue>
#include <bitset>

using namespace std;
/// <summary>
/// Different states for Keys on keyboard
/// </summary>
enum class KeyState { PRESSED, RELEASED, NOSTATE };

/// <summary>
/// Event holding information about one keyboard event
/// </summary>
class KeyEvent
{
public:
	inline KeyEvent() noexcept : state_(KeyState::NOSTATE) {};
	inline KeyEvent(KeyState state, unsigned char code) noexcept : state_(state), code_(code) {};
	inline bool isPressedEvent() const { return state_ == KeyState::PRESSED; };
	inline bool isReleasedEvent() const { return state_ == KeyState::RELEASED; };
	inline bool isNoStateEvent() const { return state_ == KeyState::NOSTATE; };
	inline unsigned char GetEventCode() const { return code_; };
private:
	KeyState state_;
	unsigned char code_;
};

/// <summary>
/// Class handling keyboard
/// </summary>
class WinKeyboard
{
public:
	WinKeyboard() = default;
	WinKeyboard(WinKeyboard&) = delete; // we have one keyboard (no copy consturctor)
	WinKeyboard& operator=(const WinKeyboard&) = delete; // we have one keyboard (no copy assign)
	KeyEvent GetNextEvent() noexcept;
	inline bool KeyPressed(unsigned char key) const noexcept { return keybitset_[key]; };
	inline bool KeyEmpty() const noexcept { return eventQueue_.empty(); };
	void DelKey() noexcept { eventQueue_ = queue<KeyEvent>(); } // Clearing of event queue
	char ReadNextChar() noexcept;
	inline bool CharEmpty() const noexcept { return charQueue_.empty(); };
	void DelChar() noexcept { charQueue_ = queue<char>(); }; // Clearing of character buffer
	void DeleteAll() noexcept;
	void AutoRepeatOn() noexcept { autoRepeat_ = true; };
	void AutoRepeatOff() noexcept { autoRepeat_ = false; };
	bool IsAutoRepeat() const noexcept { return autoRepeat_; };
private: //Those functions are visible only for windows APIs
	void OnKeyPressEvent(unsigned char key) noexcept;
	void OnKeyReleaseEvent(unsigned char key) noexcept;
	void OnCharEvent(char ch) noexcept;
	void Cleare() noexcept { keybitset_.reset(); }; // reseting state of the keyboard
	template<typename TYPE>
	void CutBuffer(std::queue<TYPE>& buffer) noexcept;
public:
	friend class Window; // windows should use private methods of keyboard
private:
	static constexpr unsigned int keys_ = 256u; // maximum amout of virtual keycodes (bitarray for keyboard)
	static constexpr unsigned int buffSize = 16u;
	bool autoRepeat_ = false;
	bitset<keys_> keybitset_;
	queue<KeyEvent> eventQueue_;
	queue<char> charQueue_;
};

/// <summary>
/// Function for hanfling proper size of queues
/// </summary>
/// <typeparam name="TYPE">Type of queue</typeparam>
/// <param name="buffer">buffer to be cleared/trimmed</param>
template<typename TYPE>
inline void WinKeyboard::CutBuffer(std::queue<TYPE>& buffer) noexcept
{
	while (buffer.size() > buffSize)
	{
		buffer.pop();
	}
};

#endif // !KEYBOARD_WIN_H
