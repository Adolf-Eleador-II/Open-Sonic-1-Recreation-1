#include "GameScene.h"

GameScene::GameScene(GameEnvironment &env, DeprecatedGameEnvironment &deprEnv, ResourceStore &store)
    : env_(env), deprEnv_(deprEnv), levelLoader_(deprEnv.scr, env.input(), deprEnv.audio, store) {}
