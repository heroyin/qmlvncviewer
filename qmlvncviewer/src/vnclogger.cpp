#include "vnclogger.h"

#include <QDebug>

VncLogger::VncLogger()
{

}

void VncLogger::print(int logLevel, const TCHAR *line)
{
    qDebug() << QString::fromWCharArray(line);
}

bool VncLogger::acceptsLevel(int logLevel)
{
    return true;
}
