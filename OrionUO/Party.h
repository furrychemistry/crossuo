﻿/***********************************************************************************
**
** Party.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/

#ifndef PARTY_H
#define PARTY_H

class CParty
{
public:
    //Серийник лидера пати
    uint32_t Leader = 0;
    //Серийник пригласившего в пати
    uint32_t Inviter = 0;
    //Может ли группа лутать труп игрока
    bool CanLoot = false;

    CParty();
    ~CParty();

    //Члены группы
    CPartyObject Member[10];

    //Обработка пакетов пати
    void ParsePacketData(Wisp::CDataReader &reader);

    //Содержит ли пати игрока с данным серийником
    bool Contains(int serial);

    //Очистить пати
    void Clear();
};

extern CParty g_Party;

#endif
