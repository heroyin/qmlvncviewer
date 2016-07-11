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

#include "ViewerExtAuthHandler.h"
#include "ExternalAuthenticationDialog.h"

#include "rfb/VendorDefs.h"
#include "viewer-core/ExternalAuthentication.h"

ViewerExtAuthHandler::ViewerExtAuthHandler(ConnectionData *connectionData)
: m_connectionData(connectionData)
{
}

ViewerExtAuthHandler::~ViewerExtAuthHandler()
{
}

void ViewerExtAuthHandler::getLoginPassword(std::vector<UINT8> *cryptedPassword)
{
  if (!m_connectionData->isSetExtAuthData()) {
    ExternalAuthenticationDialog authDialog;
    StringStorage hostname = m_connectionData->getHost();
    StringStorage username = m_connectionData->getUsernameExt();
    authDialog.setHostname(&hostname);
    authDialog.setUsername(&username);

    if (authDialog.showModal()) {
      vector<UINT8> cryptedPassword;
      ExternalAuthentication::encryptPassword(authDialog.getLogin(),
                                              authDialog.getPassword(),
                                              &cryptedPassword);
      m_connectionData->setPasswordExt(&cryptedPassword);
    } else {
      throw AuthCanceledException();
    }
  }
  *cryptedPassword = m_connectionData->getPasswordExt();
}
