#include "List2D.h"
#include <base/statics/StaticsInit.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/util/AppStyles.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/math/Math.h>
#include <base/app/IAppAssets.h>

using namespace base;

const int List2D::AlignmentH::Left = 1;
const int List2D::AlignmentH::Center = 2;
const int List2D::AlignmentH::Right = 3;

const int List2D::AlignmentV::Bottom = 1;
const int List2D::AlignmentV::Center = 2;
const int List2D::AlignmentV::Top = 3;

List2D::CbListener::CbListener() {
	//void
}

void List2D::CbListener::onEntryTouched(List2D* list, ListEntry2D* listItem) {
	if (cb_onEntryTouched) {
		cb_onEntryTouched(list, listItem);
	}
}

List2D::CbListener::~CbListener() {
	//void
}

ArrayList<MenuItemConfig*>* List2D::itemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	itemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", 0.0f)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_in_", 1.0f, EaseConfig::LINEAR)
				->putState("_out_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			// The bg rendering3DSize is scaled to content in this example.
			//->put("width", 1f, "_local.size", "width")
			//->put("height", 154f, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)

			->put("tintColor", nullptr, "_root.bgTintColor")
			->put("alpha", 1.0f, "_root.bgAlpha")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("touchAreaBack", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			// The bg rendering3DSize is scaled to content in this example.
			//->put("width", 1f, "_local.size.width")
			//->put("height", 1f, "_local.size", "height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("layerEntries", "layer", VCC_Group::New(), (new VisualPropertiesConfig())
			//void
		));
});

ArrayList<StateChangeDurations*>* List2D::animationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	animationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
});

List2D::List2D(IAppMetrics* appMetrics, IAppAssets* appAssets)
	: super(appMetrics, appAssets, itemConfigs, animationDurations)
{
	touchListener.cb_onCursorDown = std::bind(&List2D::onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool List2D::onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (buttonId != base::Touch::ButtonCode::LeftOrTouch) {
		// Only left btn or touch are handled.
		return false;
	}

	// Touch events are ignored while the menu is animating.
	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(touchAreaBack)) {
		//void
	}else if (target->getUserDataMap()->getDirectB("listEntry", nullptr) != nullptr) {
		if (listener != nullptr) {
			ListEntry2D* entry = target->getUserDataMap()->getDirectB("listEntry")->getDirectAs_Custom<ListEntry2D*>(-1);
			if (entry == nullptr) {
				throw LogicException(LOC);
			}

			listEntryTouched(entry);
		}

		entryActivateDSound->play(entryActivateSound_volume);
	} else {
		GlobalAccessLogHandler::devLogHandler->post_debug("Error", "Touch event received for a unknown target.");
	}

	return false;
}

void List2D::listEntryTouched(ListEntry2D* listEntry) {
	listener->onEntryTouched(this, listEntry);
}

void List2D::setListener(IListener* listener) {
	this->listener = listener;
}

ArrayList<ListEntry2D*>* List2D::peekEntriesList() {
	return &entriesList;
}

void List2D::createContent() {
	super::createContent();

	entryActivateDSound = new base::audio::DynamicSound(appAssets->getEmbeddedAssetsManager());

	// Because these value are used by templates they have to be initialized.
	putStaticAnimatingPropertyStatusDirect("listWidth", true);
	putStaticAnimatingPropertyStatusDirect("listHeight", true);
	putStaticAnimatingPropertyStatusDirect("bgTintColor", true);
	putStaticAnimatingPropertyStatusDirect("bgAlpha", true);
}

void List2D::initializeListTouchAreaBackTouchListener() {
	touchAreaBack->addTouchListener(&touchListener);
}

Bounds2D* List2D::getContentBounds() {
	return &contentBounds;
}

void List2D::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
	super::syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

	syncListVisuals();
}

void List2D::syncListVisuals() {
	updateEntriesListPos();

	// Scale rendering3DSize to content or or use height override.
	if (autoListSizeToContent) {
		if (autoListSizeMaxWidth >= 0.0f) {
			listWidth = Math::min(contentBounds.getWidth(), autoListSizeMaxWidth);
		} else {
			listWidth = contentBounds.getWidth();
		}

		if (autoListSizeMaxHeight >= 0.0f) {
			listHeight = Math::min(contentBounds.getHeight(), autoListSizeMaxHeight);
		} else {
			listHeight = contentBounds.getHeight();
		}
	} else {
		listHeight = manualListSizeHeight;
		listWidth = manualListSizeWidth;
	}
	
	handleContentAlignmentX();
	handleContentAlignmentY();

	for (int i = 0; i < scalableSizeElements.getCount(); i++) {
		std::string& itemID = scalableSizeElements.getReference(i);

		ISizedDrawable2D* item = getItemOptionalAs< ISizedDrawable2D * >(itemID);
		if (item == nullptr) {
			throw LogicException(LOC);
		}

		item->setSize(listWidth, listHeight);
	}

	visualsHolder->setPosition(
		posX - listWidth * anchorX,
		posY - listHeight * anchorY
	);
}

void List2D::handleContentAlignmentX() {
	if (entriesAlignmentH == AlignmentH::Left) {
		layerEntries->setX(-contentBounds.min.x);
	} else {
		if (entriesAlignmentH == AlignmentH::Center) {
			layerEntries->setX((listWidth - contentBounds.getWidth()) / 2.0f - contentBounds.min.x);
		} else if (entriesAlignmentH == AlignmentH::Right) {
			layerEntries->setX(listWidth - contentBounds.getWidth() - contentBounds.min.x);
		} else {
			throw LogicException(LOC);
		}
	}
}

void List2D::handleContentAlignmentY() {
	if (entriesAlignmentV == AlignmentV::Bottom) {
		layerEntries->setY(-contentBounds.min.y);
	} else {
		if (entriesAlignmentV == AlignmentV::Center) {
			layerEntries->setY((listHeight - contentBounds.getHeight()) / 2.0f - contentBounds.min.y);
		} else if (entriesAlignmentV == AlignmentV::Top) {
			layerEntries->setY(listHeight - contentBounds.getHeight() - contentBounds.min.y);
		} else {
			throw LogicException(LOC);
		}
	}
}

void List2D::updateEntriesListPos() {
	if ((columnsCount == 0 && rowsCount == 0)
		|| (columnsCount != 0 && rowsCount != 0))
	{
		throw LogicException(LOC);
	}

	contentBounds.set(
		0.0f, 0.0f,
		0.0f, 0.0f
	);
	if (!invertListGrowAxisX) {
		contentBounds.max.x = listPaddingLeft;
	} else {
		contentBounds.min.x = -listPaddingRight;
	}
	if (!invertListGrowAxisY) {
		contentBounds.max.y = listPaddingBottom;
	} else {
		contentBounds.min.y = -listPaddingTop;
	}

	float prevSpacedX = 0.0f;
	float prevSpacedY = 0.0f;

	float currentSecondaryAxisSegmentContentSizeWidth = 0.0f;
	float currentSecondaryAxisSegmentContentSizeHeight = 0.0f;

	float maxSecondaryAxisSegmentContentSizeWidth = 0.0f;
	float maxSecondaryAxisSegmentContentSizeHeight = 0.0f;

	int gY = 0;
	int gX = 0;

	bool isFirstEntry = true;
	for (int i = ((!invertEntriesDrawOrder) ? (0) : (entriesList.size() - 1)); 
		(!invertEntriesDrawOrder && i < entriesList.size()) || (invertEntriesDrawOrder && i >= 0);
		i = i + ((!invertEntriesDrawOrder) ? (1) : (-1)))
	{
		ListEntry2D* entry = entriesList.getDirect(i);

		IDynamicPropertiesObject2D* size = entry->getItemMustExistAs< IDynamicPropertiesObject2D * >("size");
		float entryWidth = size->getPropertyValue("width")->getDirectAs_float();
		float entryHeight = size->getPropertyValue("height")->getDirectAs_float();

		float entryPosX;
		float entryPosY;

		if (rowsCount > 0) {
			// Horizontal list.

			computeEntryPosContinuousLogic(
				isFirstEntry, invertListGrowAxisX, invertListGrowAxisY, listPaddingLeft, listPaddingRight, listPaddingBottom, listPaddingTop, entriesSpacingX, entriesSpacingY, entryWidth, entryHeight, rowsCount,
				gX, gY, prevSpacedX, prevSpacedY, currentSecondaryAxisSegmentContentSizeWidth, currentSecondaryAxisSegmentContentSizeHeight, maxSecondaryAxisSegmentContentSizeWidth, maxSecondaryAxisSegmentContentSizeHeight,
				entryPosX, entryPosY
			);
		} else if (columnsCount > 0) {
			// Vertical list.

			computeEntryPosContinuousLogic(
				isFirstEntry, invertListGrowAxisY, invertListGrowAxisX, listPaddingBottom, listPaddingTop, listPaddingLeft, listPaddingRight, entriesSpacingY, entriesSpacingX, entryHeight, entryWidth, columnsCount,
				gY, gX, prevSpacedY, prevSpacedX, currentSecondaryAxisSegmentContentSizeHeight, currentSecondaryAxisSegmentContentSizeWidth, maxSecondaryAxisSegmentContentSizeHeight, maxSecondaryAxisSegmentContentSizeWidth,
				entryPosY, entryPosX
			);
		} else {
			throw LogicException(LOC);
		}

		contentBounds.unionBounds(
			entryPosX, entryPosY,
			entryPosX + entryWidth, entryPosY + entryHeight
		);

		IDrawableNode2D* entryMainLayer = entry->getItemMustExistAs< IDrawableNode2D * >("layer");
		entryMainLayer->setPosition(entryPosX, entryPosY);

		if (isFirstEntry) {
			isFirstEntry = false;
		}
	}

	if (!invertListGrowAxisX) {
		contentBounds.max.x += listPaddingRight;
	} else {
		contentBounds.min.x -= listPaddingLeft;
	}
	if (!invertListGrowAxisY) {
		contentBounds.max.y += listPaddingTop;
	} else {
		contentBounds.min.y -= listPaddingBottom;
	}
}

void List2D::computeEntryPosContinuousLogic(
	bool isFirstEntry, bool invertListGrowAxisA, bool invertListGrowAxisB, float listPaddingAMin, float listPaddingAMax, float listPaddingBMin, float listPaddingBMax, float entriesSpacingA, float entriesSpacingB, float entrySizeA, float entrySizeB, int secondaryAxisSegmentEntriesCount,
	int& gA, int& gB, float& prevSpacedA, float& prevSpacedB, float& currentSecondaryAxisSegmentContentSizeA, float& currentSecondaryAxisSegmentContentSizeB, float& maxSecondaryAxisSegmentContentSizeA, float& maxSecondaryAxisSegmentContentSizeB,
	float& oEntryPosA, float& oEntryPosB)
{
	if (isFirstEntry) {
		if (!invertListGrowAxisA) {
			prevSpacedA = listPaddingAMin;
		} else {
			prevSpacedA = -listPaddingAMax - entrySizeA;
		}
	}
	if (gB == 0) {
		currentSecondaryAxisSegmentContentSizeB = 0.0f;
		if (!invertListGrowAxisB) {
			prevSpacedB = listPaddingBMin;
		} else {
			prevSpacedB = -listPaddingBMax - entrySizeB;
		}
	} else {
		currentSecondaryAxisSegmentContentSizeB += entriesSpacingB;
	}

	currentSecondaryAxisSegmentContentSizeA = Math::max(currentSecondaryAxisSegmentContentSizeA, entrySizeA);
	currentSecondaryAxisSegmentContentSizeB += entrySizeB;

	oEntryPosA = prevSpacedA;
	oEntryPosB = prevSpacedB;

	if (gB + 1 < secondaryAxisSegmentEntriesCount) {
		if (!invertListGrowAxisB) {
			prevSpacedB += (entrySizeB + entriesSpacingB);
		} else {
			prevSpacedB -= (entrySizeB + entriesSpacingB);
		}

		gB++;
	} else {
		if (!invertListGrowAxisA) {
			prevSpacedA += entrySizeA + entriesSpacingA;
		} else {
			prevSpacedA -= entrySizeA + entriesSpacingA;
		}

		currentSecondaryAxisSegmentContentSizeA = 0.0f;

		maxSecondaryAxisSegmentContentSizeB = Math::max(maxSecondaryAxisSegmentContentSizeB, currentSecondaryAxisSegmentContentSizeB);
		currentSecondaryAxisSegmentContentSizeB = 0.0f;

		gB = 0;
		gA++;
	}
}

void List2D::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	layerEntries = getItemMustExistAs< IContainer2D * >("layerEntries");

	touchAreaBack = getItemMustExistAs< IInteractiveDrawable2D * >("touchAreaBack");

	initializeListTouchAreaBackTouchListener();
}

ListEntry2D* List2D::createNewEntry(
	ArrayList<MenuItemConfig*>* useItemConfigs, ArrayList<StateChangeDurations*>* easeDurationsSList, int listPositionIndex, Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* extraDataMap,
	int entryUid)
{
	if (useItemConfigs == nullptr) {
		throw LogicException(LOC);
	}

	ListEntry2D* entry = createNewEntry(
		useItemConfigs, easeDurationsSList,
		entryUid
	);

	if (extraDataMap != nullptr) {
		entry->addDataMap(extraDataMap);
	}

	entry->reservedCreate();

	int entryIndex;
	if (listPositionIndex >= 0) {
		entryIndex = Math::min(listPositionIndex, entriesList.size());
	} else {
		entryIndex = Math::max(0, entriesList.size() + 1 + listPositionIndex);
	}

	IDrawableNode2D* entryMainLayer = entry->getItemOptionalAs<IDrawableNode2D * >("_root.layer");
	if (entryMainLayer == nullptr) {
		throw LogicException(LOC);
	}
	layerEntries->addChild(entryMainLayer);

	entriesList.insertDirect(entryIndex, entry);
	entriesCount++;

	IInteractiveDrawable2D* touchArea = entry->getItemOptionalAs< IInteractiveDrawable2D * >("_root.touchArea");
	if (touchArea != nullptr) {
		touchArea->getUserDataMap()->putDirectB("listEntry", IWrappedValue::new_Custom(entry, -1));

		initializeNewEntryTouchHandling(touchArea);
	}

	onListEntryCreated(entry);

	doFullVisualsSync();

	return entry;
}

ListEntry2D* List2D::createNewEntry(
	ArrayList<MenuItemConfig*>* useItemConfigs, ArrayList<StateChangeDurations*>* easeDurationsSList,
	int entryUid)
{
	ListEntry2D* entry = new ListEntry2D(
		appMetrics, appAssets, useItemConfigs, easeDurationsSList, this,
		entryUid
	);

	return entry;
}

void List2D::onListEntryCreated(ListEntry2D* listEntry) {
	//void
}

void List2D::onListEntryRemove_pre(ListEntry2D* listEntry) {
	//void
}

void List2D::removeEntry(ListEntry2D* listEntry) {
	int entryIndex = entriesList.indexOf(listEntry);

	onListEntryRemove_pre(listEntry);

	sanitizeEntry(listEntry);

	entriesList.remove(entryIndex);
	entriesCount--;

	doFullVisualsSync();
}

ListEntry2D* List2D::createNewEntry(
	ArrayList<MenuItemConfig*>* itemInstanceConfigOverride, ArrayList<StateChangeDurations*>* easeDurationsSList, Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* extraDataMap,
	int entryUid)
{
	return createNewEntry(
		itemInstanceConfigOverride, easeDurationsSList, -1, extraDataMap, 
		entryUid
	);
}

void List2D::initializeNewEntryTouchHandling(IInteractiveDrawable2D* touchArea) {
	touchArea->addTouchListener(&touchListener);
}

void List2D::removeAllEntries() {
	if (entriesList.count() == 0) {
		return;
	}

	for (int i = entriesList.size() - 1; i >= 0; i--) {
		ListEntry2D* entry = entriesList.getDirect(i);

		if (entry != nullptr) {
			sanitizeEntry(entry);

			entriesList.remove(i);
			entriesCount--;
		}
	}

	doFullVisualsSync();
}

ListEntry2D* List2D::getEtryWithIndex(int index) {
	ListEntry2D* entry = entriesList.getDirect(index);
	if (entry != nullptr) {
		return entry;
	}

	return nullptr;
}

ListEntry2D* List2D::getEtryWithUid(int entryUid) {
	for (int i = entriesList.size() - 1; i >= 0; i--) {
		ListEntry2D* entry = entriesList.getDirect(i);

		if (entry->entryUid < 0) {
			// When calling this function, need to provide a entryUid for each entry when calling the create function.
			throw LogicException(LOC);
		}

		if (entry->entryUid == entryUid) {
			return entry;
		}
	}

	return nullptr;
}

void List2D::setBgTintColor(Color& bgTintColor) {
	this->bgTintColor.set(bgTintColor);
}

void List2D::sanitizeEntry(ListEntry2D* entry) {
	IDrawableNode2D* entryMainLayer = entry->getItemMustExistAs< IDrawableNode2D * >("_root.layer");
	if (entryMainLayer->getParent() != nullptr) {
		entryMainLayer->getParent()->removeChild(entryMainLayer);
	}

	entry->reservedDisposeMain();
}

std::shared_ptr<base::IWrappedValue> List2D::getPropertyValue(std::string property) {
	if (property == "x") {
		return base::IWrappedValue::new_float(posX);
	} else if (property == "y") {
		return base::IWrappedValue::new_float(posY);
	} else if (property == "anchorX") {
		return base::IWrappedValue::new_float(anchorX);
	} else if (property == "anchorY") {
		return base::IWrappedValue::new_float(anchorY);
	}

	else if (property == "entriesAlignmentH") {
		return base::IWrappedValue::new_int(entriesAlignmentH);
	} else if (property == "entriesAlignmentV") {
		return base::IWrappedValue::new_int(entriesAlignmentV);
	} else if (property == "entriesSpacingX") {
		return base::IWrappedValue::new_float(entriesSpacingX);
	} else if (property == "entriesSpacingY") {
		return base::IWrappedValue::new_float(entriesSpacingY);
	} else if (property == "listPaddingTop") {
		return base::IWrappedValue::new_float(listPaddingTop);
	} else if (property == "listPaddingBottom") {
		return base::IWrappedValue::new_float(listPaddingBottom);
	} else if (property == "listPaddingLeft") {
		return base::IWrappedValue::new_float(listPaddingLeft);
	} else if (property == "listPaddingRight") {
		return base::IWrappedValue::new_float(listPaddingRight);
	}
	
	else if (property == "manualListSizeWidth") {
		return base::IWrappedValue::new_float(manualListSizeWidth);
	} else if (property == "manualListSizeHeight") {
		return base::IWrappedValue::new_float(manualListSizeHeight);
	}
	
	else if (property == "autoListSizeMaxWidth") {
		return base::IWrappedValue::new_float(autoListSizeMaxWidth);
	} else if (property == "autoListSizeMaxHeight") {
		return base::IWrappedValue::new_float(autoListSizeMaxHeight);
	}
	
	else if (property == "rowsCount") {
		return base::IWrappedValue::new_int(rowsCount);
	} else if (property == "columnsCount") {
		return base::IWrappedValue::new_int(columnsCount);
	}
	
	else if (property == "invertListGrowAxisX") {
		return base::IWrappedValue::new_bool(invertListGrowAxisX);
	}else if (property == "invertListGrowAxisY") {
		return base::IWrappedValue::new_bool(invertListGrowAxisY);
	}
	
	else if (property == "invertEntriesDrawOrder") {
		return base::IWrappedValue::new_bool(invertEntriesDrawOrder);
	}
	
	else if (property == "autoListSizeToContent") {
		return base::IWrappedValue::new_bool(autoListSizeToContent);
	}
	
	else if (property == "bgTintColor") {
		return base::IWrappedValue::new_Color(bgTintColor);
	} else if (property == "bgAlpha") {
		return base::IWrappedValue::new_float(bgAlpha);
	}

	else if (property == "entryActivateSoundConfig_id") {
		return base::IWrappedValue::new_String_ptr(entryActivateDSound->getSound_id());
	}else if (property == "entryActivateSound_volume") {
		return base::IWrappedValue::new_float(entryActivateSound_volume);
	}

	else if (property == "listWidth") {
		return ::IWrappedValue::new_float(listWidth);
	} else if (property == "listHeight") {
		return ::IWrappedValue::new_float(listHeight);
	}

	throw LogicException(LOC);
}

void List2D::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
	bool isChange = false;

	if (property == "x") {
		isChange = CheckedSet(posX, value->getDirectAs_float());
	} else if (property == "y") {
		isChange = CheckedSet(posY, value->getDirectAs_float());
	} else if (property == "anchorX") {
		isChange = CheckedSet(anchorX, value->getDirectAs_float());
	} else if (property == "anchorY") {
		isChange = CheckedSet(anchorY, value->getDirectAs_float());
	}

	else if (property == "entriesAlignmentH") {
		isChange = CheckedSet(entriesAlignmentH, value->getDirectAs_int());
	} else if (property == "entriesAlignmentV") {
		isChange = CheckedSet(entriesAlignmentV, value->getDirectAs_int());
	} else if (property == "entriesSpacingX") {
		isChange = CheckedSet(entriesSpacingX, value->getDirectAs_float());
	} else if (property == "entriesSpacingY") {
		isChange = CheckedSet(entriesSpacingY, value->getDirectAs_float());
	} else if (property == "listPaddingTop") {
		isChange = CheckedSet(listPaddingTop, value->getDirectAs_float());
	} else if (property == "listPaddingBottom") {
		isChange = CheckedSet(listPaddingBottom, value->getDirectAs_float());
	} else if (property == "listPaddingLeft") {
		isChange = CheckedSet(listPaddingLeft, value->getDirectAs_float());
	} else if (property == "listPaddingRight") {
		isChange = CheckedSet(listPaddingRight, value->getDirectAs_float());
	}
	
	else if (property == "manualListSizeWidth") {
		isChange = CheckedSet(manualListSizeWidth, value->getDirectAs_float());
	} else if (property == "manualListSizeHeight") {
		isChange = CheckedSet(manualListSizeHeight, value->getDirectAs_float());
	}
	
	else if (property == "autoListSizeMaxWidth") {
		isChange = CheckedSet(autoListSizeMaxWidth, value->getDirectAs_float());
	} else if (property == "autoListSizeMaxHeight") {
		isChange = CheckedSet(autoListSizeMaxHeight, value->getDirectAs_float());
	}
	
	else if (property == "rowsCount") {
		isChange = CheckedSet(rowsCount, value->getDirectAs_int());
	} else if (property == "columnsCount") {
		isChange = CheckedSet(columnsCount, value->getDirectAs_int());
	}
	
	else if (property == "invertListGrowAxisX") {
		isChange = CheckedSet(invertListGrowAxisX, value->getDirectAs_bool());
	}else if (property == "invertListGrowAxisY") {
		isChange = CheckedSet(invertListGrowAxisY, value->getDirectAs_bool());
	}
	
	else if (property == "invertEntriesDrawOrder") {
		isChange = CheckedSet(invertEntriesDrawOrder, value->getDirectAs_bool());
	}
	
	else if (property == "autoListSizeToContent") {
		isChange = CheckedSet(autoListSizeToContent, value->getDirectAs_bool());
	}
	
	else if (property == "bgTintColor") {
		isChange = CheckedSet(&bgTintColor, value->getPointerOfTypeAs_Color());
	} else if (property == "bgAlpha") {
		isChange = CheckedSet(bgAlpha, value->getDirectAs_float());
	}

	else if (property == "entryActivateSoundConfig_id") {
		std::string* s = value->getDirectAs_String_ptr();
		if (entryActivateDSound->getSound_id() != s) {
			isChange = true;
		}
		entryActivateDSound->setSound_id(s);
	}else if (property == "entryActivateSound_volume") {
		isChange = CheckedSet(entryActivateSound_volume, value->getDirectAs_float());
	}

	else if (property == "listWidth") {
		// Read only.

		throw LogicException(LOC);
	} else if (property == "listHeight") {
		// Read only.

		throw LogicException(LOC);
	}

	else {
		throw LogicException(LOC);
	}

	if (isChange) {
		setIsDirty_property();
	}
}

void List2D::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	for (int i = 0; i < entriesList.size(); i++) {
		ListEntry2D* entry = entriesList.getDirect(i);
		if (entry != nullptr) {
			entry->visualLoop(deltaS, deltaMS);
		}
	}
}

void List2D::disposeMain() {
	if (entriesList.count() > 0) {
		removeAllEntries();
	}

	super::disposeMain();
}

List2D::~List2D() {
	//void
}
