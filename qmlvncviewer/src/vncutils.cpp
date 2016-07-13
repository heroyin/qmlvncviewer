#include "vncutils.h"


StringStorage qStringToStringStorage(QString string)
{
    const wchar_t*we = reinterpret_cast<const wchar_t*>(string.utf16());
    return StringStorage(we);
}

QString stringStorageToQString(const StringStorage *string)
{
    return QString::fromWCharArray(string->getString());
}
