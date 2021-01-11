#pragma once

#include <base/gh.h>
#include "IFileSystem.h"
#include <base/object/ManagedObject.h>

class IApp;

namespace base {
class FileSystemBase : public ManagedObject, virtual public IFileSystem {priv typedef ManagedObject super; pub dCtor(FileSystemBase);
	prot IApp* app = nullptr;

    pub explicit FileSystemBase();
	pub void initVars(IApp* app) final;

	pub ~FileSystemBase() override;
};
};
