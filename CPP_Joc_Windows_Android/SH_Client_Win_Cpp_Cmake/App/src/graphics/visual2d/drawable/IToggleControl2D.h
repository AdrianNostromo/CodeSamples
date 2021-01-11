#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/IDynamicPropertiesObject2D.h>
#include <functional>

namespace base {
class IToggleControl2D : virtual public IDynamicPropertiesObject2D {
public:
	class IListener {
	public:
		virtual void onSelectedStateChanged(IToggleControl2D* target) = 0;

		virtual ~IListener() = default;
	};
	class CbListener : virtual public IListener {
	public:
		std::function<void(
			IToggleControl2D* target)> cb_onSelectedStateChanged;
	public:
		CbListener(CbListener const&) = delete;
		CbListener(CbListener&&) = default;
		CbListener& operator=(CbListener const&) = delete;
		CbListener& operator=(CbListener&&) = default;

		explicit CbListener();

		void onSelectedStateChanged(IToggleControl2D* target) override;

		~CbListener() override;
	};

public:
	pub virtual void setEventListener(IListener* eventListener) = 0;

	pub virtual bool getIsOn() = 0;
	pub virtual void setIsOn(bool isOn, bool doUpdate, bool doAnimation, bool playSound, bool dispatchEvent) = 0;
	pub virtual void setIsEnabled(bool isEnabled, bool doUpdate, bool doAnimation) = 0;

	pub virtual ~IToggleControl2D() = default;
};
};
