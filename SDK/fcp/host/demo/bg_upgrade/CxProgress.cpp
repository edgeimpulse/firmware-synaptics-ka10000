/*----------------------------------------------------------------------------
 * CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY
 *
 * The information contained in this source code file
 * is strictly confidential and proprietary to Conexant Systems, Inc.
 * ("Conexant")
 *
 * No part of this file may be possessed, reproduced or distributed, in
 * any form or by any means for any purpose, without the express written
 * permission of Conexant Systems Inc.
 *
 * Except as otherwise specifically provided through an express agreement
 * with Conexant that governs the confidentiality, possession, use
 * and distribution of the information contained in this file, CONEXANT
 * PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
 * WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 * TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
 * AND REPRESENTATIONS.  IN NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
 * ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
 *
 * Copyright (C) 2009-2013 Conexant Systems Inc.
 * All rights reserved.
 *
 *----------------------------------------------------------------------------
 *
 * File: CxProgress.cpp
 *
 * Description: Character based Progress Bar (for tools)
 *
 *----------------------------------------------------------------------------
 */
//#include <i2c.h>

#include "CxProgress.h"

#include <Windows.h>
#include <WinIoCtl.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>
#pragma warning(disable:4244 4242 4715 ) 
#include <conio.h>

/// These function are for showing the progress of FW download,
typedef enum I2C_STATE
{
    I2C_OK,
    I2C_ERR,
    I2C_RETRY,
    I2C_ERASE_CHIPS
};

/// These function are for showing the progress of FW download,
HANDLE g_hStdout = NULL;
CONSOLE_SCREEN_BUFFER_INFO g_csbiInfo;

static long bQuitWaiting = 0;
static HANDLE hWaitingThread = NULL;

unsigned __stdcall ShowWaitingCursor(void *pArguments)
{
    long *quit = (long *)pArguments;
    unsigned int i = 0, j = 0;
    for (; !InterlockedExchange(quit, *quit);)
    {
        Sleep(100);
        if (i % 2 == 0)
        {
            _putch(_T('\b'));
            switch (j % 4)
            {
            case 0:
                _putch(_T('-'));
                break;
            case 1:
                _putch(_T('\\'));
                break;
            case 2:
                _putch(_T('|'));
                break;
            case 3:
                _putch(_T('/'));
                break;
            }
            j++;
        }
    }
    COORD pos;
    pos.X = 16;
    pos.Y = g_csbiInfo.dwCursorPosition.Y;
    SetConsoleCursorPosition(g_hStdout, pos);

    _putch(_T('D'));
    _putch(_T('o'));
    _putch(_T('n'));
    _putch(_T('e'));

    _endthreadex(0);
    return 0;
}

void ShowProgress(int curPos, int bForceRedraw, int eState, const int MaxPos)
{
    const int MAX_POS = MaxPos;
    static int PrePos = 1;
    static int ErrPos = 0;
    int cpos;
    BOOL bRedraw = bForceRedraw;

    int width = g_csbiInfo.dwMaximumWindowSize.X;
    width -= 3; //[]

    COORD pos;
    if (bRedraw == 1)
    {
        pos.X = 0;
        pos.Y = g_csbiInfo.dwCursorPosition.Y;
        SetConsoleCursorPosition(g_hStdout, pos);
        putchar(_T('['));
        pos.X = g_csbiInfo.dwMaximumWindowSize.X;
        pos.X -= 1;
        pos.Y = g_csbiInfo.dwCursorPosition.Y;
        SetConsoleCursorPosition(g_hStdout, pos);
        _putch(_T(']'));
        SetConsoleCursorPosition(g_hStdout, pos);
    }
    if (bRedraw == 2)
    {
        // reset the text color to normal white.
        SetConsoleTextAttribute(g_hStdout, 0x7);
        return;
    }

    cpos = (int)((float)curPos * width * 2 / MAX_POS);
    pos.X = (float)cpos / 2 + 1;
    pos.Y = g_csbiInfo.dwCursorPosition.Y;
    if (pos.X >= g_csbiInfo.dwMaximumWindowSize.X - 1)
    {
        return;
    }

    if ((PrePos != pos.X) || MAX_POS == curPos)
    {
        COORD prepos = pos;
        for (int done = PrePos; done < pos.X; done++)
        {
            prepos.X = done;
            SetConsoleCursorPosition(g_hStdout, prepos);
            SetConsoleTextAttribute(g_hStdout, 0x7);
            _putch(_T('#'));
        }
        if (MAX_POS == curPos)
        {
            prepos.X = g_csbiInfo.dwMaximumWindowSize.X - 2;
            SetConsoleCursorPosition(g_hStdout, prepos);
            SetConsoleTextAttribute(g_hStdout, 0x7);
            _putch(_T('#'));
            //printf("#]\n");
            return;
        }

        PrePos = pos.X;
    }

    SetConsoleCursorPosition(g_hStdout, pos);

    if (hWaitingThread && eState != I2C_ERASE_CHIPS)
    {
        bQuitWaiting = 1;
        WaitForSingleObject(hWaitingThread, INFINITE);
        hWaitingThread = NULL;

        pos.X = 0;
        pos.Y = g_csbiInfo.dwCursorPosition.Y;
        SetConsoleCursorPosition(g_hStdout, pos);
        SetConsoleTextAttribute(g_hStdout, 0x7);
        putchar(_T('['));
    }

    if (ErrPos != pos.X)
    {
        if (eState == I2C_OK)
        {
            SetConsoleTextAttribute(g_hStdout, FOREGROUND_GREEN);
            if (cpos & 1)
            {
                _putch(_T('W'));
            }
            else
            {
                _putch(_T('w'));
            }
        }
        else if (eState == I2C_ERASE_CHIPS)
        {
            unsigned int threadID;
            if (hWaitingThread == NULL)
            {
                bQuitWaiting = 0;
                hWaitingThread = (HANDLE)_beginthreadex(NULL, 0, &ShowWaitingCursor, &bQuitWaiting, 0, &threadID);
            }

            SetConsoleTextAttribute(g_hStdout, 6 | FOREGROUND_INTENSITY);
            _putch(_T('E'));
            _putch(_T('r'));
            _putch(_T('a'));
            _putch(_T('s'));
            _putch(_T('i'));
            _putch(_T('n'));
            _putch(_T('g'));
            _putch(_T(' '));
            _putch(_T('C'));
            _putch(_T('h'));
            _putch(_T('i'));
            _putch(_T('p'));
            _putch(_T('.'));
            _putch(_T('.'));
            _putch(_T('.'));
            _putch(_T(' '));
        }
        else if (eState == I2C_RETRY)
        {
            SetConsoleTextAttribute(g_hStdout, FOREGROUND_RED | FOREGROUND_GREEN);
            _putch(_T('R'));
            ::Sleep(700);
        }
        else
        {
            SetConsoleTextAttribute(g_hStdout, FOREGROUND_RED);
            _putch(_T('F'));
            SetConsoleTextAttribute(g_hStdout, 0x07);
            ErrPos = pos.X;
            ::Sleep(700);
        }
    }
    else
    {
    }
}

void InitShowProgress(const int MaxPos)
{
    printf("\n");
    g_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(g_hStdout, &g_csbiInfo);
    g_csbiInfo.dwMaximumWindowSize.X -= 1;

    ShowProgress(0, 1, I2C_OK, MaxPos);
}

void set_txt_color(int color)
{
    SetConsoleTextAttribute(g_hStdout, color & 0xff);
}
