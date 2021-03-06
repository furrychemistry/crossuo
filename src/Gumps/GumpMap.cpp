// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2016 Hotride

#include <cmath> // atan, sin, cos
#include <SDL_rect.h>
#include "GumpMap.h"
#include "../CrossUO.h"
#include "../SelectedObject.h"
#include "../PressedObject.h"
#include "../Managers/MouseManager.h"
#include "../Network/Packets.h"
#include "../Renderer/RenderAPI.h"

#define XUO_M_PI 3.14159265358979323846264338327950288

enum
{
    ID_GM_PLOT_COURSE = 1,
    ID_GM_STOP_PLOTTING = 2,
    ID_GM_CLEAR_COURSE = 3,

    ID_GM_PIN_LIST = 10,
    ID_GM_PIN_LIST_INSERT = 1000,
};

CGumpMap::CGumpMap(
    uint32_t serial,
    uint16_t graphic,
    int startX,
    int startY,
    int endX,
    int endY,
    int width,
    int height)
    : CGump(GT_MAP, serial, 0, 0)
    , StartX(startX)
    , StartY(startY)
    , EndX(endX)
    , EndY(endY)
    , Width(width)
    , Height(height)
{
    Graphic = graphic;

    Add(new CGUIResizepic(0, 0x1432, 0, 0, Width + 44, Height + 61)); //Map Gump

    m_PlotCourse = (CGUIButton *)Add(new CGUIButton(
        ID_GM_PLOT_COURSE, 0x1398, 0x1398, 0x1398, (Width - 100) / 2, 5)); //Plot Course
    m_StopPlotting = (CGUIButton *)Add(new CGUIButton(
        ID_GM_STOP_PLOTTING, 0x1399, 0x1399, 0x1399, (Width - 70) / 2, 5)); //Stop Plotting
    m_ClearCourse = (CGUIButton *)Add(new CGUIButton(
        ID_GM_CLEAR_COURSE, 0x139A, 0x139A, 0x139A, (Width - 66) / 2, Height + 37)); //Clear Course

    m_PlotCourse->Visible = (m_PlotState == 0);
    m_StopPlotting->Visible = (m_PlotState == 1);
    m_ClearCourse->Visible = (m_PlotState == 1);

    m_Texture = (CGUIExternalTexture *)Add(new CGUIExternalTexture(new CSprite(), true, 24, 31));

    Add(new CGUIGumppic(0x139D, Width - 20, Height - 20)); //N

    m_DataBox = (CGUIDataBox *)Add(new CGUIDataBox());
    m_DataBox->Visible = false;
}

CGumpMap::~CGumpMap()
{
    for (CGUIText *text : m_Labels)
    {
        delete text;
    }

    m_Labels.clear();
}

void CGumpMap::SetPlotState(int val)
{
    m_PlotState = val;
    m_PlotCourse->Visible = (val == 0);
    m_StopPlotting->Visible = (val == 1);
    m_ClearCourse->Visible = (val == 1);

    WantRedraw = true;
}

int CGumpMap::LineUnderMouse(int &x1, int &y1, int x2, int y2)
{
    int tempX = x2 - x1;
    int tempY = y2 - y1;

    float testOfsX = (float)tempX;

    if (testOfsX == 0.0f)
    {
        testOfsX = 1.0f;
    }

    float pi = (float)XUO_M_PI;

    float a = -(atan(tempY / testOfsX) * 180.0f / pi);

    bool inverseCheck = false;

    if (x1 >= x2 && y1 <= y2)
    {
        inverseCheck = true;
    }
    else if (x1 >= x2 && y1 >= y2)
    {
        inverseCheck = true;
    }

    float sinA = sin(a * pi / 180.f);
    float cosA = cos(a * pi / 180.f);

    int offsX = (int)((tempX * cosA) - (tempY * sinA));
    int offsY = (int)((tempX * sinA) + (tempY * cosA));

    int endX2 = x1 + offsX;
    int endY2 = y1 + offsY;

    tempX = g_MouseManager.Position.X - x1;
    tempY = g_MouseManager.Position.Y - y1;

    offsX = (int)((tempX * cosA) - (tempY * sinA));
    offsY = (int)((tempX * sinA) + (tempY * cosA));

    SDL_Point mousePoint = { x1 + offsX, y1 + offsY };

    const int polyOffset = 5;

    int result = 0;

    if (!inverseCheck)
    {
        SDL_Rect lineRect = {
            x1 - polyOffset, y1 - polyOffset, endX2 + polyOffset, endY2 + polyOffset
        };

        if (SDL_PointInRect(&mousePoint, &lineRect) != 0u)
        {
            x1 = x1 + ((x2 - x1) / 2);
            y1 = y1 + ((y2 - y1) / 2);

            result = 1;
        }
    }
    else
    {
        SDL_Rect lineRect = {
            endX2 - polyOffset, endY2 - polyOffset, x1 + polyOffset, y1 + polyOffset
        };

        if (SDL_PointInRect(&mousePoint, &lineRect) != 0u)
        {
            x1 = x2 + ((x1 - x2) / 2);
            y1 = y2 + ((y1 - y2) / 2);

            result = 2;
        }
    }

    return result;
}

void CGumpMap::PrepareContent()
{
    if (m_DataBox != nullptr)
    {
        int serial = 1;

        QFOR(item, m_DataBox->m_Items, CBaseGUI *)
        {
            item->Serial = serial;

            serial++;
        }

        //Если окошко захвачено для перемещения - вычислим оффсеты
        if (g_PressedObject.LeftGump == this && g_PressedObject.LeftObject != nullptr &&
            (m_PlotState != 0))
        {
            if (m_PinOnCursor == nullptr)
            {
                CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

                if (((offset.X != 0) || (offset.Y != 0)) &&
                    g_PressedObject.LeftSerial > ID_GM_PIN_LIST &&
                    g_PressedObject.LeftSerial < ID_GM_PIN_LIST_INSERT && m_PinTimer > g_Ticks)
                {
                    m_PinOnCursor = (CBaseGUI *)g_PressedObject.LeftObject;
                }
            }

            if (m_PinOnCursor != nullptr)
            {
                int newX = g_MouseManager.Position.X - (m_X + 20);
                int newY = g_MouseManager.Position.Y - (m_Y + 30);

                WantRedraw = (m_PinOnCursor->GetX() != newX || m_PinOnCursor->GetY() != newY);

                m_PinOnCursor->SetX(newX);
                m_PinOnCursor->SetY(newY);
            }
        }

        NoMove = (m_PinOnCursor != nullptr);
    }
}

void CGumpMap::GenerateFrame(bool stop)
{
    //m_Labels

    if (m_DataBox != nullptr)
    {
        int idx = 0;

        QFOR(item, m_DataBox->m_Items, CBaseGUI *)
        {
            int drawX = item->GetX() + 18;
            int drawY = item->GetY() + 21;

            if (item != m_PinOnCursor)
            {
                CGUIText *text = nullptr;

                if (idx >= (int)m_Labels.size())
                {
                    text = new CGUIText(0, drawX - 10, drawY - 12);
                    m_Labels.push_back(text);
                }
                else
                {
                    text = m_Labels[idx];
                }

                idx++;
                text->CreateTextureA(0, str_from(idx));
            }
        }
    }

    CGump::GenerateFrame(false);

    if (m_DataBox != nullptr)
    {
        int idx = 1;

        QFOR(item, m_DataBox->m_Items, CBaseGUI *)
        {
            int drawX = item->GetX() + 18;
            int drawY = item->GetY() + 21;

            CBaseGUI *next = (CBaseGUI *)item->m_Next;

            if (next != nullptr)
            {
                int nextDrawX = next->GetX() + 20;
                int nextDrawY = next->GetY() + 30;

#ifndef NEW_RENDERER_ENABLED
                if (next == m_PinOnCursor || item == m_PinOnCursor)
                {
                    glColor4f(0.87f, 0.87f, 0.87f, 1.0f);
                }
                else
                {
                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                }

                g_GL.DrawLine(drawX + 2, drawY + 8, nextDrawX, nextDrawY);

                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
#else
                static float4 s_colorGray87 = { .87f, .87f, .87f, 1.f };
                const auto useGray = next == m_PinOnCursor || item == m_PinOnCursor;
                RenderAdd_DrawLine(
                    g_renderCmdList,
                    DrawLineCmd{ drawX + 2,
                                 drawY + 8,
                                 nextDrawX,
                                 nextDrawY,
                                 useGray ? s_colorGray87 : g_ColorWhite });
#endif

                if (m_PinOnCursor == nullptr && g_SelectedObject.Serial >= ID_GM_PIN_LIST_INSERT &&
                    (g_SelectedObject.Serial - ID_GM_PIN_LIST_INSERT) == idx)
                {
                    int checkX = drawX + 2;
                    int checkY = drawY + 8;

                    if (LineUnderMouse(checkX, checkY, nextDrawX, nextDrawY) != 0)
                    {
                        g_Game.DrawGump(0x139B, 0, checkX - 2, checkY - 8);
                    }
                }
            }

            if (item != m_PinOnCursor)
            {
                g_Game.DrawGump(0x139B, 0, drawX, drawY);

                if (idx - 1 < (int)m_Labels.size())
                {
                    CGUIText *text = m_Labels[idx - 1];
                    text->Draw();
                }

                idx++;
            }
        }
    }
}

CRenderObject *CGumpMap::Select()
{
    CRenderObject *selected = CGump::Select();

    if (m_DataBox != nullptr)
    {
        CPoint2Di oldPos = g_MouseManager.Position;
        g_MouseManager.Position =
            CPoint2Di(oldPos.X - (int)g_GumpTranslate.X, oldPos.Y - (int)g_GumpTranslate.Y);

        QFOR(item, m_DataBox->m_Items, CBaseGUI *)
        {
            int drawX = item->GetX() + 18;
            int drawY = item->GetY() + 21;

            CBaseGUI *next = (CBaseGUI *)item->m_Next;

            if (next != nullptr)
            {
                int nextDrawX = next->GetX() + 20;
                int nextDrawY = next->GetY() + 30;

                int checkX = drawX + 2;
                int checkY = drawY + 8;

                if (LineUnderMouse(checkX, checkY, nextDrawX, nextDrawY) != 0)
                {
                    g_SelectedObject.Init(item, this);
                    g_SelectedObject.Serial = item->Serial + ID_GM_PIN_LIST_INSERT;
                }
            }

            if (g_Game.PolygonePixelsInXY(drawX, drawY, 10, 10))
            {
                g_SelectedObject.Init(item, this);
                g_SelectedObject.Serial = item->Serial + ID_GM_PIN_LIST;
            }
        }

        g_MouseManager.Position = oldPos;
    }

    return selected;
}

void CGumpMap::GUMP_BUTTON_EVENT_C
{
    if (serial == ID_GM_PLOT_COURSE || serial == ID_GM_STOP_PLOTTING) //Plot Course /Stop Plotting
    {
        CPacketMapMessage(Serial, MM_EDIT, m_PlotState).Send();
        SetPlotState(static_cast<int>(static_cast<int>(m_PlotState) == 0));

        WantRedraw = true;
    }
    else if (serial == ID_GM_CLEAR_COURSE) //Clear Course
    {
        CPacketMapMessage(Serial, MM_CLEAR).Send();

        m_DataBox->Clear();

        WantRedraw = true;
    }
}

void CGumpMap::OnLeftMouseButtonDown()
{
    CGump::OnLeftMouseButtonDown();

    m_PinTimer = g_Ticks + 300;
}

void CGumpMap::OnLeftMouseButtonUp()
{
    CGump::OnLeftMouseButtonUp();

    if (m_DataBox != nullptr && g_PressedObject.LeftObject != nullptr)
    {
        if ((m_PlotState != 0) && m_PinOnCursor == nullptr && m_PinTimer > g_Ticks)
        {
            if (g_PressedObject.LeftSerial >= ID_GM_PIN_LIST_INSERT)
            {
                int idx = g_PressedObject.LeftSerial - ID_GM_PIN_LIST_INSERT - 1;

                CBaseGUI *first = (CBaseGUI *)m_DataBox->Get(idx);

                if (first != nullptr)
                {
                    int x = m_X;
                    int y = m_Y;

                    int drawX = x + first->GetX() + 18;
                    int drawY = y + first->GetY() + 21;

                    CBaseGUI *next = (CBaseGUI *)first->m_Next;

                    if (next != nullptr)
                    {
                        int nextDrawX = x + next->GetX() + 20;
                        int nextDrawY = y + next->GetY() + 30;

                        int checkX = drawX + 2;
                        int checkY = drawY + 8;

                        if (LineUnderMouse(checkX, checkY, nextDrawX, nextDrawY) != 0)
                        {
                            checkX = checkX - (x + 20);
                            checkY = checkY - (y + 29);

                            CPacketMapMessage(Serial, MM_INSERT, idx + 1, checkX, checkY).Send();

                            m_DataBox->Insert(first, new CGUIGumppic(0x139B, checkX, checkY));
                            WantRedraw = true;
                        }
                    }
                }
            }
            else
            {
                int x = m_X + 24;
                int y = m_Y + 32;

                if (g_Game.PolygonePixelsInXY(x, y, Width, Height))
                {
                    x = g_MouseManager.Position.X - x - 4;
                    y = g_MouseManager.Position.Y - y - 2;

                    CPacketMapMessage(Serial, MM_ADD, 0, x, y).Send();

                    m_DataBox->Add(new CGUIGumppic(0x139B, x, y));
                    WantRedraw = true;
                }
            }
        }
    }

    if (m_PinOnCursor != nullptr && m_DataBox != nullptr)
    {
        int x = m_X + 24;
        int y = m_Y + 32;

        int idx = g_PressedObject.LeftSerial - ID_GM_PIN_LIST - 1;

        if (g_Game.PolygonePixelsInXY(x, y, Width, Height))
        {
            x = g_MouseManager.Position.X - (x - 4);
            y = g_MouseManager.Position.Y - (y - 2);

            m_PinOnCursor->SetX(x);
            m_PinOnCursor->SetY(y);

            CPacketMapMessage(Serial, MM_MOVE, idx, x, y).Send();
        }
        else
        {
            CPacketMapMessage(Serial, MM_REMOVE, idx).Send();

            m_DataBox->Delete(m_PinOnCursor);
        }

        WantRedraw = true;
        m_PinOnCursor = nullptr;
    }
}
