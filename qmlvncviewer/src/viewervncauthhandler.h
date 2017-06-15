#ifndef VIEWERVNCAUTHHANDLER_H
#define VIEWERVNCAUTHHANDLER_H

#include "viewer-core/VncAuthenticationHandler.h"

class ViewerVncAuthHandler : public VncAuthenticationHandler
{
public:
  ViewerVncAuthHandler();
  virtual ~ViewerVncAuthHandler();

  void setPassword(std::wstring passString);
private:
  virtual void getPassword(StringStorage *passString);

  std::wstring m_password;
};

#endif // VIEWERVNCAUTHHANDLER_H
