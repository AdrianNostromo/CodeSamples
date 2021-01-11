#pragma once

#include <base/gh.h>
#include <base/app/IApp.h>

namespace worldGame3D {
	class ScenariosHandler;
	class IServer;
};

namespace worldGame3D {
class IAppGame : virtual public IApp {
	pub virtual worldGame3D::ScenariosHandler* getScenariosHandler() = 0;

	virtual IServer* getServer() = 0;

    pub ~IAppGame() override = default;
};
};
