#pragma once

#include <memory>
#include <base/list/ArrayList.h>

class IWorldEntity;
namespace rpg3D {
	class StatusEffect;
}

namespace rpg3D {
class CfgStatusEffect {
public:
	class SubStatusEffect {
	public:
		std::shared_ptr<CfgStatusEffect> subStatusEffectConfig;

		bool isDependantTracked;

	public:
		SubStatusEffect(SubStatusEffect const&) = delete;
		SubStatusEffect(SubStatusEffect &&) = default;
		SubStatusEffect& operator=(SubStatusEffect const&) = delete;
		SubStatusEffect& operator=(SubStatusEffect &&) = default;

		explicit SubStatusEffect(std::shared_ptr<CfgStatusEffect>& subStatusEffectConfig, bool isDependantTracked)
			: subStatusEffectConfig(subStatusEffectConfig), isDependantTracked(isDependantTracked)
			{}

		virtual ~SubStatusEffect() {};
	};

public:
	int type;

	float durationS;

	ArrayList<SubStatusEffect> subStatusEffectsList{};

protected:
	static int TYPE_COUNTER;
	static int NewTypeIndex();

public:
    CfgStatusEffect(CfgStatusEffect const&) = delete;
    CfgStatusEffect(CfgStatusEffect &&) = default;
    CfgStatusEffect& operator=(CfgStatusEffect const&) = delete;
    CfgStatusEffect& operator=(CfgStatusEffect &&) = default;

    explicit CfgStatusEffect(int type, float durationS);

    void addSubEffect(std::shared_ptr<CfgStatusEffect> subStatusEffectConfig, bool isDependantTracked);

    // Note. Use the param to have a reference to the shared pointer version.
	virtual StatusEffect* buildSP(std::shared_ptr<CfgStatusEffect> statusEffectConfig, IWorldEntity* entity) = 0;

	virtual ~CfgStatusEffect();
};
};
