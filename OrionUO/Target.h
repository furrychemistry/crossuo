﻿/***********************************************************************************
**
** Target.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/

#ifndef TARGET_H
#define TARGET_H

//Класс для работы с таргетом
class CTarget
{
public:
    //Тип объекта прицела
    uint8_t Type = 0;
    //Тип прицела
    uint8_t CursorType = 0;
    //Мульти на таргете
    uint16_t MultiGraphic = 0;
    //Мульти на таргете X
    uint16_t MultiX = 0;
    //Мульти на таргете Y
    uint16_t MultiY = 0;
    //Серийник объекта, к которому привязан прицел
    uint32_t CursorID = 0;
    //Флаг состояния прицела
    bool Targeting = false;

private:
    //Текущие и последний данные прицела
    uint8_t m_Data[19];
    uint8_t m_LastData[19];

    //Список объектов мульти на таргете
    CMulti *m_Multi{ nullptr };

    //Добавить мульти-объект
    void AddMultiObject(CMultiObject *obj);

public:
    CTarget();
    ~CTarget() {}

    //Послать таргет на объект
    void SetLastTargetObject(int serial);

    //Установить данные прицела
    void SetData(Wisp::CDataReader &reader);

    //Установить данные мульти-таргета
    void SetMultiData(Wisp::CDataReader &reader);

    bool IsTargeting() const { return Targeting; }

    //Очистить таргет
    void Reset();

    void RequestFromCustomHouse();

    //Послать таргет на объект
    void SendTargetObject(int Serial);

    //Послать таргет на тайл
    void SendTargetTile(uint16_t tileID, short x, short Y, char z);

    //Послать отмену таргета
    void SendCancelTarget();

    //Послать таргет на объект
    void Plugin_SendTargetObject(int Serial);

    //Послать таргет на тайл
    void Plugin_SendTargetTile(uint16_t tileID, short x, short Y, char z);

    //Послать отмену таргета
    void Plugin_SendCancelTarget();

    //Послать таргет
    void Plugin_SendTarget();

    //Послать таргет на последнюю цель
    void SendLastTarget();

    //Послать таргет
    void SendTarget();

    //Загрузить мульти-объект
    void LoadMulti(int offsetX, int offsetY, char offsetZ);

    //Выгрузить мульти-объект
    void UnloadMulti();

    //Получить объект мульти в координатах
    CMulti *GetMultiAtXY(short x, short y);
};

extern CTarget g_Target;

#endif
