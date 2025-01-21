#pragma once

#include "core/game_enviroment/artist/Animator.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "entities/Entity.h"

struct MonitorAnimations {
  artist_api::Animation &animMain;
  artist_api::Animation &animBroken;
  artist_api::Animation &animNoise;
  artist_api::Animation &animIcon;
};

class Monitor : public Entity {
  public:
    enum Item { M_RINGS, M_INVINCIBILITY, M_LIVE, M_SHIELD, M_SPEED };

    Monitor(v2f _pos, artist_api::Artist &artist, MonitorAnimations &anims, Item item) : Entity(_pos), animator_(anims.animMain), animatorScreen_(anims.animIcon),
                                                  anims_(anims), item(item), artist_(artist) {}
    void init() {
        dv_solid = true;
        dv_platform = true;
        dv_type = TYPE_MONITOR;
        dv_hitBoxSize = v2f(32, 32);
        animator_.setSpeed(0.0f);
        animatorScreen_.setSpeed(0.1f);
        animatorScreen_.changeTo(anims_.animNoise);
    }
    void d_update(){
      liveTime++;
      if(liveTime%120==60){
        animatorScreen_.changeTo(anims_.animIcon);
      }
      if(liveTime%120==0){
        animatorScreen_.changeTo(anims_.animNoise);
      }
      animator_.tick();
      animatorScreen_.tick();
    }

    void draw(Camera &cam) {
        auto &spr = animator_.getCurrentFrame();
        artist_.drawSprite(spr, {.x = dv_pos.x - cam.getPos().x,
                   .y = dv_pos.y - cam.getPos().y});
        auto &sprIcon = animatorScreen_.getCurrentFrame();
        artist_.drawSprite(sprIcon, {.x = dv_pos.x - cam.getPos().x,
                       .y = dv_pos.y - 3 - cam.getPos().y});
    }

    Item getItem() { return item; }
    artist_api::Animation& getAnimationIcon() { return anims_.animIcon; }
    artist_api::Animation& getAnimationBroken() { return anims_.animBroken; }

    EntityTypeID type() override { return EntityTypeID::MONITOR; }

  private:
    artist_api::Artist &artist_;
    MonitorAnimations anims_;
    artist_api::Animator animator_;
    artist_api::Animator animatorScreen_;
    
    int liveTime=0;
    Item item;
};

class BrokenMonitor : public Entity {
  public:
    BrokenMonitor(v2f _pos, artist_api::Artist &artist, artist_api::Animation &anim) : Entity(_pos), animator_(anim), artist_(artist) {
        animator_.setSpeed(0.f);
    }
    void draw(Camera &cam) {
        auto &spr = animator_.getCurrentFrame();
        artist_.drawSprite(spr, {.x = dv_pos.x - cam.getPos().x,
                                 .y = dv_pos.y - cam.getPos().y});
    }
    EntityTypeID type() override { return EntityTypeID::DEPRECATED; }
  private:
    artist_api::Artist &artist_;
    artist_api::Animator animator_;
};

class MonitorIcon : public Entity {
  public:
    MonitorIcon(v2f _pos, artist_api::Artist &artist, artist_api::Animation &anim, Monitor::Item item) : Entity(_pos), animator_(anim), item(item), artist_(artist) {
        init();
    }

    void init() {
        dv_pos.y -= 3;
        tick = 0;
        animator_.setSpeed(0);
    }
    void draw(Camera &cam) {
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
    Monitor::Item item;
    int tick;
};