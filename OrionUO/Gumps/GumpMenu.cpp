﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpMenu.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/

#include "stdafx.h"

CGumpMenu::CGumpMenu(uint32_t serial, uint32_t id, short x, short y)
    : CGump(GT_MENU, serial, x, y)
{
    ID = id;
}

CGumpMenu::~CGumpMenu()
{
}

void CGumpMenu::CalculateGumpState()
{
    DEBUG_TRACE_FUNCTION;
    CGump::CalculateGumpState();

    if (g_GumpPressed)
        FrameCreated = false;
}

void CGumpMenu::PrepareContent()
{
    DEBUG_TRACE_FUNCTION;
    if (TextChanged)
    {
        TextChanged = false;

        if (m_TextObject != nullptr)
        {
            if (Text.length())
                m_TextObject->CreateTextureA(1, Text, 200, TS_LEFT, UOFONT_FIXED);
            else
                m_TextObject->m_Texture.Clear();

            WantRedraw = true;
        }
    }
}

bool CGumpMenu::OnLeftMouseButtonDoubleClick()
{
    DEBUG_TRACE_FUNCTION;
    if (g_PressedObject.LeftSerial && g_PressedObject.LeftSerial != ID_GM_HTMLGUMP)
    {
        SendMenuResponse(g_PressedObject.LeftSerial);

        return true;
    }

    return false;
}

void CGumpMenu::SendMenuResponse(int index)
{
    DEBUG_TRACE_FUNCTION;
    //Ответ на меню
    CPacketMenuResponse(this, index).Send();

    //Удаляем использованный гамп
    RemoveMark = true;
}

