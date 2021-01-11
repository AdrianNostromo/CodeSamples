#pragma once

#include <base/map/Map1D.h>
#include <string>
#include <graphics/visual/drawable/IGenericDrawable.h>
#include <graphics/visual2d/drawable/IDynamicPropertiesObject2D.h>

class PseudoVisualData : virtual public IDynamicPropertiesObject2D, virtual public IGenericDrawable {
private:
    Map1D<std::string, std::shared_ptr<base::IWrappedValue>> propertiesMap{};

    // Pseudo bool object.
    // -1 : not init.
    // 0 : false
    // 1 : true
    Map1D<std::string, int> staticAnimatingPropertiesMap{};

public:
    PseudoVisualData(PseudoVisualData const&) = delete;
    PseudoVisualData(PseudoVisualData &&) = default;
    PseudoVisualData& operator=(PseudoVisualData const&) = delete;
    PseudoVisualData& operator=(PseudoVisualData &&) = default;

    explicit PseudoVisualData();

    std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
    void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

    bool isInit_StaticAnimatingPropertyStatus(std::string& property) override;
    bool isTrue_StaticAnimatingPropertyStatus(std::string& property) override;
    void putStaticAnimatingPropertyStatus(std::string& property, int status) override;

    ~PseudoVisualData() override;
};
