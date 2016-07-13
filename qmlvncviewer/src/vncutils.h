#ifndef VNCUTILS_H
#define VNCUTILS_H

#include "util/StringStorage.h"

#include <QString>

StringStorage qStringToStringStorage(QString string);
QString stringStorageToQString(const StringStorage *string);


#endif // VNCUTILS_H
