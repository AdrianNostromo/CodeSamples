#pragma once

#include "GPMEntitiesCreator.h"

namespace base {
class GPMTouchable3DEntities : public GPMEntitiesCreator {
private: typedef GPMEntitiesCreator super;
private:
    ArrayList<IWorldEntity*> touchableEntities{};

public:
    GPMTouchable3DEntities(GPMTouchable3DEntities const&) = delete;
    GPMTouchable3DEntities(GPMTouchable3DEntities &&) = default;
    GPMTouchable3DEntities& operator=(GPMTouchable3DEntities const&) = delete;
    GPMTouchable3DEntities& operator=(GPMTouchable3DEntities &&) = default;

    explicit GPMTouchable3DEntities(base::IGameWorld* gw);

	std::shared_ptr<ArrayList<DT_Entity_Distance>> getEntitiesUnderTouch(Ray& ray, IEntitiesFilter* entityFilter) override;
	std::shared_ptr<ArrayList<DT_Entity_Distance>> getEntitiesUnderTouch(Ray& ray) override;

    ~GPMTouchable3DEntities() override;

protected:
	void createBMain() override;

private:
	void onEvent(IEventListener& eventListener, GWEvent& event);

	void onEntityAdded(IWorldEntity* entity);
	void onEntityRemoved_Pre(IWorldEntity* entity);

	static int FetchInsertIndexClosestFirst(std::shared_ptr<ArrayList<DT_Entity_Distance>>& list, float targetDist);

};
};
