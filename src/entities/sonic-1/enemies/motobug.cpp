#include "EnemiesGHZ.h"

void EnMotobug::init() {
    dv_hitBoxSize = v2f(42, 34);
    dv_type = TYPE_ENEMY;
    animator_.setSpeed(0.2f);
}

void EnMotobug::d_update() {
    dv_pos.x += MOTOBUG_SPD * dir;
    animator_.tick();
    trnCollision();
}

void EnMotobug::d_draw(Camera &cam) {
    bool flip = false;
    if (dir == 1)
        flip = true;
    auto &spr = animator_.getCurrentFrame();
    artist_.drawSprite(spr, {.x = dv_pos.x - cam.getPos().x,
                             .y = dv_pos.y - cam.getPos().y}, {.flipHorizontal=flip});///
    m_sensor.draw(cam);
}

void EnMotobug::trnCollision() {
    m_sensor.setPosition(v2f(dv_pos.x, dv_pos.y + 14));

    auto distToGround = m_sensor.getDistance();
    bool colliding = (distToGround >= -8) && (distToGround <= 12);

    if (colliding) {
        dv_pos.y += distToGround;
    } else {
        dir = -dir;
    }
}