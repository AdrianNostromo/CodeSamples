#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>
#include <base/list/ArrayList.h>
#include <string>
#include <memory>
#include <base/list/ListDL.h>
#include <base/sp.h>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>
#include <base/list/Array3D.h>

class EntityTemplate;
class Visual3DModelMaterial;
class IWorldEntity;
namespace base {
	class WrappedWorldEntity;
};
namespace graphics {
	class TilingInformation;
};

namespace squareTactics_a_v1 {
class IGPPartyCandidates : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub class EntityCreateInfo_Base {pub dCtor(EntityCreateInfo_Base);
		//The pos is in room_space, not zone_space.
		pub EntityTemplate* entityTemplate;
		
		pub std::shared_ptr<std::string> instanceId;

		pub explicit EntityCreateInfo_Base(
			EntityTemplate* entityTemplate,
			std::shared_ptr<std::string> instanceId)
			: entityTemplate(entityTemplate),
			instanceId(instanceId)
		{
			//void
		}

		pub virtual ~EntityCreateInfo_Base() {};
	};
	pub class EntitiesCreateInfo : public EntityCreateInfo_Base {priv typedef EntityCreateInfo_Base super; pub dCtor(EntitiesCreateInfo);
		pub int teamIndex;

		pub Array3D<signed char>* voxelGridZXY;
		pub std::string extraDataId_voxelGridZXY;

		pub ArrayList<Visual3DModelMaterial*>* materialsList;
		pub std::string extraDataId_materialsList;

		// This is used to keep track of entities that created.
		pub sp<base::WrappedWorldEntity> wEntity = nullptr;
		pub bool isEntityInActiveParty = false;

		pub explicit EntitiesCreateInfo(
			EntityTemplate* entityTemplate,
			std::shared_ptr<std::string> instanceId, int teamIndex,
			Array3D<signed char>* voxelGridZXY, std::string extraDataId_voxelGridZXY,
			ArrayList<Visual3DModelMaterial*>* materialsList, std::string extraDataId_materialsList)
			: super(
				entityTemplate,
				instanceId),
			teamIndex(teamIndex),
			voxelGridZXY(voxelGridZXY), extraDataId_voxelGridZXY(extraDataId_voxelGridZXY),
			materialsList(materialsList), extraDataId_materialsList(extraDataId_materialsList)
		{
			//void
		}
	};

	pub virtual ListDL<EntitiesCreateInfo>& getPartyCandidatesConfigsListRef() = 0;
	pub virtual ListDL<sp<base::WrappedWorldEntity>>& getActivePartyMemberEntitiesListRef() = 0;
	
	pub virtual EntitiesCreateInfo* getEntityPartyCandidateConfigIfExists(IWorldEntity* entity) = 0;
	pub virtual void addCandidateToActiveParty(EntitiesCreateInfo& entityConfig) = 0;
	pub virtual void removeAllEntitiesFromActiveParty() = 0;
	pub virtual void removeActivePartyConfigs() = 0;
	pub virtual void disposeUnusedPartyCandidated() = 0;
	pub virtual void disposeAllPartyCandidated() = 0;

	pub virtual IWorldEntity* createPartyCandidateEntity(
		EntitiesCreateInfo& createConfig,
		Vector3& pos, Quaternion& rot) = 0;

    pub ~IGPPartyCandidates() override = default;
};
};
