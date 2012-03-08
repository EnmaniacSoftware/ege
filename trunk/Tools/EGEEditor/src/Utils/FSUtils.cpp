#include "FSUtils.h"
#include <QRegExp>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes invalid characters from given file name. */
QString FSUtils::ValidateFileName(const QString& fileName)
{
	// http://msdn.microsoft.com/en-us/library/aa365247.aspx
	//
	//	You may use any character in the current code page (Unicode/ANSI above 127), except:
	//
	//	< > : " / \ | ? *
	//	Characters whose integer representations are 0-31 (less than ASCII space)
	//	Any other character that the target file system does not allow (say, trailing periods or spaces)
	//	Any of the DOS names: CON, PRN, AUX, NUL, COM1, COM2, COM3, COM4, COM5, COM6, COM7, COM8, COM9, LPT1, LPT2, LPT3, LPT4, LPT5, LPT6, LPT7, LPT8, LPT9 (and avoid AUX.txt, etc)
	//	The file name is all periods

	// Just make sure "< > : " / \ | ? * \t" are removed
	QString str = fileName;
	str.replace(QRegExp(QString::fromLatin1("[\\t\\/:?\"<>|*]")), QString());
	return str;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------