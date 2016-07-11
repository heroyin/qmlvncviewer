// Copyright (C) 2012 GlavSoft LLC.
// All rights reserved.
//
//-------------------------------------------------------------------------
// This file is part of the TightVNC software.  Please visit our Web site:
//
//                       http://www.tightvnc.com/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//-------------------------------------------------------------------------
//

#ifndef _EXTERNAL_AUTHENTICATION_DIALOG_H_
#define _EXTERNAL_AUTHENTICATION_DIALOG_H_

#include "gui/BaseDialog.h"
#include "gui/TextBox.h"
#include "resource1.h"

class ExternalAuthenticationDialog : public BaseDialog
{
public:
  ExternalAuthenticationDialog();

  //
  // This function returns the login entered by user.
  // Returned value must be copied: after removed object of this class,
  // pointer to this StringStorage will be incorrect.
  //
  const StringStorage *getLogin() const;

  //
  // This function returns the password entered by user.
  // Returned value must be copied: after removed object of this class,
  // pointer to this StringStorage will be incorrect.
  //
  const StringStorage *getPassword() const;

  // This function set host in login dialog.
  void setHostname(const StringStorage *hostname);

  // This function set username in login dialog.
  void setUsername(const StringStorage *username);

protected:
  BOOL onCommand(UINT controlID, UINT notificationID);
  BOOL onInitDialog();

  TextBox m_login;
  TextBox m_password;
  TextBox m_hostname;

  StringStorage m_strLogin;
  StringStorage m_strPassword;
  StringStorage m_strHost;
};

#endif
