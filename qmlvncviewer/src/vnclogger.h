#ifndef VNCLOGGER_H
#define VNCLOGGER_H

#include "log-writer/Logger.h"
#include "log-writer/LogWriter.h"

class VncLogger: public Logger
{
public:
    VncLogger();

    void print(int logLevel, const TCHAR *line);
    bool acceptsLevel(int logLevel);
};

#endif // VNCLOGGER_H
