#include "DeV_DynamicEntries.h"
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include <base/visual2D/util/VisualCreateConfig.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/util/StringUtil.h>
#include <base/fileSystem/fileHandler/util/FileIOProfilerStatics.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include "entry/DebugEntry.h"
#include "entryUser/DebugEntryUser.h"
#include <base/visual2D/VisualUtil2D.h>
#include <base/app/IAppAssets.h>
#include <base/app/IAppMetrics.h>
#include <graphics/visual2d/drawable/ToggleSwitch2D.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <base/opengl/util/GLUtil.h>
#include <base/opengl/IGL.h>
#include <base/screen/IScreen.h>

using namespace base;

ManagedAssetPath* DeV_DynamicEntries::FontPreloadingPreparer_0 = ManagedAssetPath::stringPathToManagedFontPath("fonts/RobotoMono-Medium-AaNSS-14-_dpFont_");

MenuItemConfig* DeV_DynamicEntries::new_viewItemConfig_bodyDynamics() {
	return new MenuItemConfig_ComposedVisual("bodyDynamics", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("size", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_appSize.width")
			->put("height", 1.0f, "_appSize.height")
		))
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("entriesPosX", 7.0f, "_dpm.height")
			->put("entryRightMarginPosX",
				1.0f, "size.width",
				VisualValueConfig::OPERATION_ADD,
				-5.0f, "_dpm.width"
			)
			// Add space for the top row that contains buttons.
			->put("entriesListPaddingY", 5.0f + 24.0f + 5.0f, "_dpm.height")
			->put("entrySizeY", 22.0f, "_dpm.height")
			->put("btnSizeX", 48.0f, "_dpm.width")
			->put("btnSizeY", 24.0f, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("y",
				1.0f, "size.height",
				VisualValueConfig::OPERATION_ADD,
				-(5.0f + 56.0f), "_dpm.height"
			)

			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_in_._isDynamicPage:true_", 1.0f, EaseConfig::ACCELERATION)
				->putState("_", 0.0f, EaseConfig::ACCELERATION)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_in_._isDynamicPage:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("darkBg", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width",
				1.0f, "size.width",
				VisualValueConfig::OPERATION_ADD,
				-5.0f * 2.0f, "_dpm.width"
			)
			// Height is manually set;
			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 5.0f, "_dpm.width")
			->put("y", 0.0f, "_dpm.height")
			->put("tintColor", Color("#444444ff"))
			->put("alpha", 0.6f)
		))

		// Not. Use RobotoMono-Regular here at 12 because it won't be initialised for the preloader and it is required in dynamic content in this class.
		->appendDirect_chain(new MenuItemConfig_Visual("lblClearMinMax", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/RobotoMono-Medium-AaNSS-14-_dpFont_"), "Clear min, max"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/RobotoMono")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f, "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("btnClearMinMaxCumulative", "layer", VCC_SpriteButton::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF",
			&base::SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 1.0f)
			->put("anchorY", 1.0f)
			->put("x",
				1.0f, "size.width",
				VisualValueConfig::OPERATION_ADD,
				-5.0f, "_dpm.width"
			)
			->put("y", 0.0f, "_dpm.height")
			->put("tintColor", Color("#000088ff"))
			->put("alpha", 0.8f)
		))
	);
}

DeV_DynamicEntries::DeV_DynamicEntries(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
	: super(app, viewItemConfigs, viewEaseDurationsSList)
{
	touchListener.cb_onCursorDown = std::bind(&DeV_DynamicEntries::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

void DeV_DynamicEntries::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	// Grab the metrics object.
	props = getItemMustExistAs<IDynamicPropertiesObject2D*>("bodyDynamics.props");

	mainLayer = getItemMustExistAs<IContainer2D*>("bodyDynamics.layer");
	darkBg = getItemMustExistAs<ISizedDrawable2D*>("bodyDynamics.darkBg");

	lblClearMinMax = getItemMustExistAs<base::BitmapLabel*>("bodyDynamics.lblClearMinMax");

	(btnClearMinMaxCumulative = getItemMustExistAs<base::SpriteButton2D*>("bodyDynamics.btnClearMinMaxCumulative"))->addTouchListener(&touchListener);
}

int DeV_DynamicEntries::registerNewPage(bool isDefaultPage, bool usesDynamicEntries) {
	int pageIndex = pagesList.count();
	if (isDefaultPage) {
		if (defaultPageIndex >= 0) {
			// Default page already set.
			throw LogicException(LOC);
		}
		defaultPageIndex = pageIndex;
	}

	IContainer2D* vContainer = nullptr;
	if (usesDynamicEntries) {
		vContainer = new Container2D();
		mainLayer->addChild(vContainer);
		vContainer->setIsVisible(false);
	}
	EntriesPage* page = new EntriesPage(vContainer);
	pagesList.insertDirect(pageIndex, page);

	page->isPositionsInvalid = true;

	return pageIndex;
}

int DeV_DynamicEntries::getActivePageIndex() {
	return activePageIndex;
}

void DeV_DynamicEntries::gotoNextPage() {
	if (pagesList.count() <= 0) {
		return;
	}

	if (activePageIndex >= 0) {
		EntriesPage* page = pagesList.getDirect(activePageIndex);
		if (page->vContainer != nullptr) {
			page->vContainer->setIsVisible(false);
		}
	}

	if (activePageIndex + 1 >= pagesList.count()) {
		activePageIndex = -1;

		GLUtil::gl->disableProfiler();
	} else {
		activePageIndex++;

		if (activePageIndex == 0) {
			GLUtil::gl->enableProfiler();
		}
	}

	if (activePageIndex >= 0) {
		EntriesPage* page = pagesList.getDirect(activePageIndex);
		if (page->vContainer != nullptr) {
			page->vContainer->setIsVisible(true);
			// Do this to make the bg resize.
			page->isPositionsInvalid = true;
		}

		lblClearMinMax->setTextDirect(std::string("Page index: ") + std::to_string(activePageIndex) + ". Clear min, max");
	}

	updateShowStateIfChanged(true, nullptr);

	onActivePageChanged();
}

void DeV_DynamicEntries::onActivePageChanged() {
	//void
}

bool DeV_DynamicEntries::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive() || isAnimating()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnClearMinMaxCumulative)) {
		clearMinMaxCumulative();
	}

	else {
		throw LogicException(LOC);
	}

	return true;
}

void DeV_DynamicEntries::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
	super::syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

	// Grab the metrics.
	entriesPosX = props->getPropertyValue("entriesPosX")->getDirectAs_float();
	entryRightMarginPosX = props->getPropertyValue("entryRightMarginPosX")->getDirectAs_float();
	entriesListPaddingY = props->getPropertyValue("entriesListPaddingY")->getDirectAs_float();
	entrySizeY = props->getPropertyValue("entrySizeY")->getDirectAs_float();

	btnSize.set(
		props->getPropertyValue("btnSizeX")->getDirectAs_float(),
		props->getPropertyValue("btnSizeY")->getDirectAs_float()
	);
}

void DeV_DynamicEntries::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	if (!isActive()) {
		return;
	}

	EntriesPage* page = nullptr;
	if (activePageIndex >= 0) {
		page = pagesList.getDirect(activePageIndex, nullptr);
	}

	if (page != nullptr) {
		if (page->invalidEntriesList.count() > 0) {
			ListDL<DebugEntry*>::Entry* entry;
			while ((entry = page->invalidEntriesList.getFirst()) != nullptr) {
				DebugEntry* debugEntry = entry->data;

				debugEntry->isDirty_data = false;

				debugEntry->syncLblText();

				debugEntry->inHandler_invalidEntriesListEntry = nullptr;
				entry->remove();
			}
		}
	}
}

void DeV_DynamicEntries::visualLoopB(float deltaS, int deltaMS) {
	super::visualLoopB(deltaS, deltaMS);

	if (!isActive()) {
		return;
	}

	EntriesPage* page = nullptr;
	if (activePageIndex >= 0) {
		page = pagesList.getDirect(activePageIndex, nullptr);
	}

	if (page != nullptr) {
		if (page->isPositionsInvalid) {
			page->isPositionsInvalid = false;

			int ct = 0;
			for (ListDL<DebugEntry>::Entry* entry = page->entriesList.getFirst(); entry != nullptr; entry = entry->next) {
				DebugEntry* debugEntry = &entry->data;

				debugEntry->lbl->setPosition(
					entriesPosX,
					-entriesListPaddingY - ct * entrySizeY
				);
				if (debugEntry->toggleSwitch != nullptr) {
					debugEntry->toggleSwitch->setPosition(entryRightMarginPosX, -entriesListPaddingY - ct * entrySizeY);
				}
				if (debugEntry->btn != nullptr) {
					debugEntry->btn->setSize(btnSize.x, btnSize.y);
					debugEntry->btn->setPosition(entryRightMarginPosX, -entriesListPaddingY - ct * entrySizeY);
				}

				ct += debugEntry->linesCount;
			}

			darkBg->setHeight(entriesListPaddingY * 2.0f + ct * entrySizeY);
		}
	}
}

std::shared_ptr<base::IDebugEntryUser> DeV_DynamicEntries::addDebugEntry(int linesCount, bool hasToggleSwitch, int pageIndex, bool hasBtn) {
	if (pageIndex < 0) {
		if (defaultPageIndex < 0) {
			// No page speciffied and the default page doesn't exist.
			throw LogicException(LOC);
		}

		pageIndex = defaultPageIndex;
	}
	
	EntriesPage* page = pagesList.getDirect(pageIndex);

	// This will not be loaded unless it is used in a static template. Make sure it is the same as in the above template.
	std::string initialText = "Data: ###";
	Color col{ "#FFFFFFFF" };

	BitmapLabel* lbl = VisualUtil2D::newBitmapLabel(
		appAssets->getEmbeddedAssetsManager(), 
		initialText,
		page->vContainer
	);
	lbl->reservedCreate();
	
	lbl->setFontNameDirect("fonts/RobotoMono");
	lbl->setFontWeight(VCC_BitmapLabel::FontWeight::Medium);
	lbl->setFontSize((int)Math::floor(14.0f * appMetrics->getScreen()->getScreenDensityMultiplier_fonts()));
	lbl->setCharactersRangeDirect("AaNSS");

	lbl->setAnchor(0.0f, 1.0f);
	lbl->setAlpha(1.0f);
	lbl->setTintColor(col);
	lbl->setLinesCountOverride(-1);

	base::ToggleSwitch2D* toggleSwitch = nullptr;
	if (hasToggleSwitch) {
		toggleSwitch = new base::ToggleSwitch2D(appMetrics, appAssets);
		
		toggleSwitch->putStaticAnimatingPropertyStatusDirect("x", true);
		toggleSwitch->putStaticAnimatingPropertyStatusDirect("y", true);
		toggleSwitch->putStaticAnimatingPropertyStatusDirect("anchorX", true);
		toggleSwitch->putStaticAnimatingPropertyStatusDirect("anchorY", true);

		toggleSwitch->reservedCreate();
		
		toggleSwitch->setAnchor(1.0f, 1.0f);
		toggleSwitch->setToggleOffSound(&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f/*volume*/);
		toggleSwitch->setToggleOnSound(&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f/*volume*/);

		page->vContainer->addChild(toggleSwitch->getVisualsHolder());

		toggleSwitch->gotoShowState(toggleSwitch->computeStateFlags("_in_"), false, nullptr, false);
	}
	
	base::SpriteButton2D* btn = nullptr;
	if (hasBtn) {
		std::string atlasID = "visual2D/taUtilLinear.atlas";
		std::string buttonAssetsPrefix = "bg/simple_white_FFFFFF";
		btn = VisualUtil2D::newSpriteButton2D(
			appAssets->getEmbeddedAssetsManager(),
			atlasID/*atlasID*/, buttonAssetsPrefix/*buttonAssetsPrefix*/,
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f/*volume*/,
			nullptr/*touchUpSoundConfig_id*/, 0.0f/*touchUpSound_volume*/,
			nullptr/*disabledTouchDownSoundConfig_id*/, 0.0f/*disabledTouchDownSound_volume*/,
			nullptr/*parent*/
		);

		btn->setAnchor(1.0f, 1.0f);

		page->vContainer->addChild(btn);
	}

	ListDL<DebugEntry>::Entry* listEntry = page->entriesList.appendEmplace(this, pageIndex, linesCount, lbl, toggleSwitch, btn);
	DebugEntry* debugEntry = &listEntry->data;
	listEntry->data.inHandler_listEntry = listEntry;
	if (debugEntry->toggleSwitch) {
		debugEntry->toggleSwitchLoopManager_listEntry = manualSmartItemsList.appendDirect(toggleSwitch);

		debugEntry->toggleSwitch->setEventListener(&debugEntry->toggleSwitchListener);
	}
	if (debugEntry->btn) {
		debugEntry->btn->addTouchListener(&debugEntry->btnTouchListener);
	}

	std::shared_ptr<DebugEntryUser> debugEntryUser = std::make_shared<DebugEntryUser>(debugEntry);
	debugEntry->hookedUser = debugEntryUser.get();

	debugEntry->invalidateData();

	page->isPositionsInvalid = true;

	return debugEntryUser;
}

void DeV_DynamicEntries::removeDebugEntry(DebugEntry* debugEntry) {
	if (debugEntry->inHandler_listEntry == nullptr || debugEntry->hookedUser->debugEntry == nullptr) {
		throw LogicException(LOC);
	}

	EntriesPage* page = pagesList.getDirect(debugEntry->pageIndex);

	debugEntry->hookedUser->debugEntry = nullptr;
	debugEntry->hookedUser = nullptr;

	if (debugEntry->lbl != nullptr) {
		debugEntry->lbl->reservedDispose();
		delete debugEntry->lbl;
		debugEntry->lbl = nullptr;
	}

	if (debugEntry->toggleSwitch != nullptr) {
		if (debugEntry->toggleSwitchLoopManager_listEntry == nullptr) {
			// The toggle switch must be in manualSmartItemsList.
			throw LogicException(LOC);
		}

		debugEntry->toggleSwitchLoopManager_listEntry->remove();

		debugEntry->toggleSwitch->reservedDisposeMain();
		delete debugEntry->toggleSwitch;
		debugEntry->toggleSwitch = nullptr;
	}
	if (debugEntry->btn != nullptr) {
		debugEntry->btn->reservedDisposeMain();
		delete debugEntry->btn;
		debugEntry->btn = nullptr;
	}

	// This releases the sp memory, call after.
	if (debugEntry->inHandler_invalidEntriesListEntry != nullptr) {
		debugEntry->inHandler_invalidEntriesListEntry->remove();
		debugEntry->inHandler_invalidEntriesListEntry = nullptr;
	}
	debugEntry->inHandler_listEntry->remove();
	// Don't make nullptr because the memory for debugEntry got released in the above function call.
	//debugEntry->inHandler_listEntry = nullptr;

	page->isPositionsInvalid = true;
}

void DeV_DynamicEntries::onDebugEntryInvalidation(DebugEntry* entry) {
	if (!entry->isDirty_data) {
		entry->isDirty_data = true;

		if (entry->inHandler_invalidEntriesListEntry != nullptr) {
			throw LogicException(LOC);
		}

		EntriesPage* page = pagesList.getDirect(entry->pageIndex);

		entry->inHandler_invalidEntriesListEntry = page->invalidEntriesList.appendDirect(entry);
	}
}

void DeV_DynamicEntries::onDebugEntryLinesCountChanged(DebugEntry* entry) {
	EntriesPage* page = pagesList.getDirect(entry->pageIndex);
	page->isPositionsInvalid = true;
}

void DeV_DynamicEntries::clearMinMaxCumulative() {
	//void
}

std::string DeV_DynamicEntries::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	state += "_activePageIndex:" + std::to_string(activePageIndex) + "_";
	if (activePageIndex >= 0 && pagesList.getDirect(activePageIndex)->vContainer != nullptr) {
		state += "_isDynamicPage:true_";
	} else {
		state += "_isDynamicPage:false_";
	}

	return state;
}

void DeV_DynamicEntries::CheckedSetDebugEntry(std::shared_ptr<base::WrappedValue<int>> ddeValue, int newVal, bool& isChange) {
	if (ddeValue->getReference() != newVal) {
		ddeValue->setReference(newVal);

		isChange = true;
	}
}

void DeV_DynamicEntries::dispose() {
	for (int i = pagesList.count() - 1; i >= 0; i--) {
		EntriesPage* page = pagesList.getDirect(i);
		if (page == nullptr) {
			continue;
		}

		ListDL<DebugEntry*>::Entry* entry;
		while ((entry = page->invalidEntriesList.getFirst()) != nullptr) {
			entry->data->inHandler_invalidEntriesListEntry = nullptr;

			entry->remove();
		}

		if (page->entriesList.count() > 0) {
			// All connected debug entry users must disconnect before this is disposed.
			throw LogicException(LOC);
		}

		if (page->vContainer != nullptr) {
			page->vContainer->reservedDispose();
			delete page->vContainer;
			page->vContainer = nullptr;
		}

		delete page;
	}
	
	super::dispose();
}

DeV_DynamicEntries::~DeV_DynamicEntries() {
	//void
}
