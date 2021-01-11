#pragma once

#include <base/gh.h>
#include <base/menu/AnimatedComposedStiledVisual.h>
#include <graphics/visual2d/drawable/IDynamicPropertiesObject2D.h>
#include "ListEntry2D.h"
#include <base/interaction/CbTouchListener.h>
#include <base/audio/sound/DynamicSound.h>
#include <base/math/Bounds2D.h>

namespace base {
class List2D : public AnimatedComposedStiledVisual, virtual public IDynamicPropertiesObject2D {priv typedef AnimatedComposedStiledVisual super;pub dCtor(List2D);
	pub class IListener {
		pub virtual void onEntryTouched(List2D* list, ListEntry2D* listItem) = 0;

		pub virtual ~IListener() = default;
	};
	pub class CbListener : virtual public IListener {pub dCtor(CbListener);
		pub std::function<void(
			List2D* list, ListEntry2D* listItem)> cb_onEntryTouched;
	
		pub explicit CbListener();

		pub void onEntryTouched(List2D* list, ListEntry2D* listItem) override;

		pub ~CbListener() override;
	};

	pub class AlignmentH {
		pub static const int Left;
		pub static const int Center;
		pub static const int Right;
	};

	pub class AlignmentV {
		pub static const int Bottom;
		pub static const int Center;
		pub static const int Top;
	};

	priv static ArrayList<MenuItemConfig*>* itemConfigs;

	priv static ArrayList<StateChangeDurations*>* animationDurations;

	prot ArrayList<ListEntry2D*> entriesList{};
	prot int entriesCount = 0;

	prot float posX = 0.0f;
	prot float posY = 0.0f;
	prot float anchorX = 0.0f;
	prot float anchorY = 0.0f;

	// Value is computed.
	prot float listWidth = 0.0f;
	prot float listHeight = 0.0f;

	prot Bounds2D contentBounds{};
	
	prot Color bgTintColor{ "#FFFFFFFF" };
	prot float bgAlpha = 1.0f;

	prot int entriesAlignmentH = AlignmentH::Left;
	prot int entriesAlignmentV = AlignmentV::Bottom;
	
	prot float entriesSpacingX = 0.0f;
	prot float entriesSpacingY = 0.0f;

	prot float listPaddingTop = 0.0f;
	prot float listPaddingBottom = 0.0f;
	prot float listPaddingLeft = 0.0f;
	prot float listPaddingRight = 0.0f;

	prot float manualListSizeWidth = 0.0f;
	prot float manualListSizeHeight = 0.0f;
	
	// (>= 0.0f) Used.
	// (< 0.0f) Not used.
	prot float autoListSizeMaxWidth = -1.0f;
	prot float autoListSizeMaxHeight = -1.0f;

	// These values control if the list is horizontal or vertical.
	prot int rowsCount = 0;
	prot int columnsCount = 0;

	prot bool invertListGrowAxisX = false;
	prot bool invertListGrowAxisY = false;
	
	prot bool invertEntriesDrawOrder = false;

	prot bool autoListSizeToContent = true;

	prot Array1D<std::string> scalableSizeElements{ 2, new std::string[2] {
		"_local.bg",
		"_local.touchAreaBack"
	} };

	prot IContainer2D* layerEntries = nullptr;
	prot IInteractiveDrawable2D* touchAreaBack = nullptr;

	prot base::audio::DynamicSound* entryActivateDSound;
	prot float entryActivateSound_volume = 1.0f;

	priv CbTouchListener touchListener{};

	priv IListener* listener = nullptr;

    pub explicit List2D(IAppMetrics* appMetrics, IAppAssets* appAssets);
	prot void createContent() override;

	pub void setListener(IListener* listener);

	pub ArrayList<ListEntry2D*>* peekEntriesList();

	pub Bounds2D* getContentBounds();

	pub virtual ListEntry2D* createNewEntry(
		ArrayList<MenuItemConfig*>* useItemConfigs, ArrayList<StateChangeDurations*>* easeDurationsSList, int listPositionIndex, Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* extraDataMap,
		int entryUid = -1);
	pub virtual ListEntry2D* createNewEntry(
		ArrayList<MenuItemConfig*>* itemInstanceConfigOverride, ArrayList<StateChangeDurations*>* easeDurationsSList, Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* extraDataMap,
		int entryUid = -1);

	pub virtual void removeEntry(ListEntry2D* listEntry);

	pub virtual void removeAllEntries();

	pub ListEntry2D* getEtryWithIndex(int index);
	pub ListEntry2D* getEtryWithUid(int entryUid);

	pub void setBgTintColor(Color& bgTintColor);

	pub std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
	pub void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

	pub void visualLoop(float deltaS, int deltaMS) override;

	prot void listEntryTouched(ListEntry2D* listEntry);

	prot virtual void initializeListTouchAreaBackTouchListener();

	prot void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) override;
	prot void syncListVisuals();

	// Can be fully extendable.
	prot virtual void handleContentAlignmentX();
	prot virtual void handleContentAlignmentY();

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	prot virtual ListEntry2D* createNewEntry(
		ArrayList<MenuItemConfig*>* useItemConfigs, ArrayList<StateChangeDurations*>* easeDurationsSList,
		int entryUid = -1);

	prot void onListEntryCreated(ListEntry2D* listEntry);
	prot void onListEntryRemove_pre(ListEntry2D* listEntry);

	prot virtual void initializeNewEntryTouchHandling(IInteractiveDrawable2D* touchArea);

	priv bool onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	priv void updateEntriesListPos();
	priv static void computeEntryPosContinuousLogic(
		bool isFirstEntry, bool invertListGrowAxisA, bool invertListGrowAxisB, float listPaddingAMin, float listPaddingAMax, float listPaddingBMin, float listPaddingBMax, float entriesSpacingA, float entriesSpacingB, float entrySizeA, float entrySizeB, int secondaryAxisSegmentEntriesCount,
		int& gA, int& gB, float& prevSpacedA, float& prevSpacedB, float& currentSecondaryAxisSegmentContentSizeA, float& currentSecondaryAxisSegmentContentSizeB, float& maxSecondaryAxisSegmentContentSizeA, float& maxSecondaryAxisSegmentContentSizeB,
		float& oEntryPosA, float& oEntryPosB);

	priv void sanitizeEntry(ListEntry2D* entry);

	prot void disposeMain() override;
	pub ~List2D() override;
};
};
