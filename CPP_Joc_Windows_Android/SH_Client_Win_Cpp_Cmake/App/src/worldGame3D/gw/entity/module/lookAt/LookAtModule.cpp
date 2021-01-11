#include <base/math/Math.h>
#include <base/log/ILogHandler.h>
#include "LookAtModule.h"

LookAtModule::LookAtModule(
    IWorldEntity* entity,
    LookAtTemplate* t
)
    : super(t->moduleName, entity),
      t(t)
{
    //void
}

Vector3& LookAtModule::peekLookAtDirection() {
    return lookAtDirection;
}

float LookAtModule::getAngDElevation() {
    return angDElevation;
}

float LookAtModule::getAngDZ() {
    return angDZ;
}

void LookAtModule::setLookAtAnglesDeg(float _angDZ, float _angDElevation) {
    float new_angDZ = Math::normalizeAngleDeg(_angDZ);
    float n = Math::max(-179.99f, Math::min(179.99f, _angDElevation));
    float new_angDElevation = Math::normalizeAngleDeg(n);

    if(Math::isEqual(this->angDZ, _angDZ) && Math::isEqual(this->angDElevation, _angDElevation)) {
        return ;
    }

    this->angDZ = new_angDZ;
    this->angDElevation = new_angDElevation;

    // Update the 3 quaternions.
    rotElevation.set(Vector3::Z, this->angDZ);
    rotZ.set(Vector3::Y, this->angDElevation);

    rotCombined.set(rotElevation)->mul(rotZ);

    // Update the look at direction vector.
    lookAtDirection.set(Vector3::X)->mul(rotCombined);
}

LookAtModule::~LookAtModule() {
    //void
}
