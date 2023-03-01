#ifndef CUSTOM_EXCEPTIONS_H
#define CUSTOM_EXCEPTIONS_H

#include "Includer.h"
#include <exception>
#include <string>
#include <map>
#include <sstream>

using namespace std;

enum class EXCEPTIONTYPE { WINDOWS };

/// <summary>
/// definition of type for own enum translation to string
/// </summary>
typedef map<EXCEPTIONTYPE, string> ExceptionMap;

/// <summary>
/// Costom exception class
/// </summary>
/// 
class BaseException : public exception
{
public:
	inline BaseException(size_t line, const char* file) noexcept : line_(line), file_(file) {};
	virtual EXCEPTIONTYPE getType() const noexcept;
	virtual char const* what();
	inline const string& getFile() const noexcept { return file_; }
	inline size_t getLine() const noexcept { return line_; }

protected:
	size_t line_;
	string file_;
};

class WindowException : public BaseException
{
public:
	inline WindowException(size_t line, const char* file, HRESULT hRes) noexcept : BaseException(line, file), hResult_(hRes) {};
	inline EXCEPTIONTYPE getType() const noexcept override { return EXCEPTIONTYPE::WINDOWS; };
	const char* what() override;
private:
	HRESULT hResult_;
	string exBuffer;
};

static string& translateException(EXCEPTIONTYPE exType);

#endif // !CUSTOM_EXCEPTIONS_H

