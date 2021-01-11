#pragma once

#include <graphics/visual2d/drawable/IDynamicPropertiesObject2D.h>
#include <functional>

namespace base {
class ISlider2D : virtual public IDynamicPropertiesObject2D {
public:
	class IListener {
	public:
		virtual void onValueChanged(ISlider2D* target) = 0;

		virtual ~IListener() = default;
	};
	class CbListener : virtual public IListener {
	public:
		std::function<void(
			ISlider2D* target)> cb_onValueChanged;
	public:
		CbListener(CbListener const&) = delete;
		CbListener(CbListener&&) = default;
		CbListener& operator=(CbListener const&) = delete;
		CbListener& operator=(CbListener&&) = default;

		explicit CbListener();

		void onValueChanged(ISlider2D* target) override;

		~CbListener() override;
	};

public:
	virtual void setEventListener(IListener* eventListener) = 0;
	
	virtual float getRangeValue() = 0;
	virtual void setRangeValue(float rangeValue, bool doAnimation, bool dispatchEvent) = 0;

	virtual ~ISlider2D() = default;
};
};
