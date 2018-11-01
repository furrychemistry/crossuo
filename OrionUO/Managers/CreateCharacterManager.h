﻿/***********************************************************************************
**
** CreateCharacterManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/

#ifndef CREATECHARACTERMANAGER_H
#define CREATECHARACTERMANAGER_H

//!Информация о волосеном покрове
struct CC_HAIR_STYLE
{
    //!Индекс гампа
    uint16_t GumpID;

    //!Индекс картинки из Art.mul
    uint16_t GraphicID;

    //!Название
    string Name;
};

//!Класс менеджера создания персонажей
class CCreateCharacterManager
{
protected:
    bool m_Female = false;

public:
    bool GetFemale() { return m_Female; };
    void SetFemale(bool val);

protected:
    RACE_TYPE m_Race = RT_HUMAN;

public:
    RACE_TYPE GetRace() { return m_Race; };
    void SetRace(RACE_TYPE val);
    uint8_t HairStyle = 1;
    uint8_t BeardStyle = 0;
    uint16_t SkinTone = 0x03EA;
    uint16_t ShirtColor = 0x0084;
    uint16_t PantsColor = 0x035F;
    uint16_t HairColor = 0x044E;
    uint16_t BeardColor = 0x044E;

public:
    static const int HUMAN_MALE_HAIR_COUNT = 10;
    static const int HUMAN_FEMALE_HAIR_COUNT = 11;
    static const int HUMAN_MALE_FACIAL_HAIR_COUNT = 8;

    static const int ELF_MALE_HAIR_COUNT = 9;
    static const int ELF_FEMALE_HAIR_COUNT = 9;

    static const int GARGOYLE_MALE_HAIR_COUNT = 9;
    static const int GARGOYLE_FEMALE_HAIR_COUNT = 9;
    static const int GARGOYLE_MALE_FACIAL_HAIR_COUNT = 5;

    static const int HUMAN_SKIN_TONE_COUNT = 64;
    static const int ELF_SKIN_TONE_COUNT = 32;
    static const int GARGOYLE_SKIN_TONE_COUNT = 28;

    static const int HUMAN_HAIR_COLOR_COUNT = 48;
    static const int ELF_HAIR_COLOR_COUNT = 54;
    static const int GARGOYLE_HAIR_COLOR_COUNT = 18;

private:
    //!Перечень стилей причесок и бороды
    static const CC_HAIR_STYLE m_HumanMaleHairStyleTable[HUMAN_MALE_HAIR_COUNT];
    static const CC_HAIR_STYLE m_HumanFemaleHairStyleTable[HUMAN_FEMALE_HAIR_COUNT];
    static const CC_HAIR_STYLE m_HumanBeardStyleTable[HUMAN_MALE_FACIAL_HAIR_COUNT];

    static const CC_HAIR_STYLE m_ElfMaleHairStyleTable[ELF_MALE_HAIR_COUNT];
    static const CC_HAIR_STYLE m_ElfFemaleHairStyleTable[ELF_FEMALE_HAIR_COUNT];

    static const CC_HAIR_STYLE m_GargoyleMaleHairStyleTable[GARGOYLE_MALE_HAIR_COUNT];
    static const CC_HAIR_STYLE m_GargoyleFemaleHairStyleTable[GARGOYLE_FEMALE_HAIR_COUNT];
    static const CC_HAIR_STYLE m_GargoyleBeardStyleTable[GARGOYLE_MALE_FACIAL_HAIR_COUNT];

    static const uint16_t m_HumanSkinTone[HUMAN_SKIN_TONE_COUNT];
    static const uint16_t m_ElfSkinTone[ELF_SKIN_TONE_COUNT];
    static const uint16_t m_GargoyleSkinTone[GARGOYLE_SKIN_TONE_COUNT];

    static const uint16_t m_HumanHairColor[HUMAN_HAIR_COLOR_COUNT];
    static const uint16_t m_ElfHairColor[ELF_HAIR_COLOR_COUNT];
    static const uint16_t m_GargoyleHairColor[GARGOYLE_HAIR_COLOR_COUNT];

public:
    CCreateCharacterManager();
    ~CCreateCharacterManager();

    /*!
	Инициализация
	@return
	*/
    void Init();

    /*!
	Очистка
	@return
	*/
    void Clear();

    int GetCurrentHairCount();

    int GetCurrentFacialHairCount();

    uint16_t GetBodyGump();

    uint16_t GetShirtGump();

    uint16_t GetPantsGump();

    uint16_t GetBootsGump();

    const uint16_t *GetSkinTonePtr();

    const uint16_t *GetHairColorPtr();

    /*!
	Получить стиль волос
	@param [__in] pos Индекс волос
	@return Структура с данными о волосах
	*/
    CC_HAIR_STYLE GetHair(uint8_t pos) const;

    /*!
	Получить стиль бороды
	@param [__in] pos Индекс бороды
	@return Структура с данными о бороде
	*/
    CC_HAIR_STYLE GetBeard(uint8_t pos) const;
};

//!Менеджер создания мерсонажа
extern CCreateCharacterManager g_CreateCharacterManager;

#endif
