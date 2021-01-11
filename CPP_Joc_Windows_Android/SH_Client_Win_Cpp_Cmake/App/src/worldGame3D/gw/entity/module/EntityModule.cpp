#include <worldGame3D/gw/entity/module/EntityModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

EntityModule::EntityModule(IWorldEntity* entity)
    : super(),
    moduleId("")
{
    this->entity = this->e = entity;

    componentClassVoidP = this;
}

EntityModule::EntityModule(std::string& _moduleId, IWorldEntity* entity)
    : super(),
    moduleId(_moduleId)
{
    this->entity = this->e = entity;

    componentClassVoidP = this;
}

void EntityModule::reservedCreateB() {
    createBMain();
}

void EntityModule::createBMain() {
    //void
}

// This is made final so it is not used by mistake. Modules should use createBMain instead (probabbly).
void EntityModule::createPost() {
    super::createPost();

    // This is made final so it is not used by mistake. Modules should use createBMain instead (probabbly).

    //void
}

std::string EntityModule::getModuleId() {
    return moduleId;
}

IWorldEntity* EntityModule::getEntity() {
    return entity;
}

void EntityModule::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
    this->gwGamePartsOptional = gwGameParts;
}

void EntityModule::disconnectFromGamePartsPre() {
    this->gwGamePartsOptional = nullptr;

    disconnectedFromGameParts();
}

void EntityModule::disconnectedFromGameParts() {
    //void
}

void EntityModule::addAutoListener(std::shared_ptr<IEventListener> listener) {
    if(autoListenersList.containsDirect(listener)) {
        throw LogicException(LOC);
    }

    autoListenersList.appendReference(listener);
}

void EntityModule::reservedDisposePre() {
    disposePre();
}

void EntityModule::disposePre() {
	for(int i=autoListenersList.size()-1;i>=0;i--) {
        std::shared_ptr<IEventListener> listener = autoListenersList.removeAndGetDirect(i);

        if(!listener->isConnected()) {
            listener->disconnect();
        }
    }
}

EntityModule::~EntityModule() {
    //void
}
