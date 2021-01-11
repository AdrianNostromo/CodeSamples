#pragma once

#include <graphics/visual2d/drawable/Container2D.h>
#include <graphics/camera/Camera.h>
#include <graphics/visual2d/drawable/IRoot2D.h>
#include <graphics/visual/drawable/IStage.h>

namespace graphics {
	class IEnvironment2D;
};

class BasicStage2D : public Container2D, virtual public IRoot2D, virtual public IStage {
private: typedef Container2D super;
protected:
	graphics::IEnvironment2D* environment;
    
public:
    BasicStage2D(BasicStage2D const&) = delete;
    BasicStage2D(BasicStage2D &&) = default;
    BasicStage2D& operator=(BasicStage2D const&) = delete;
    BasicStage2D& operator=(BasicStage2D &&) = default;

    explicit BasicStage2D(graphics::IEnvironment2D* environment);

    virtual ~BasicStage2D();
};
