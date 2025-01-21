#include "GameScene.h"

GameScene::GameScene(GameEnvironment &env, DeprecatedGameEnvironment &deprEnv, ResourceStore &store, artist_api::Artist &artist, resource_store::TextureLoader &loader)
    : env_(env), deprEnv_(deprEnv), levelLoader_(deprEnv.scr, env.input(), deprEnv.audio, store, artist, loader), artist_(artist), loader_(loader) {}
