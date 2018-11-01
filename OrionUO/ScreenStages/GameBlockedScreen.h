﻿/***********************************************************************************
**
** GameBlockedScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/

#pragma once
#include "Input.h"

class CGameBlockedScreen : public CBaseScreen
{
public:
    //!Код блокировки
    uint8_t Code = 0;

    CGump m_GameBlockedScreenGump;

    CGameBlockedScreen();
    ~CGameBlockedScreen();

    /*!
	Инициализация
	@return
	*/
    virtual void Init();

    /*!
	Отрисовка/выбор объектов
	@param [__in] mode true - отрисовка, false - выбор
	@return При выборе объектов - идентификатор выбранного объекта
	*/
    virtual void Render(bool mode);

    virtual void OnLeftMouseButtonDown();
    virtual void OnLeftMouseButtonUp();

#if USE_WISP
    virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
#else
    virtual void OnTextInput(const SDL_TextInputEvent &ev) override;
#endif
    virtual void OnKeyDown(const KeyEvent &ev) override;
};

extern CGameBlockedScreen g_GameBlockedScreen;
