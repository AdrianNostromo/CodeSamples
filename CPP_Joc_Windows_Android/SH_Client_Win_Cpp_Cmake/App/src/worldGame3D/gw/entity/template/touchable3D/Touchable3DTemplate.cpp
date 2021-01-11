#include "Touchable3DTemplate.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/module/touchable3D/Touchable3DModule.h>

Touchable3DTemplate::Touchable3DTemplate(
    const int shapeType,
    Vector3 mouseboxMin, Vector3 mouseboxMax)
    : super("touchable", ITouchable3DModule::TYPE),
      shapeType(shapeType), mouseboxMin(), mouseboxMax()
{
    this->mouseboxMin.set(mouseboxMin);
    this->mouseboxMax.set(mouseboxMax);
}

IEntityModule* Touchable3DTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    Touchable3DTemplate* t = dynamic_cast<Touchable3DTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    Touchable3DModule* mod = new Touchable3DModule(entity, t);

    return mod;
}

Touchable3DTemplate::~Touchable3DTemplate() {
    //void
}
