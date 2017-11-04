/** Based on "Bluegigas Bluetooth Smart Demo Application"
  * NOTE: lacks ifdefs and only works under windows */
//
// Bluegigas Bluetooth Smart Demo Application
// Contact: support@bluegiga.com.
//
// This is free software distributed under the terms of the MIT license reproduced below.
//
// Copyright (c) 2012, Bluegiga Technologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
//

#include <stdio.h>

#include "BluegigaUART.h"
#include <logger.h>

#ifdef _MSC_VER
#include <windows.h>
#define snprintf _snprintf

void usleep(__int64 usec)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
#endif
#include <Setupapi.h>


HANDLE serial_handle = INVALID_HANDLE_VALUE;

const int max_open_attempts = 5;


BluegigaUARTHandler::BluegigaUARTHandler()
{

}

BluegigaUARTHandler::~BluegigaUARTHandler()
{
    close();
}

std::list<int> BluegigaUARTHandler::list_devices() const
{
    LOG_FUNC_BEGIN
    std::list< int > COMs;
    char name[]="Bluegiga Bluetooth Low Energy";

    BYTE* pbuf = NULL;
    DWORD reqSize = 0;
    DWORD n=0;
    HDEVINFO hDevInfo;
    //guid for ports
    static const GUID guid = { 0x4d36e978, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } };
    char *str;
    char tmp[MAX_PATH+1];
    int i;
    SP_DEVINFO_DATA DeviceInfoData;

    snprintf(tmp,MAX_PATH,"%s (COM%%d)",name);

    DeviceInfoData.cbSize=sizeof(SP_DEVINFO_DATA);
    hDevInfo = SetupDiGetClassDevs(&guid,   //Retrieve all ports
                                   0L,
                                   NULL,
                                   DIGCF_PRESENT );

    if(hDevInfo==INVALID_HANDLE_VALUE)
    {
        LOG_INFO(COMs.size())
        return COMs;
    }
    while(1)
    {

        if(!SetupDiEnumDeviceInfo(
                hDevInfo,
                n++,
                &DeviceInfoData
        ))
        {
            SetupDiDestroyDeviceInfoList(hDevInfo);
            LOG_INFO(COMs.size())
            return COMs;
        }
        reqSize = 0;
        SetupDiGetDeviceRegistryPropertyA(hDevInfo, &DeviceInfoData, SPDRP_FRIENDLYNAME, NULL, NULL, 0, &reqSize);
        pbuf = (BYTE*)malloc(reqSize>1?reqSize:1);
        if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo, &DeviceInfoData, SPDRP_FRIENDLYNAME, NULL, pbuf, reqSize, NULL))
        {
            free(pbuf);
            continue;
        }
        str = (char*)pbuf;
        if(sscanf(str,tmp,&i)==1)
        {
            COMs.push_back(i);
            printf("%s\n", str);
            // Hrmm, seems to have been QT code here at some point...
            //emit DeviceFound(str,QString("\\\\.\\COM%1").arg(i));
        }
        else
        {

        }
        free(pbuf);
    }
    LOG_INFO(COMs.size())
    return COMs;

    LOG_FUNC_END
}

bool BluegigaUARTHandler::close_and_open(int _com)
{
    // close any connection
    uart_close();

    // try to connect
    int n_attempts = 0;
    do {
        usleep(500000); // 0.5s
        n_attempts++;
    } while (!uart_open(_com) && n_attempts < max_open_attempts);

    return handle_ok();
}

bool BluegigaUARTHandler::uart_open(int _com)
{
    char str[20];

    snprintf(str,sizeof(str)-1,"\\\\.\\COM%d",_com);
    serial_handle = CreateFileA(str,
                                GENERIC_READ | GENERIC_WRITE,
                                FILE_SHARE_READ|FILE_SHARE_WRITE,
                                NULL,
                                OPEN_EXISTING,
                                0,//FILE_FLAG_OVERLAPPED,
                                NULL);

    if (serial_handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    return true;

}

bool BluegigaUARTHandler::feeling_lucky_open()
{
    // close any connection
    uart_close();

    // list devices
    std::list< int > coms(list_devices());

    if( coms.empty())
        return false;
    return close_and_open(coms.front());
}

void BluegigaUARTHandler::close()
{
    uart_close();
}

void BluegigaUARTHandler::uart_close()
{
    CloseHandle(serial_handle);
    serial_handle = INVALID_HANDLE_VALUE;
}

bool BluegigaUARTHandler::handle_ok() const
{
    if( serial_handle != INVALID_HANDLE_VALUE) // handle should be OK
    {
         DWORD flags;
        // test: can we get information?
        if( GetHandleInformation(serial_handle, &flags) != 0)
            return true;
    }
    return false;
}

bool BluegigaUARTHandler::tx(int len, unsigned char *data)
{
    bool ok(false);
    DWORD r,written;
    while(len)
    {

        ok = WriteFile (serial_handle,
                data,
                len,
                &written,
                NULL ) != 0;
        if(!ok)
        {
            return false;
        }
        len-=written;
        data+=len;
    }

    return true;
}

BluegigaUARTHandler::RXStatus BluegigaUARTHandler::rx(int len, unsigned char *data, int timeout_ms)
{
    int l=len;
    bool ok(false);
    DWORD rread;
    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout=MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier=0;
    timeouts.ReadTotalTimeoutConstant=timeout_ms;
    timeouts.WriteTotalTimeoutMultiplier=0;
    timeouts.WriteTotalTimeoutConstant=0;

    SetCommTimeouts(
            serial_handle,
            &timeouts
    );
    while(len)
    {
        ok = ReadFile (serial_handle,
                data,
                len,
                &rread,
                NULL ) != 0;

        if(!ok)
        {
            l=GetLastError();
            if(l==ERROR_SUCCESS)
                return RX_TIMEOUT;
            return RX_ERROR;
        }
        else
        {
            // nothing to read, done
            if(rread==0)
                return RX_TIMEOUT;
        }
        len-=rread;
        data+=len;
    }

    return RX_READ_ALL_DATA;
}
