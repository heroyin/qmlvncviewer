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

#include "ExternalAuthenticationDialog.h"

ExternalAuthenticationDialog::ExternalAuthenticationDialog()
: BaseDialog(IDD_AUTHLOGINPASS)
{
}


BOOL ExternalAuthenticationDialog::onInitDialog()
{
  setControlById(m_hostname, IDC_EHOST);
  m_hostname.setText(m_strHost.getString());
  setControlById(m_login, IDC_LOGIN);
  m_login.setText(m_strLogin.getString());
  setControlById(m_password, IDC_PASSWORD);
  if (m_strLogin.isEmpty()) {
      m_login.setFocus();
  } else {
    m_password.setFocus();
  }
  return FALSE;
}

void ExternalAuthenticationDialog::setHostname(const StringStorage *hostname)
{
  m_strHost = *hostname;
}

void ExternalAuthenticationDialog::setUsername(const StringStorage *username)
{
  m_strLogin = *username;
}

BOOL ExternalAuthenticationDialog::onCommand(UINT controlID, UINT notificationID)
{
  if (controlID == IDOK) {
    m_login.getText(&m_strLogin);
    m_password.getText(&m_strPassword);
    kill(1);
    return TRUE;
  }
  if (controlID == IDCANCEL) {
    kill(0);
    return TRUE;
  }
  return FALSE;
}

const StringStorage *ExternalAuthenticationDialog::getLogin() const
{
  return &m_strLogin;
}

const StringStorage *ExternalAuthenticationDialog::getPassword() const
{
  return &m_strPassword;
}
