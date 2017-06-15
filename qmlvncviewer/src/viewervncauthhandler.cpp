#include "viewervncauthhandler.h"

ViewerVncAuthHandler::ViewerVncAuthHandler()
{

}

ViewerVncAuthHandler::~ViewerVncAuthHandler()
{

}

void ViewerVncAuthHandler::setPassword(std::wstring passString)
{
    m_password = passString;
}

void ViewerVncAuthHandler::getPassword(StringStorage *passString)
{
    passString->appendString(m_password.c_str());
}
