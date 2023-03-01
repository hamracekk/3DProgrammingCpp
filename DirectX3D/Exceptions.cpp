#include "Exceptions.h"

/// <summary>
/// Returns brief information about exception
/// </summary>
/// <returns>Pointer to exception message</returns>
const char* WindowException::what()
{
	ostringstream ostream;
	ostream << "Exception type: " << translateException(this->getType()) << "!" << endl;
	ostream << "Line: " << getLine() << endl;
	ostream << "File" << getFile() << endl;
	exBuffer = ostream.str(); // we have to save it to buffer beacuse pointer cease to exist after function call
	return exBuffer.c_str();
}

/// <summary>
/// Translates exception from enum type to string type
/// </summary>
/// <param name="exType">Enum type of exception</param>
/// <returns>String representation of the exception</returns>
static string& translateException(EXCEPTIONTYPE exType)
{
	static ExceptionMap exceptionTranslation = { { EXCEPTIONTYPE::WINDOWS, "WINDOW EXCEPTION" } };
	return exceptionTranslation[exType];
}
