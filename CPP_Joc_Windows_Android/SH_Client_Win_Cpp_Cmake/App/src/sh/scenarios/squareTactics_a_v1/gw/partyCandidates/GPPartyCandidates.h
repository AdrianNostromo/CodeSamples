#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGPPartyCandidates.h"

namespace squareTactics_a_v1 {
class GPPartyCandidates final : public GamePart, virtual public IGPPartyCandidates {priv typedef GamePart super;pub dCtor(GPPartyCandidates);
	// This includes all valid entities (including active party members).
	priv ListDL<EntitiesCreateInfo> partyCandidatesConfigsList{};

	priv ListDL<sp<base::WrappedWorldEntity>> activePartyMemberEntitiesList{};

	pub explicit GPPartyCandidates(base::IGameWorld* gw);

	pub ListDL<EntitiesCreateInfo>& getPartyCandidatesConfigsListRef() final;
	pub ListDL<sp<base::WrappedWorldEntity>>& getActivePartyMemberEntitiesListRef() final;

	pub EntitiesCreateInfo* getEntityPartyCandidateConfigIfExists(IWorldEntity* entity) final;
	pub void addCandidateToActiveParty(EntitiesCreateInfo& entityConfig) final;
	pub void removeAllEntitiesFromActiveParty() final;
	pub void removeActivePartyConfigs() final;
	pub void disposeUnusedPartyCandidated() final;
	pub void disposeAllPartyCandidated() final;

	pub IWorldEntity* createPartyCandidateEntity(
		EntitiesCreateInfo& createConfig,
		Vector3& pos, Quaternion& rot) final;

    pub ~GPPartyCandidates() override;
};
};
