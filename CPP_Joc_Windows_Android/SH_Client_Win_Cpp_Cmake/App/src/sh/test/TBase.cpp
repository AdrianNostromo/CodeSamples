#include "TBase.h"
#include <iostream>
#include <vendor/stb/stb_image.h>
#include <vendor/stb/stb_image_write.h>
#include <vendor/stb/stb_truetype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <base/exceptions/LogicException.h>
#include <sh/app/IAppSquareHeads.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>

using namespace sh;

TBase::TBase(IAppSquareHeads* app)
    : super(), app(app)
{
    //void
}

void TBase::create() {
    super::create();

    app->getLoopDistribution()->addEventListener(
        base::ILoopDistribution::AppLoopEvent_inputDispatch->type,
        std::bind(&TBase::onLoopEvent_appLoop_general, this, std::placeholders::_1, std::placeholders::_2),
        localAutoListenersList
    );

//    tst1();
//    tst2();
//    tst3();
}

void TBase::tst1() {
    //int rr = 0;
    int width;
    int height;
    int bytesPerPixel_NOT_BITS;
    //int w2;
    //int h2;
    std::string path = "assets/texture/test.png";
    std::string path_out = "assets/texture/test_2.png";

    stbi_set_flip_vertically_on_load(1);

    unsigned char *data;
    data = stbi_load(path.c_str(), &width, &height, &bytesPerPixel_NOT_BITS, 4);
    if (data) {
        std::cout << "A 1.\n";
    } else {
        std::cout << "Err 1.\n";
    }

    stbi_write_png(path_out.c_str(), width, height, 4, data, width * 4);

    free(data);
}

void TBase::tst2() {
    std::string path = "assets/fonts/Roboto-Regular.ttf";

    const int BITMAP_W = 1024;
    const int BITMAP_H = 1024;

    unsigned char* temp_bitmap = new unsigned char[BITMAP_H * BITMAP_W];
    stbtt_bakedchar* cdata = new stbtt_bakedchar[256*2]; // ASCII 32..126 is 95 glyphs
    //stbtt_packedchar* pdata = new stbtt_packedchar[256*2];

    unsigned char* ttf_buffer = new unsigned char[1 << 25];
    //unsigned char* output = new unsigned char[512*100];

    FILE* fp = fopen(path.c_str(), "rb");
    fread(ttf_buffer, 1, 1<<25, fp);

    int r = stbtt_BakeFontBitmap(
        ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0),
        40.0,
        temp_bitmap, BITMAP_W, BITMAP_H,
        32, 96, cdata
    ); // no guarantee this fits!

    std::cout << "B: " << r << "\n";

    stbi_write_png("assets/texture/test_3.png", BITMAP_W, BITMAP_H, 1, temp_bitmap, 0);
}

void TBase::tst3() {
    //void
}

void TBase::onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) {
    //void
}

TBase::~TBase() {
    //void
}
