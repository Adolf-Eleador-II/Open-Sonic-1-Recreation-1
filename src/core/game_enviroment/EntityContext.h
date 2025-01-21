#ifndef OS1R_ENTITYCONTEXT_H
#define OS1R_ENTITYCONTEXT_H

#include "core/Screen.h"
#include "core/game_enviroment/InputMgr.h"
#include "core/game_enviroment/artist/Artist.h"
#include "core/game_enviroment/ISceneDirector.h"

namespace entity_v3 {
    

class InitContext {};

struct DrawContext final {
    artist_api::Artist &artist;
};

struct UpdateContext final {
    IInputMgr& input;
    ISceneDirector& sceneDirector;
};

struct Contexts {
    InitContext& init;
    DrawContext& draw;
    UpdateContext& update;
};

} // namespace entity_v3

#endif // OS1R_ENTITYCONTEXT_H
