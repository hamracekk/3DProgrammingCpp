#include "Exceptions.h"

/// <summary>
/// Translates exception from enum type to string type
/// </summary>
/// <param name="exType">Enum type of exception</param>
/// <returns>String representation of the exception</returns>
static string& translateException(EXCEPTIONTYPE exType)
{
	static ExceptionMap exceptionTranslation = { 
		{ EXCEPTIONTYPE::WINDOWS, "WINDOW EXCEPTION" },
		{ EXCEPTIONTYPE::GRAPHICS, "GRAPHICS EXCEPTION" },
		{ EXCEPTIONTYPE::NOGRAPHICS, "NO GRAPHICS EXCEPTION" },
		{ EXCEPTIONTYPE::DISPLAY, "DISPLAY EXCEPTION" },
	};
	return exceptionTranslation[exType];
}

static void convertLPCWSTRToString(string& out, LPCWSTR input)
{
	wstring wstring(input);
	out = string(wstring.begin(), wstring.end());
}

/// <summary>
/// Returns brief information about exception
/// </summary>
/// <returns>Pointer to exception message</returns>
char const* BaseException::what()
{
	ostringstream ostream;
	ostream << "Exception type: " << translateException(getType()) << "!" << endl;
	ostream << "Line: " << getLine() << endl;
	ostream << "File" << getFile() << endl;

	optional<HRESULT> res = getHResult();
	if (res.has_value())
	{
		_com_error err(res.value());
		string errorString = "";
		convertLPCWSTRToString(errorString, err.ErrorMessage());
		ostream << "Message: " << errorString << endl;
	}
	exBuffer = ostream.str(); // we have to save it to buffer beacuse pointer cease to exist after function call
	return exBuffer.c_str();
};
