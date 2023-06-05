/*
 * CANAL interface DLL for RUSOKU technologies for TouCAN, TouCAN Marine, TouCAN Duo USB to CAN bus converter
 *
 * Copyright (C) 2000-2008 Ake Hedman, eurosource, <akhe@eurosource.se>
 * Copyright (C) 2005-2023 Gediminas Simanskis (gediminas@rusoku.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 3.0 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.
 *
 */

#include "windows.h"
#include "include/CDllDrvObj.h"
#include "include/canal_macro.h"

/////////////////////////////////////////////////////////////////////////////
// CDllDrvObj

////////////////////////////////////////////////////////////////////////////
// CDllDrvObj construction

CDllDrvObj::CDllDrvObj()
{
	m_instanceCounter = 0;

	m_objMutex = CreateMutex(nullptr, true, nullptr);

	// Init the driver array
	for (int i = 0; i<CANAL_TouCAN_DRIVER_MAX_OPEN; i++)
    {
		m_drvObjArray[i] = nullptr;
	}

	UNLOCK_MUTEX(m_objMutex);
}


CDllDrvObj::~CDllDrvObj()
{
	LOCK_MUTEX(m_objMutex);

	for (int i = 0; i<CANAL_TouCAN_DRIVER_MAX_OPEN; i++)
	{

		if (nullptr == m_drvObjArray[i])
		{

			CTouCANObj *pdrvObj = GetDriverObject(i);

			if (nullptr != pdrvObj)
			{
				pdrvObj->Close();
				delete m_drvObjArray[i];
				m_drvObjArray[i] = nullptr;
			}
		}
	}

	UNLOCK_MUTEX(m_objMutex);
	CloseHandle(m_objMutex);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDllDrvObjgerdllApp object

//CDllDrvObj theApp;



///////////////////////////////////////////////////////////////////////////////
// addDriverObject
//

long CDllDrvObj::AddDriverObject(CTouCANObj *pdrvObj)
{
	long h = 0;

	LOCK_MUTEX(m_objMutex);

	for (int i = 0; i<CANAL_TouCAN_DRIVER_MAX_OPEN; i++)
	{
		if (m_drvObjArray[i] == nullptr)
		{
			m_drvObjArray[i] = pdrvObj;
			h = i + 1681;
			break;
		}
	}

	UNLOCK_MUTEX(m_objMutex);
	return h;
}


///////////////////////////////////////////////////////////////////////////////
// getDriverObject
//

CTouCANObj * CDllDrvObj::GetDriverObject(long h)
{
	long idx = h - 1681;

	// Check if valid handle
	if (idx < 0) return nullptr;
	if (idx >= CANAL_TouCAN_DRIVER_MAX_OPEN) return nullptr;
	return m_drvObjArray[idx];
}


///////////////////////////////////////////////////////////////////////////////
// removeDriverObject
//

void CDllDrvObj::RemoveDriverObject(long h)
{
	long idx = h - 1681;

	// Check if valid handle
	if (idx < 0) return;
	if (idx >= CANAL_TouCAN_DRIVER_MAX_OPEN) return;

	LOCK_MUTEX(m_objMutex);

	if (nullptr != m_drvObjArray[idx])
		delete m_drvObjArray[idx];

	m_drvObjArray[idx] = nullptr;

	UNLOCK_MUTEX(m_objMutex);
}

///////////////////////////////////////////////////////////////////////////////
// InitInstance

BOOL CDllDrvObj::InitInstance()
{
	m_instanceCounter++;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//    removeAllObjects
//

BOOL CDllDrvObj::RemoveAllObjects()
{
	long h = 0;

	LOCK_MUTEX(m_objMutex);

	for (int i = 0; i<CANAL_TouCAN_DRIVER_MAX_OPEN; i++)
	{

		if (nullptr == m_drvObjArray[i])
		{

			CTouCANObj *pdrvObj = GetDriverObject(i);

			if (nullptr != pdrvObj)
			{
				pdrvObj->Close();
				delete m_drvObjArray[i];
				m_drvObjArray[i] = nullptr;
			}
		}

	}

	UNLOCK_MUTEX(m_objMutex);
	return TRUE;
}

