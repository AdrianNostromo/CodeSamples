#pragma once

#include <base/gh.h>
#include "CG_Entries.h"

namespace base {
class CG_IndexFileHandler : public CG_Entries {priv typedef CG_Entries super;pub dCtor(CG_IndexFileHandler);
	prot enum LoadIndexFileRet { Success, Fail_MissingOrCorrupt, Fail_WrongVersion };

	priv static std::string INDEX_FILE_NAME;

    pub explicit CG_IndexFileHandler(std::string& cacheGroupId);

	prot void saveIndexFile();
	prot LoadIndexFileRet loadIndexFile(int targetVersionIndex);

	prot void onEntryChanged() override;

    pub ~CG_IndexFileHandler() override;
};
};
