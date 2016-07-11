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

#ifndef __WINDXGIADAPTER_H__
#define __WINDXGIADAPTER_H__

#include "WinDxgiDevice.h"

#include <d3d11.h>
#include <DXGI1_2.h>

class WinDxgiAdapter
{
public:
  // Refers to default adapter for the device
  WinDxgiAdapter(WinDxgiDevice *winDxgiDevice);
  // This constructor first finds parent factory for default 
  // adapter and use it to get adapter with iAdapter number.
  // Throws WinDxRecoverableException if there is 
  // no adapter with iAdapter number.
  // Throws WinDxCriticalException on other errors.
  WinDxgiAdapter(WinDxgiDevice *winDxgiDevice, int iAdapter);
  virtual ~WinDxgiAdapter();

  // This function try to get output for iOutput from the adapter.
  // Throws WinDxCriticalException on errors.
  // Returns S_OK on success and DXGI_ERROR_NOT_FOUND if there is 
  // no output with iOutput number
  HRESULT getDxgiOutput(UINT iOutput, IDXGIOutput **iDxgiOutput);

private:
  IDXGIAdapter *m_dxgiAdapter;
};

#endif // __WINDXGIADAPTER_H__
