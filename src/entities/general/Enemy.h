#pragma once

#include "../Entity.h"
#include "core/game_enviroment/artist/Animator.h"
#include "core/game_enviroment/artist/ArtistStructs.h"

class Enemy : public Entity
{
    public:
        Enemy(v2f _pos) : Entity(_pos){}
        void setAbilityToDying(bool _canDie) { canDie = _canDie; };
        void onOutOfView() override {
            dv_pos = dv_startPos;
        }
    private:
        bool canDie = true;
};

class EnemyScore : public Entity {
    public:
        enum Points { P_100, P_200, P_500, P_1000 };
        EnemyScore(v2f _pos, artist_api::Artist &artist, artist_api::Animation &anim, Points points) : //
            Entity(_pos), artist_(artist), animator_(anim), points(points) { init(); }//

        void init() {
            dv_pos.y -= 3;
            tick = 0;
            // dv_anim.create(TEX_OBJECTS);
            // dv_anim.set(70+points, 70+points, 0);
            animator_.setSpeed(0.0f);
        }

        void draw(Camera &cam){
            auto &spr = animator_.getCurrentFrame();
            artist_.drawSprite(spr, {.x = dv_pos.x - cam.getPos().x,
                                                   .y = dv_pos.y - cam.getPos().y});
        }

        void d_update() {
            if (tick < 30)  
                dv_pos.y -= 2;
            if (tick > 40)
                dv_living = false;

            tick++;
        }

        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

    private:
        artist_api::Artist &artist_;
        artist_api::Animator animator_;
        Points points;
        int tick;
};