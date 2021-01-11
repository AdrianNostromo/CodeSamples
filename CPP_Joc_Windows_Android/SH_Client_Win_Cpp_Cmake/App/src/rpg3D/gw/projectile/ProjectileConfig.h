#pragma once

#include <string>
#include <base/object/Object.h>
#include "EffectPayload.h"
#include <base/list/ArrayList.h>
#include <memory>
#include <base/map/Map2D.h>

namespace rpg3D {
class ProjectileConfig : public base::Object {
private: typedef base::Object super;
public:
	static Map2D<std::string, std::string, ProjectileConfig*> entriesMap;

public:
	std::string nSpace;
	std::string id;

	std::string projectileTemplateNSpace;
	std::string projectileTemplateId;

	ArrayList<std::shared_ptr<rpg3D::EffectPayload>> effectPayloadsList{};

public:
	ProjectileConfig(ProjectileConfig const&) = delete;
	ProjectileConfig(ProjectileConfig &&) = default;
	ProjectileConfig& operator=(ProjectileConfig const&) = delete;
	ProjectileConfig& operator=(ProjectileConfig &&) = default;

	static ProjectileConfig* newEntry(std::string nSpace, std::string& id);
	static ProjectileConfig* fromId(std::string nSpace, std::string id);

	ProjectileConfig* init(std::string& projectileTemplateNSpace, std::string& projectileTemplateId);

	~ProjectileConfig() override;
private:
	explicit ProjectileConfig(std::string& nSpace, std::string& id);

};
};
