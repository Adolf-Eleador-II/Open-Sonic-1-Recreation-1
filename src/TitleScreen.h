#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "Bg.h"
#include "Camera.h"
#include "core/game_enviroment/InputMgr.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include <memory>

class TitleScreen {
public:
    TitleScreen(artist_api::Artist &artist, resource_store::TextureLoader &loader) : artist_(artist), loader_(loader) {

        bgTexture_ =
            loader_.loadFromFile("content/textures/texTitle.png");

        pos = v2f((float)427 / 2,
                  (float)240 / 2);

        yAnimShift = 8;
    }

    void draw() {

        yAnimShift -= 6;
        if (yAnimShift < -48) {
            yAnimShift = -48;
        }

        artist_api::Sprite sprBackRing = {.texture = *bgTexture_,
                                    .rect = {.width = 256, .height = 82}};
        sprBackRing.setOffsetCenter();

        artist_api::Sprite sprFrontRing = {.texture = *bgTexture_,
                                    .rect = {.y = 83, .width = 256, .height = 82}};
        sprBackRing.setOffsetCenter();


        artist_.drawSprite(sprBackRing, {.x = pos.x, .y = pos.y});
        artist_.drawSprite(sprFrontRing, {.x = pos.x, .y = pos.y});
    }

private:
    artist_api::Artist &artist_;
    resource_store::TextureLoader &loader_;
    Camera cam;

    v2f pos;

    float currentFrame_ = 0;
    float frameSpeed_ = 0.125f;

    std::unique_ptr<artist_api::Texture> bgTexture_;

    float yAnimShift;
};

#endif // TITLESCREEN_H
