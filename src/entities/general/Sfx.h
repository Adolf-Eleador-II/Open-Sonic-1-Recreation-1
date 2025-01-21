#pragma once
#include "../Entity.h"
#include "core/game_enviroment/artist/Animator.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "entity-pool.hpp"

class SingleAnimationEffect : public Entity {
public:
    SingleAnimationEffect(v2f pos, artist_api::Artist &artist, artist_api::Animation &anim, EntityPool& pool) : m_pos(pos), animator_(anim), m_pool(pool), artist_(artist) {}
    void init(){
        animator_.setSpeed(0.01f);
    }

    void update() override {
        animator_.tick();
        liveTimer++;
        if (liveTimer >= 8) {
            m_pool.destroy(*this);
        }
    }

    void draw(Camera &cam) override {
        auto &spr = animator_.getCurrentFrame();
        artist_.drawSprite(spr, {.x = m_pos.x - cam.getPos().x,
                                 .y = m_pos.y - cam.getPos().y});
    }

    EntityTypeID type() override { return EntityTypeID::PARTICLE; }

private:
    artist_api::Artist &artist_;
    v2f m_pos;
    int liveTimer = 0;
    artist_api::Animator animator_;
    EntityPool& m_pool;
};
