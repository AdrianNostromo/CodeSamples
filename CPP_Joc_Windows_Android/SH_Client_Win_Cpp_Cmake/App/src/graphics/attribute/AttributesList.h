#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/object/Object.h>
#include <string>
#include "IAttributesList.h"

namespace graphics {
class AttributesList : public base::Object, public virtual IAttributesList {priv typedef base::Object super;pub dCtor(AttributesList);
    // asd_r_1;// Need to sort this list.
    prot ArrayList<Attribute*> list{};
    prot long bitMask = 0;

    pub explicit AttributesList();

    pub long getBitMask() final;

    pub ArrayList<Attribute*>* peekList() final;

    prot virtual void onEnableAtribute(Attribute* attribute);
    prot virtual void onDisableAtribute(Attribute* attribute);

    pub int count();

    priv static int BinarySearchAttribute(ArrayList<Attribute*>& list, int l, int r, long typeBitGroup);

    pub Attribute* getOptional(long typeBitGroup) final;

    pub void set(Attribute* attribute);
    pub void removeAndDispose(long typeBitMask);

    // This is a bad compare because MABLend can be false in one and true in the other and they compare to be equals. The renderer uses a geometry manager and this system is not suited for that (geometry combining).
    pub bool hasAll(const long typeBitMask);
    prot int indexOf(const long typeBitGroup);

    pub virtual AttributesList* clone(AttributesList* o=nullptr);

    prot void dispose() override;
    pub ~AttributesList() override;
};
};
