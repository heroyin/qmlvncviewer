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

#include "WinDxCriticalException.h"
#include "WinDxRecoverableException.h"

// The header including of this cpp file must be at last place to avoid build conflicts.
#include "WinDxgiAdapter.h"

WinDxgiAdapter::WinDxgiAdapter(WinDxgiDevice *winDxgiDevice)
: m_dxgiAdapter(0)
{
  winDxgiDevice->getAdapter(&m_dxgiAdapter);
}

WinDxgiAdapter::WinDxgiAdapter(WinDxgiDevice *winDxgiDevice, int iAdapter)
: m_dxgiAdapter(0)
{
  winDxgiDevice->getAdapter(&m_dxgiAdapter);
  IDXGIFactory *m_dxgiFactory;
  HRESULT hr = m_dxgiAdapter->GetParent( __uuidof( IDXGIFactory ), (void**) (&m_dxgiFactory) );
  if (FAILED(hr)) {
    throw WinDxCriticalException(_T("Can't get IDXGIAdapter parent factory (%ld)"), (long)hr);
  }

  hr = m_dxgiFactory->EnumAdapters(iAdapter, &m_dxgiAdapter);
  m_dxgiFactory->Release();
  if (hr == DXGI_ERROR_NOT_FOUND) {
    StringStorage errMess;
    errMess.format(_T("IDXGIAdapter not found for iAdapter = %u"), iAdapter);
    throw WinDxRecoverableException(errMess.getString(), hr);
  }
  if (FAILED(hr)) {
    throw WinDxCriticalException(_T("Can't IDXGIFactory::EnumAdapters()"), hr);
  }
}

WinDxgiAdapter::~WinDxgiAdapter()
{
  if (m_dxgiAdapter != 0) {
    m_dxgiAdapter->Release();
    m_dxgiAdapter = 0;
  }
}

HRESULT WinDxgiAdapter::getDxgiOutput(UINT iOutput, IDXGIOutput **iDxgiOutput)
{
  HRESULT hr = m_dxgiAdapter->EnumOutputs(iOutput, iDxgiOutput);
  if (hr != DXGI_ERROR_NOT_FOUND && FAILED(hr)) {
    throw WinDxCriticalException(_T("Can't IDXGIAdapter::EnumOutputs()"), hr);
  }
  return hr;
}
