#ifndef CUSTOM_EXCEPTIONS_H
#define CUSTOM_EXCEPTIONS_H

#include "Includer.h"
#include <exception>
#include <string>
#include <map>
#include <sstream>
#include <comdef.h>
#include <optional>

using namespace std;

enum class EXCEPTIONTYPE { WINDOWS, GRAPHICS, NOGRAPHICS, DISPLAY };

/// <summary>
/// definition of type for own enum translation to string
/// </summary>
typedef map<EXCEPTIONTYPE, string> ExceptionMap;

/// <summary>
/// Custom exception class
/// </summary>
class BaseException : public exception
{
public:
	inline BaseException(size_t line, const char* file) noexcept 
		: line_(line), file_(file) {};
	char const* what();
	//Getters for exception
	virtual EXCEPTIONTYPE getType() const noexcept = 0;
	inline const string& getFile() const noexcept { return file_; }
	inline size_t getLine() const noexcept { return line_; }
	virtual optional<HRESULT> getHResult() const noexcept = 0;
protected:
	size_t line_;
	string file_;
	string exBuffer;
};

/// <summary>
/// Exception class representing exceptions thrown by window class
/// </summary>
class WindowException : public BaseException
{
public:
	inline WindowException(size_t line, const char* file, HRESULT hRes) noexcept 
		: BaseException(line, file), hResult_(hRes) {};
	inline EXCEPTIONTYPE getType() const noexcept override { return EXCEPTIONTYPE::WINDOWS; };
	inline optional<HRESULT> getHResult() const noexcept override { return hResult_; };
private:
	HRESULT hResult_;
};

/// <summary>
/// Exception class representing exceptions thrown by graphics class
/// </summary>
class GraphicsException : public BaseException
{
public:
	inline GraphicsException(size_t line, const char* file, HRESULT hRes) noexcept 
		: BaseException(line, file), hResult_(hRes) {};
	inline EXCEPTIONTYPE getType() const noexcept override { return EXCEPTIONTYPE::GRAPHICS; };
	inline optional<HRESULT> getHResult() const noexcept override { return hResult_; };
private:
	HRESULT hResult_;
};

/// <summary>
/// Exception class representing exceptions thrown by windows class without
/// </summary>
class NoGraphicsException : public BaseException
{
public:
	inline NoGraphicsException(size_t line, const char* file) noexcept
		: BaseException(line, file) {};
	inline EXCEPTIONTYPE getType() const noexcept override { return EXCEPTIONTYPE::NOGRAPHICS; };
	inline optional<HRESULT> getHResult() const noexcept override { return optional<HRESULT>(); };
};

/// <summary>
/// Exception used in GraphicsDraw object if there are some objects added to Draw stage incorrectly
/// </summary>
class GraphicsDrawException : public BaseException
{
public:
	inline GraphicsDrawException(size_t line, const char* file) noexcept 
		: BaseException(line, file) {};
	inline EXCEPTIONTYPE getType() const noexcept override { return EXCEPTIONTYPE::DISPLAY; };
	inline optional<HRESULT> getHResult() const noexcept override { return optional<HRESULT>(); };
};

static string& translateException(EXCEPTIONTYPE exType);

#endif // !CUSTOM_EXCEPTIONS_H

