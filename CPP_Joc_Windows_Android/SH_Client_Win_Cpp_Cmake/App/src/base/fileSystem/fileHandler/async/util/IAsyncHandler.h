#pragma once

#include <base/gh.h>
#include <memory>
#include <base/fileSystem/fileHandler/async/util/AsyncFHEvent.h>

namespace base {
	class FileHandlerAsyncHdd;
	class AsyncFileOperation;
};

namespace base {
class IAsyncHandler {
	pub virtual void pushAsyncOperation(std::shared_ptr<AsyncFileOperation> asyncOperation) = 0;

	virtual ~IAsyncHandler() = default;
};
};
