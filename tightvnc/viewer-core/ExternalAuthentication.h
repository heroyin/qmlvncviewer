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

#ifndef _EXTERNAL_AUTHENTICATION_H_
#define _EXTERNAL_AUTHENTICATION_H_

#include "AuthHandler.h"

class ExternalAuthentication
{
public:
  ExternalAuthentication();
  virtual ~ExternalAuthentication();

  //
  // This method do authentication with username and password.
  //
  static void extAuthenticate(DataInputStream *input, DataOutputStream *output,
                              const vector<UINT8> *cryptedPassword);

  //
  // This algorithm generate from pair of login and password sequence of bytes:
  // length login (UINT8), length password (UINT8), login and password.
  // This sequence encrypt with DES.
  //
  static void encryptPassword(const StringStorage *username,
                              const StringStorage *password,
                              vector<UINT8> *cryptedPassword);

protected:
  //
  // Username and password is encrypted with this key.
  //
  static UINT8 key[8];

protected:
  //
  // This is crypted pair of login and password.
  // Algorithm of encryption see in encryptPassword().
  //
  vector<UINT8> m_cryptedPassword;
};

#endif
