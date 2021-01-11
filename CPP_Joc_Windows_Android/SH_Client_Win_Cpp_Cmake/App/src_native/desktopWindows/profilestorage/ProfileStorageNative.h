#pragma once

#include <base/gh.h>
#include <base/profilestorage/ProfileStorageBase.h>

namespace desktopWindows {
class ProfileStorageNative : public base::ProfileStorageBase {priv typedef base::ProfileStorageBase super;pub dCtor(ProfileStorageNative);

	pub explicit ProfileStorageNative();

	pub ~ProfileStorageNative() override;
};
};
