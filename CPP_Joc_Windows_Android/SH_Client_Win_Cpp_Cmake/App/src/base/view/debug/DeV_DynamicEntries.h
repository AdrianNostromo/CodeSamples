#pragma once

#include <base/gh.h>
#include "DeV_Base.h"
#include "entry/IHandler_DebugEntry.h"
#include <memory>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include <base/list/ListDL.h>
#include <base/list/ArrayList.h>
#include <base/interaction/CbTouchListener.h>
#include <base/MM.h>
#include "entry/DebugEntry.h"
#include <base/assets/util/ManagedAssetPath.h>

class IContainer2D;
class ISizedDrawable2D;
class IDynamicPropertiesObject2D;
namespace base {
	class BitmapLabel;
	class IDebugEntryUser;
	class SpriteButton2D;
};

namespace base {
class DeV_DynamicEntries : public DeV_Base, virtual public IHandler_DebugEntry {priv typedef DeV_Base super;pub dCtor(DeV_DynamicEntries);
	priv class EntriesPage {pub dCtor(EntriesPage);
		// Optional entries. Only pages that use sdynamic entries have a entry in this list.
		pub IContainer2D* vContainer;

		pub ListDL<DebugEntry> entriesList{LOC};
		pub bool isPositionsInvalid = false;
		pub ListDL<DebugEntry*> invalidEntriesList{ LOC };

		pub explicit EntriesPage(IContainer2D* vContainer)
			: vContainer(vContainer)
		{}
	};
	
	priv static ManagedAssetPath* FontPreloadingPreparer_0;

	prot static MenuItemConfig* new_viewItemConfig_bodyDynamics();

	priv CbTouchListener touchListener{};

	priv BitmapLabel* lblClearMinMax = nullptr;

	priv base::SpriteButton2D* btnClearMinMaxCumulative = nullptr;

	priv int defaultPageIndex = -1;
	priv int activePageIndex = -1;
	// Note. The pointers are not stable in AList, use dynamic entries.
	pub ArrayList<EntriesPage*> pagesList{};

	priv IContainer2D* mainLayer = nullptr;
	priv IDynamicPropertiesObject2D* props = nullptr;
	priv float entriesPosX = 0;
	priv float entryRightMarginPosX = 0;
	priv float entriesListPaddingY = 0;
	priv float entrySizeY = 0;
	priv Vector2 btnSize{};
	priv ISizedDrawable2D* darkBg = nullptr;

    pub explicit DeV_DynamicEntries(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);
	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	prot int registerNewPage(bool isDefaultPage, bool usesDynamicEntries);
	prot int getActivePageIndex();
	prot void gotoNextPage();
	prot virtual void onActivePageChanged();

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	prot void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) override;

	pub void visualLoop(float deltaS, int deltaMS) override;
	pub void visualLoopB(float deltaS, int deltaMS) override;

	// pageIndex: Use -1 for the default page.
	pub std::shared_ptr<base::IDebugEntryUser> addDebugEntry(int linesCount = 1, bool hasToggleSwitch = false, int pageIndex=-1, bool hasBtn=false) final;
	pub void removeDebugEntry(DebugEntry* debugEntry) final;

	pub void onDebugEntryInvalidation(DebugEntry* entry) final;
	pub void onDebugEntryLinesCountChanged(DebugEntry* entry) final;

	prot virtual void clearMinMaxCumulative();

	pub std::string computeStateFlags(std::string baseState) override;

	prot static void CheckedSetDebugEntry(std::shared_ptr<base::WrappedValue<int>> ddeValue, int newVal, bool& isChange);

	prot void dispose() override;
    pub ~DeV_DynamicEntries() override;
};
};
