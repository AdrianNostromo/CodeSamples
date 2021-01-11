#pragma once

#include <base/gh.h>
#include <string>

namespace base {
class ETemplateIdentifier {pub dCtor(ETemplateIdentifier);
	std::string nSpace;
	std::string id;

	pub explicit ETemplateIdentifier()
		: nSpace(""), id("")
	{}

	pub void set(std::string& nSpace, std::string& id) {
		this->nSpace = nSpace;
		this->id = id;
	}
};
};
