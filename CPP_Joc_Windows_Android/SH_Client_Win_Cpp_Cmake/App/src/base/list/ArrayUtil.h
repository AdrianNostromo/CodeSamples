#pragma once

#include <memory>

namespace base {
class ArrayUtil {
public:
	template <typename T>
	static std::shared_ptr<Array1D<T>> concatenateArrays_sp(
		std::shared_ptr<Array1D<T>>& a1,
		std::shared_ptr<Array1D<T>>& a2,
		std::shared_ptr<Array1D<T>>& a3,
		std::shared_ptr<Array1D<T>>& a4,
		std::shared_ptr<Array1D<T>>& a5,
		std::shared_ptr<Array1D<T>>& a6);

};

template <typename T>
std::shared_ptr<Array1D<T>> ArrayUtil::concatenateArrays_sp(
	std::shared_ptr<Array1D<T>>& a1,
	std::shared_ptr<Array1D<T>>& a2,
	std::shared_ptr<Array1D<T>>& a3,
	std::shared_ptr<Array1D<T>>& a4,
	std::shared_ptr<Array1D<T>>& a5,
	std::shared_ptr<Array1D<T>>& a6)
{
	if (a1 == nullptr || a2 == nullptr || a3 == nullptr || a4 == nullptr || a5 == nullptr || a6 == nullptr) {
		throw LogicException(LOC);
	}

	int totalCt = a1->getCount() + a2->getCount() + a3->getCount() + a4->getCount() + a5->getCount() + a6->getCount();
	
	std::shared_ptr<Array1D<T>> ret = std::make_shared< Array1D<T>>(totalCt);
	
	const int listsCount = 6;
	std::shared_ptr<Array1D<T>> lists[listsCount]{
		a1,
		a2,
		a3,
		a4,
		a5,
		a6
	};
	int nextIndex = 0;
	for (int i = 0; i < listsCount; i++) {
		std::shared_ptr<Array1D<T>> list = lists[i];

		ret->set(nextIndex, *list, 0, list->getCount());
		nextIndex += list->getCount();
	}

	return ret;
}

};
