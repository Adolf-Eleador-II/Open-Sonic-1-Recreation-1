#pragma once

#include "../core/_index.hpp"

#include "Camera.h"

#include "Entity.h"

#include "core/game_enviroment/artist/Animator.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "entities/Entity.h"
#include "entity-pool.hpp"
#include "new-terrain.hpp"
#include "player-collider.hpp"
#include "player-state-base.hpp"
#include "player-state-normal.hpp"
#include "terrain-sensor.hpp"

#include "general/_index.hpp"
#include "sonic-1/_index.hpp"

#include "../player-collider.hpp"
#include "../player-state-base.hpp"

// === Constants === //

// = physics =
// normal
#define PL_ACC 0.046875
#define PL_DEC 0.5
#define PL_FRC 0.046875
#define PL_TOP 6
#define PL_JMP 6.5
#define PL_GRAV 0.21875
#define PL_AIR 0.09375

#define PL_SLP 0.125
#define PL_SLP_ROLL_UP 0.078125
#define PL_SLP_ROLL_DOWN 0.3125

#define PL_FRC_ROLL 0.0234375
#define PL_DEC_ROLL 0.125

#define PL_FOOT_LEVEL 20

using namespace gmath;

struct PlayerAnimations {
    artist_api::Animation &idle;
    artist_api::Animation &boredStart;
    artist_api::Animation &bored;

    artist_api::Animation &walk;
    // artist_api::Animation &walkDiagonal;
    artist_api::Animation &run;
    // artist_api::Animation &runDiagonal;
    artist_api::Animation &dash;
    // artist_api::Animation &dashDiagonal;

    artist_api::Animation &sit;

    artist_api::Animation &roll;
    artist_api::Animation &skidStart;
    artist_api::Animation &skid;
    artist_api::Animation &hurt;
    artist_api::Animation &die;
    artist_api::Animation &push;
    artist_api::Animation &lookUp;

    artist_api::Animation &spring;
    artist_api::Animation &fall;

    

    artist_api::Animation &n10;
    artist_api::Animation &n100;
    artist_api::Animation &n200;
    artist_api::Animation &n500;
    artist_api::Animation &n1_000;
    artist_api::Animation &n10_000;
};

class Player : public Entity {
public:
    Player(v2f _pos, PlayerAnimations &anims, std::list<Entity *> &entities, EntityPool &entityPool,
           Camera &cam, terrain::Terrain &_trn, IInputMgr &input, Audio &audio,
           int &rings, int &score)
        : Entity(_pos), anims_(anims), animator_(anims.die), m_entityPool(entityPool), input(input), audio(audio),
          rings(rings), score(score), m_collider(dv_pos, spd, gsp, _trn),
          m_stateMachine(m_props), cam(cam), entities(entities),

          sndSpring_(audio.store().get<dj::Sound>(
              audio.store().map<SonicResources>().sounds.spring)),
          sndDestroy_(audio.store().get<dj::Sound>(
              audio.store().map<SonicResources>().sounds.breaking)),
          sndRing_(audio.store().get<dj::Sound>(
              audio.store().map<SonicResources>().sounds.ring)),
          sndShield_(audio.store().get<dj::Sound>(
              audio.store().map<SonicResources>().sounds.player.shield)),
          sndRoll_(audio.store().get<dj::Sound>(
              audio.store().map<SonicResources>().sounds.player.roll)),
          sndSkid_(audio.store().get<dj::Sound>(
              audio.store().map<SonicResources>().sounds.player.skid)),
          sndJump_(audio.store().get<dj::Sound>(
              audio.store().map<SonicResources>().sounds.player.jump)),

          sndRingLoss_(audio.store().get<dj::Sound>(
              audio.store().map<SonicResources>().sounds.ringLoss))

              {};

    ENTITY_EXPOSE_HITBOX(m_hitbox)

    EntityTypeID type() override { return EntityTypeID::PLAYER; }

    void onHitboxCollision(Entity &entity) override;

    void tuduring() { printf("Tuduringl\n"); }
    void init() override;
    void d_update() override;
    void d_draw(Camera &cam) override;
    bool isEndLv() { return endLv; }
    bool isDied() { return dead; }

private:
    dj::Sound &sndSpring_;
    dj::Sound &sndDestroy_;
    dj::Sound &sndRing_;
    dj::Sound &sndShield_;
    dj::Sound &sndRoll_;
    dj::Sound &sndRingLoss_;
    dj::Sound &sndSkid_;
    dj::Sound &sndJump_;

private:
    PlayerAnimations anims_;
    artist_api::Animator animator_;
    EntityHitBox m_hitbox = EntityHitBox(dv_pos, v2i(8, 16));

    void moveCam(Camera &cam);
    void terrainCollision(Camera &cam);
    void entitiesCollision(std::list<Entity *> &entities, Camera &cam);

    PlayerCollider m_collider;
    PlayerStateMachine m_stateMachine;

    std::list<Entity *> &entities;
    EntityPool &m_entityPool;

    Camera &cam;
    IInputMgr &input;
    Audio &audio;

    v2f spd = v2f(0.f, 0.f);

    int &rings;
    int &score;

    // enums
    enum FlrMode { FLOOR, RIGHT_WALL, BOTTOM, LEFT_WALL };

    // controlls
    bool canHorMove = true;

    bool debug = false;

    // movement
    float gsp = 0.0;
    float angle = 0;
    float shiftX = 0; ////
    float shiftY = 0; ////
    int enemyCombo = 0;

    Entity *standOn;

    // gameplay
    bool isSpindashDirRight = false;
    bool standOnObj = false;
    bool sTube = false;
    bool endLv = false;
    bool dead = false;

    // timers
    int ringTimer = 0;
    int horizontalLockTimer = 0;
    int camLagTimer = 0;
    int invicTimer = 0;

    // animation
    bool animFlip = false;
    float anim8Angle = 0.0;
    float animAngle = 0.0;
    bool diaAnim = false;

    int layer = 0;

    // debug
    irect lSenRect, rSenRect;
    bool isDebugPressed = false;

    PlayerStateProps m_props{dv_pos, spd,     gsp,        input,
                             audio, m_collider, m_stateMachine};

    // === functions === //
    void gameplay();
    void movement();
    void animation();
    void getHit(std::list<Entity *> &entities);
};