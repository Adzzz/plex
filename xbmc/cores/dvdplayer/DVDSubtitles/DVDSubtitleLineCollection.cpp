/*
 *      Copyright (C) 2005-2008 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */
 
#include "stdafx.h"
#include "DVDSubtitleLineCollection.h"


CDVDSubtitleLineCollection::CDVDSubtitleLineCollection()
{
  m_pHead = 0;
  m_pCurrent = 0;
  m_pTail = 0;
  
  m_iSize = 0;
  m_fLastPts = 0.0;
}

CDVDSubtitleLineCollection::~CDVDSubtitleLineCollection()
{
  Clear();
}

void CDVDSubtitleLineCollection::Add(CDVDOverlay* pOverlay)
{
  ListElement* pElement = new ListElement;
  pElement->pOverlay = pOverlay;
  pElement->pNext = 0;

  if (!m_pHead)
  {
    m_pHead = m_pTail = pElement;
    m_pCurrent = m_pHead;
  }
  else
  {
    m_pTail->pNext = pElement;
    m_pTail = pElement;
  }
  
  m_iSize++;
}

CDVDOverlay* CDVDSubtitleLineCollection::Get(double iPts)
{
  CDVDOverlay* pOverlay = NULL;

  // If we're suddenly asked to get a subtitle for the past, reset.
  if (iPts < m_fLastPts)
    Reset();
  
  if (m_pCurrent)
  {
    while (m_pCurrent && m_pCurrent->pOverlay->iPTSStopTime < iPts)
    {
      m_pCurrent = m_pCurrent->pNext;
    }
    
    if (m_pCurrent)
    {
      pOverlay = m_pCurrent->pOverlay;

      // advance to the next overlay
      m_pCurrent = m_pCurrent->pNext;
      m_fLastPts = iPts;
    }
  }
  return pOverlay;
}

void CDVDSubtitleLineCollection::Reset()
{
  m_pCurrent = m_pHead;
}

void CDVDSubtitleLineCollection::Clear()
{
  ListElement* pElement = NULL;
  
  while (m_pHead)
  {
    pElement = m_pHead;
    m_pHead = pElement->pNext;

    pElement->pOverlay->Release();
    delete pElement;
  }
  
  m_iSize = 0;
}
