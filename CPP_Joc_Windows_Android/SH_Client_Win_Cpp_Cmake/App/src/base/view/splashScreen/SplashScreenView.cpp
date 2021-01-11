#include <base/util/AppStyles.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include <base/visual2D/util/VisualCreateConfig.h>
#include <base/visual2D/util/VCC_Group.h>	
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_Triangles2D.h>
#include "SplashScreenView.h"
#include <base/statics/StaticsInit.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <graphics/visual2d/drawable/Triangles2DDrawable.h>
#include <graphics/visual2d/drawable/Sprite2D.h>
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include <graphics/util/GraphicsUtil.h>

using namespace base;

ArrayList<MenuItemConfig*>* SplashScreenView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_appSize.width")
			->put("height", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_in_", 1.0f, EaseConfig::ACCELERATION)
				->putState("_out_", 0.0f, EaseConfig::ACCELERATION)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg", "layer", VCC_Triangles2D::New(
			// Counter-clockwise direction and starts at bottom left.
			new Array1D<ITriangles2DDrawable::LocalVertex>(
				4,
				new ITriangles2DDrawable::LocalVertex[4]{
					ITriangles2DDrawable::LocalVertex{Vector2{0.0f, 0.0f}, Vector2{-1, -1}, Color{"#8CB5F8FF"}},
					ITriangles2DDrawable::LocalVertex{Vector2{1.0f, 0.0f}, Vector2{-1, -1}, Color{"#8CB5F8FF"}},
					ITriangles2DDrawable::LocalVertex{Vector2{1.0f, 1.0f}, Vector2{-1, -1}, Color{"#4285F4FF"}},
					ITriangles2DDrawable::LocalVertex{Vector2{0.0f, 1.0f}, Vector2{-1, -1}, Color{"#4285F4FF"}}
				}
			),
			// Counter-clockwise triangles, startrs at bottom left.
			new Array1D<unsigned short>(
				6,
				new unsigned short[6]{
					0, 1, 2,
					0, 2, 3
				}
				)
		), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->put("y", 0.0f, "_dpm.height")
			->put("scaleX", 1.0f, "props.width")
			->put("scaleY", 1.0f, "props.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("imgTitle", "layer", VCC_Triangles2D::New(
			graphics::GraphicsUtil::GenerateTextGeometry2D("PIXEL"/*text*/, 1.0f/*geometryWidth*/, Vector2{ 0.5f, 0.0f }/*anchor*/, Vector2{ 0.0f, 0.0f }/*pos*/, Color{ "#FFFFFFFF" })
		), (new VisualPropertiesConfig())
			->put("scaleX", 249.0f, "_dpm.width")
			->put("scaleY", 249.0f, "_dpm.width")
			->put("x", 0.5f, "_appSize.width")
			->put("y",
				0.5f, "_appSize.height",
				VisualValueConfig::OPERATION_ADD,
				100.0f, "_dpm.width"
			)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_in_", 1.0f, EaseConfig::DECELERATION)
				->putState("_out_", 0.0f, EaseConfig::ACCELERATION)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("imgLogo", "layer", VCC_Triangles2D::New(
			// Counter-clockwise direction and starts at bottom left.
			new Array1D<ITriangles2DDrawable::LocalVertex>(
				4,
				new ITriangles2DDrawable::LocalVertex[4]{
					ITriangles2DDrawable::LocalVertex{Vector2{-0.5f, -0.5f}, Vector2{-1, -1}, Color{"#FFFFFFFF"}},
					ITriangles2DDrawable::LocalVertex{Vector2{0.5f, -0.5f}, Vector2{-1, -1}, Color{"#FFFFFFFF"}},
					ITriangles2DDrawable::LocalVertex{Vector2{0.5f, 0.5f}, Vector2{-1, -1}, Color{"#FFFFFFFF"}},
					ITriangles2DDrawable::LocalVertex{Vector2{-0.5f, 0.5f}, Vector2{-1, -1}, Color{"#FFFFFFFF"}}
				}
			),
			// Counter-clockwise triangles, startrs at bottom left.
			new Array1D<unsigned short>(
				6,
				new unsigned short[6]{
					0, 1, 2,
					0, 2, 3
				}
			)
		), (new VisualPropertiesConfig())
			// Note. Scale to max of app height and width so the logo remain a square or the rotation will look off.
			->putAnimating("scaleX", (new MenuVisualAnimationProperty())
				->putState("_in_", 74.0f, "_dpm.width", EaseConfig::DECELERATION)
				->putState("_out_",
					1.0f, "_appSize.height", 
					VisualValueConfig::OPERATION_ADD,
					1.0f, "_appSize.width",
					EaseConfig::ACCELERATION
				)
			)
			->putAnimating("scaleY", (new MenuVisualAnimationProperty())
				->putState("_in_", 74.0f, "_dpm.width", EaseConfig::DECELERATION)
				->putState("_out_",
					1.0f, "_appSize.height",
					VisualValueConfig::OPERATION_ADD,
					1.0f, "_appSize.width",
					EaseConfig::ACCELERATION
				)
			)

			->put("x", 0.5f, "props.width")
			->put("y", 0.5f, "props.height")
			/*->putAnimating("alpha", (new MenuVisualAnimationProperty())
					->putState("_in_", 1.0f, AppStyles.EASE_FUNCTION_TEST_ID, AppStyles.EASE_FUNCTION_TEST_CONFIG)
					->putState("_out_", 0.0f, AppStyles.EASE_FUNCTION_TEST_ID, AppStyles.EASE_FUNCTION_TEST_CONFIG)
			)*/
			->putAnimating("rotationD", (new MenuVisualAnimationProperty())
				->putState("_in_", 0.0f, EaseConfig::DECELERATION)
				->putState("_out_", -90.0f, EaseConfig::ACCELERATION)
			)
		));
});

ArrayList<StateChangeDurations*>* SplashScreenView::viewAnimationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	viewAnimationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_SPLASH_SCREEN_OUT_DURATION_S));
});

SplashScreenView::SplashScreenView(IApp* app, IHandlerAutoDisposal* handlerAutoDisposal)
    : super(app, viewItemConfigs, viewAnimationDurations, handlerAutoDisposal)
{
    //void
}

void SplashScreenView::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	//void
}

SplashScreenView::~SplashScreenView() {
    //void
}
