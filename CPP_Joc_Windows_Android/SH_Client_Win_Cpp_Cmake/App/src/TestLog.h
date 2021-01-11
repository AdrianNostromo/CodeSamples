#pragma once

#include <base/gh.h>
#include <chrono>
#include <base/list/Array1D.h>

class TestLog {
    pub class A {
        pub bool isLogFrame = false;
        pub int logFrameCounter = 0;

        pub std::chrono::time_point<std::chrono::system_clock> frameStartTimestamp;

        pub enum TimestampSlots{ PhysicsSimStart, PhysicsSimEnd, MainLoopEnd, PreRenderStart, RenderEnd, GLSwapBufferPre, GLSwapBufferPost };
        pub Array1D<std::chrono::time_point<std::chrono::system_clock>> timestampsArray{7};

    };

    pub static A a;
};
