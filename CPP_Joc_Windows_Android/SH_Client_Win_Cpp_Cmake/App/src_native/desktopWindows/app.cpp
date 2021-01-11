#include <iostream>
#include <string>
#include <desktopWindows/launcher/NativeLauncher.h>
#include <base/exceptions/LoggableException.h>
#include <desktopWindows/crashHandler/CrashHandlerNative.h>

using namespace desktopWindows;

NativeLauncher *launcher;

void uncaughtExceptionsHandler() {
    base::ICrashHandler::crashHandler->pushEntry_CriticalException(
        LOC, "uncaughtExceptionsHandler"
    );

    base::ICrashHandler::crashHandler->handleCrash();

    abort();
}

//asd_x;_r;
//class Foo : /*public virtual base::IDisposable, */public virtual mem::ISelfSharedPointerContainer {
//public:
//    pub bool getIsDisposed() final { return true; };
//
//    // Note. call only one.
//    pub void reservedDisposeIfNeeded() final {};
//    pub void reservedDispose() final {
//        int asd_01 = 0;
//
//    };
//    pub void reservedDisposeMain() final {};

    //priv spV2<Foo> selfWSP{ true/*isWeakPointer*/ };
    //pub spV2<Foo>& getSelfWSPRef() { return selfWSP; }

//    int a = 111;
//    int getY()
//    {
//        return 0;
//    }
//    virtual ~Foo() = default;
//};
//
//class Foo2 : public Foo {
//public:
//    int b = 222;
//    virtual int getX()
//    {
//        return 0;
//    }
//    ~Foo2() override = default;
//};
//
//class Bar {
//
//};
//
//void func(spV2<int> v1) {
//
//}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    //spV2<BinPackingBinaryTree> customGlyphsPackingTree = new BinPackingBinaryTree{2, 2};
    //{
    //    spV2<Foo> p1 = mspV2<Foo>();
    //    p1 = nullptr;
    //    int asd = 0;
    //}
    //{
    //    spV2<Foo> p1 = mspV2<Foo>();
    //    p1 = nullptr;
    //    int asd = 0;
    //}
    //{
    //    Foo2* f2p = new Foo2();
    //    Foo* f1p = f2p;

    //    //void* f2p_vp = f2p;
    //    //void* f1p_vp = f1p;

    //    //char* f2p_cp = static_cast<char*>(f2p_vp);
    //    //char* f1p_cp = static_cast<char*>(f1p_vp);

    //    std::cout << "The address of pA is: " << f2p << std::endl;
    //    std::cout << "The address of pB is: " << f1p << std::endl;
    //}

    //{
    //    // Test 1;// Normal ussage, should work.
    //    spV2<Foo2> p1 = mspV2<Foo2>();
    //    spV2<Foo> p1b = p1;
    //    spV2<Foo2> p2 = p1b.dCast<Foo2>(false);
    //    int asd_01 = 0;
    //}
    //{
    //    // Test 2;// THe dynamic_cast must return nullptr in this case;
    //    spV2<Foo> p1b = mspV2<Foo>();
    //    spV2<Foo2> p2 = p1b.dCast<Foo2>(false);
    //    int asd_01 = 0;
    //}
    //spV2<Foo> p1b2 = p1b;
    //p1 = p1b;
    //p1b = p1;
    //spV2<Foo> p2 = nullptr;
    //func(nullptr);
    ////spV2<Foo2> p3 = spV2<Foo2>(std::move(p1));
    //spV2<Foo2> p3 = p1;
    //spV2<Foo> p2 = p1;

    std::set_terminate(uncaughtExceptionsHandler);

    CrashHandlerNative* crashHandlerNative = new CrashHandlerNative();
    base::ICrashHandler::crashHandler = crashHandlerNative;
    crashHandlerNative->reservedCreate();

    launcher = new NativeLauncher();
    int ret = launcher->run();
    launcher->reservedDisposeMain();
    delete launcher;

    return ret;
}
