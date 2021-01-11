#pragma once

#include <base/visual3d/util/Drawable3DCreateConfigBase.h>
#include <base/list/ArrayList.h>

class Visual3DCreateConfig {
public:
    ArrayList<ArrayList<Drawable3DCreateConfigBase*>*>* drawableCreateConfigs;

public:
    Visual3DCreateConfig(ArrayList<ArrayList<Drawable3DCreateConfigBase*>*>* drawableCreateConfigs);
};
