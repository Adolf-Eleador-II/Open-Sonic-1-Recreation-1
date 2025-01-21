#pragma once

#include "Camera.h"
#include "core/Audio.h"
#include "core/Geometry.h"
#include "core/game_enviroment/ResourceStore.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "core/game_enviroment/artist/SpriteFont.h"
#include "entities/Entity.h"
#include "entity-pool.hpp"
#include "sonic/SonicResources.h"
#include <format>

class TitleCardSonic1 : public Entity {
public:
    TitleCardSonic1(const std::string &zonename, int act, float screenWidth,
                    EntityPool &entityPool, ResourceStore &store, artist_api::Artist &artist)
        : m_act(act), m_zonename(zonename), m_entityPool(entityPool), store_(store), artist_(artist) {
        m_textActX = screenWidth;
        m_shapeX = screenWidth;
        m_textTopX = 0;
        m_textBottomX = 0;
    };

    EntityTypeID type() override { return EntityTypeID::LEVEL_INFORMER; }

    void draw(Camera &cam) override {
        if (m_tick < 200) {
            if (m_tick > 0) {
                m_textTopX += 8;
                m_shapeX -= 8;
                if (m_tick > 30)
                    m_textBottomX += 8;
                if (m_tick > 50)
                    m_textActX -= 8;
            }
        } else {
            m_textTopX -= 16;
            m_textBottomX -= 16;
            m_shapeX += 16;
            m_textActX += 16;
        }

        float centerX = 427 / 2;

        m_tick++;

        if (m_shapeX < centerX + 25 )
            m_shapeX = centerX + 25;
        if (m_textActX < centerX + 32)
            m_textActX = centerX + 32;

        if (m_textTopX > centerX + 75)
            m_textTopX = centerX + 75;
        if (m_textBottomX > centerX + 58)
            m_textBottomX = centerX + 58;


        auto& sprAct = store_.get<artist_api::Animation>(
            store_.map<SonicResources>().animations.act.act
        ).frames[0];
        auto& sprActBack = store_.get<artist_api::Animation>(
            store_.map<SonicResources>().animations.act.ellipse
        ).frames[0];

        artist_.drawSprite(
            sprActBack,
            artist_api::Vector2D<float>{.x = m_shapeX, .y = 69});
        artist_.drawSprite(
            sprAct,
            artist_api::Vector2D<float>{.x = m_textActX, .y = 110});

        v2f _v2f=v2f(m_textActX + 29, 94);

        auto& actResources = store_.map<SonicResources>().animations.act;
        auto actDigitResource = actResources.d1;
        switch (m_act) {
            case 1:
                _v2f.x+=3;
                actDigitResource = actResources.d1;
                break;
            case 2:
                actDigitResource = actResources.d2;
                break;
            case 3:
                actDigitResource = actResources.d3;
                break;
        }

        auto& sprActDigit = store_.get<artist_api::Animation>(actDigitResource).frames[0];

        artist_.drawSprite(
            sprActDigit,
            artist_api::Vector2D<float>{.x = _v2f.x, .y = _v2f.y});


        auto &font = store_.get<artist_api::SpriteFont>(
            store_.map<SonicResources>().fonts.s1TitleCard);

        artist_.drawText(
            m_zonename, {.x = m_textTopX, .y = 72}, font,
            {.horizontalAlign = artist_api::HorizontalAlign::RIGHT});
        artist_.drawText(
            "ZONE", {.x = m_textBottomX, .y = 92}, font,
            {.horizontalAlign = artist_api::HorizontalAlign::RIGHT});

        if (m_tick > 220) {
            m_entityPool.destroy(*this);
        }
    }

private:
    artist_api::Artist &artist_;
    ResourceStore &store_;
    EntityPool &m_entityPool;
    const std::string &m_zonename;
    int m_act;
    int m_tick = 0;

    float m_textTopX;
    float m_textBottomX;
    float m_textActX;
    float m_shapeX;
};
