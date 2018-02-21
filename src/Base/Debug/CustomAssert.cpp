#include <vector>

#include "Utils\EnumToString.h"
#include "Utils\Logger\Logger.h"
#include "Utils\StringUtils.h"

bool CustomAssertFunction(bool expr, int line, char* file, bool* ignoreAlways, const char* format, ...)
{
	if (expr || *ignoreAlways)
		return false;

	va_list args;
	char* buffer;
	va_start(args, format);
	int len = _vscprintf(format, args) + 1;
	buffer = (char*)malloc(len * sizeof(char));
	vsprintf_s(buffer, len, format, args);
	std::string lMsg;
	base::utils::FormatString(lMsg, "%s\n", buffer );
	OutputDebugString(lMsg.c_str());
	free(buffer);
	va_end(args);

	bool lOk = true;

	std::string fileAndLine = base::utils::GetFilename(file);
	base::utils::FormatString(fileAndLine, "%s at line %d", fileAndLine.c_str(), line);

	int msgboxID = MessageBox(
		NULL,
		lMsg.c_str(),
		fileAndLine.c_str(),
		MB_ICONWARNING | MB_ABORTRETRYIGNORE | MB_DEFBUTTON2
		);

	switch (msgboxID)
	{
	case IDABORT:
		ExitProcess(0);
		break;
	case IDRETRY:
		lOk = true;
		// TODO: add code
		break;
	case IDIGNORE:
		*ignoreAlways = true;
		lOk = false;
		break;
	}

	return lOk;
}