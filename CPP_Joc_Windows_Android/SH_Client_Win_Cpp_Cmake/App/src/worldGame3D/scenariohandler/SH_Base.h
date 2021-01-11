#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include <base/component/IComponent_AppComponent.h>
#include <graphics/visual2d/drawable/IContainer2D.h>

class IAppAssets;
class IAppMetrics;
class IAppUtils;
class IApp;

namespace worldGame3D {
class SH_Base : public ComponentsHandler, public virtual IComponent_AppComponent {priv typedef ComponentsHandler super;pub dCtor(SH_Base);
	prot IAppAssets* appAssets;
	prot IAppMetrics* appMetrics;
	prot IAppUtils* appUtils;
	prot IApp* app;

	prot IContainer2D* layer2DGameWorld;

	pub explicit SH_Base(
        IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
        IContainer2D* layer2DGameWorld,
		IApp* app);
	
	prot IContainer2D* getLayer2DGameWorld();
	
	pub ~SH_Base() override;
};
};
