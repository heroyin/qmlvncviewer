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

#include "ExternalAuthentication.h"

#include "util/AnsiStringStorage.h"
#include "util/DesCrypt.h"

UINT8 ExternalAuthentication::key[8] = {11,110,60,254,61,210,245,92};

ExternalAuthentication::ExternalAuthentication()
{
}

ExternalAuthentication::~ExternalAuthentication()
{
}

void ExternalAuthentication::extAuthenticate(DataInputStream *input, DataOutputStream *output,
                                             const vector<UINT8> *cryptedPassword)
{
  // Send authentication data
  if (!cryptedPassword->empty()) {
    output->writeFully(&cryptedPassword->front(), cryptedPassword->size());
  }
  output->flush();
}

void ExternalAuthentication::encryptPassword(const StringStorage *username,
                                             const StringStorage *password,
                                             vector<UINT8> *cryptedPassword)
{
  // Prepare data for authentication
  const int MAX_LENGTH = 255;

  StringStorage truncatedUsername;
  StringStorage truncatedPassword;
  username->getSubstring(&truncatedUsername, 0, MAX_LENGTH - 1);
  password->getSubstring(&truncatedPassword, 0, MAX_LENGTH - 1);

  AnsiStringStorage usernameAnsi(&truncatedUsername);
  AnsiStringStorage passwordAnsi(&truncatedPassword);

  UINT8 usernameLen = static_cast<UINT8>(usernameAnsi.getLength());
  UINT8 passwordLen = static_cast<UINT8>(passwordAnsi.getLength());

  vector<UINT8> usernameBytes(usernameLen);
  vector<UINT8> passwordBytes(passwordLen);

  usernameBytes.assign(usernameAnsi.getString(), usernameAnsi.getString() + usernameLen);
  passwordBytes.assign(passwordAnsi.getString(), passwordAnsi.getString() + passwordLen);

  size_t bufferLength = (usernameLen + passwordLen + 7) & 0xFFFFFFF8;
  cryptedPassword->resize(2 + bufferLength);
  cryptedPassword->operator [](0) = usernameLen;
  cryptedPassword->operator [](1) = passwordLen;

  vector<UINT8> buffer(bufferLength);
  copy(usernameBytes.begin(), usernameBytes.end(), buffer.begin());
  copy(passwordBytes.begin(), passwordBytes.end(), buffer.begin() + usernameLen);
  fill(buffer.begin() + usernameLen + passwordLen, buffer.end(), '\0');

  // Encrypt the username/password pair
  if (bufferLength > 0) {
    DesCrypt desCrypt;
    desCrypt.encrypt(&buffer.front(),
                     &buffer.front(),
                     buffer.size(),
                     key);
    copy(buffer.begin(), buffer.end(), cryptedPassword->begin() + 2);
  }

  // Lose the passwords from memory
  truncatedPassword = _T("");
  passwordAnsi = "";
  fill(passwordBytes.begin(), passwordBytes.end(), 0);
  fill(buffer.begin(), buffer.end(), 0);
}
