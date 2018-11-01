﻿/***********************************************************************************
**
** SelectedObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/

#ifndef SELECTEDOBJECT_H
#define SELECTEDOBJECT_H

class CSelectedObject
{
public:
    uint32_t Serial = 0;
    CRenderObject *Object = nullptr;
    CGump *Gump = nullptr;
    CSelectedObject();
    virtual ~CSelectedObject();

    void Clear();
    void Clear(CRenderObject *obj);

    void Init(CRenderObject *obj, CGump *gump = nullptr);
    void Init(const CSelectedObject &obj);
};

extern CSelectedObject g_SelectedObject;
extern CSelectedObject g_LastSelectedObject;

#endif
