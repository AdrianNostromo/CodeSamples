#pragma once

#include "GPMCamera.h"

namespace base {
class GPMLayers2D : public GPMCamera {
private: typedef GPMCamera super;
public:
	IContainer2D* layerTouchAreaUnder = nullptr;
	IContainer2D* layerSubUserInterface = nullptr;
	IContainer2D* layerMainUserInterface = nullptr;
	IContainer2D* layerTouchAreaOver = nullptr;

public:
    GPMLayers2D(GPMLayers2D const&) = delete;
    GPMLayers2D(GPMLayers2D &&) = default;
    GPMLayers2D& operator=(GPMLayers2D const&) = delete;
    GPMLayers2D& operator=(GPMLayers2D &&) = default;

    explicit GPMLayers2D(base::IGameWorld* gw);

    void createLayers() override;

    IContainer2D* getLayerSubUserInterface() override { return layerSubUserInterface; };
    IContainer2D* getLayerMainUserInterface() override { return layerMainUserInterface; };

    ~GPMLayers2D() override;

};
};
