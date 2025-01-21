#include "GameScene.h"

GameScene::GameScene(GameEnvironment &env, DeprecatedGameEnvironment &deprEnv, ResourceStore &store, artist_api::Artist &artist)
    : env_(env), deprEnv_(deprEnv), levelLoader_(deprEnv.scr, env.input(), deprEnv.audio, store, artist), artist_(artist) {}
