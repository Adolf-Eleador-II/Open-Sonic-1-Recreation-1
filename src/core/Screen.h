#pragma once

#include <cstdint>
#include <cstring>
#include <map>

#include "Geometry.h"
#include "core/game_enviroment/artist/Artist.h"
#include "core/game_enviroment/artist/TextureLoader.h"
#include <SFML/Graphics.hpp>
#include <vector>

#include "core/game_enviroment/ResourceStore.h"
#include "sfml_game_environment/SfmlArtist.h"

class Screen {
public:
    explicit Screen(SfmlArtist &artist, ResourceStore &store,
                    resource_store::TextureLoader &loader)
        : artist_(artist), store_(store), loader_(loader) {}

    ResourceStore &store() { return store_; }
    artist_api::Artist &artist() { return artist_; }
    resource_store::TextureLoader &textureLoader() { return loader_; }

private:
    SfmlArtist &artist_;
    resource_store::TextureLoader &loader_;
    ResourceStore &store_;
};
