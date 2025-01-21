#pragma once

#include <filesystem>

#include "Level.h"

#include "entity-placement.hpp"
#include "entity-pool.hpp"
#include "terrain-loader.hpp"

#include "zone-enums-sonic1.hpp"
#include "zone-enums-sonic2.hpp"
#include "zone-enums-sonic3.hpp"

class LevelLoader {
public:
    LevelLoader(
        IInputMgr& input,
        Audio& audio,
        ResourceStore &store,
        artist_api::Artist &artist,
        resource_store::TextureLoader &loader
    )
        : m_audio(audio)
        , m_input(input)
        , store_(store)
        , artist_(artist)
        , loader_(loader)
    {}

    Level* loadFromSonic1(ZoneSonic1 zone, int act);
    Level* loadFromSonic2(ZoneSonic2 zone, int act);
    Level* loadFromSonic3K(ZoneSonic3K zone, int act);

private:
    IInputMgr&  m_input;
    Audio&      m_audio;

    terrain::Terrain*                               m_terrain       = nullptr;
    std::unique_ptr<terrain::Store<terrain::Tile>>  m_storeTiles;
    std::unique_ptr<terrain::Store<terrain::Block>> m_storeBlocks;
    std::unique_ptr<terrain::Store<terrain::Chunk>> m_storeChunks;
    std::unique_ptr<terrain::Layout>                m_layout;
    
    v2f m_playerStartPosition;

    std::vector<EntityPlacement> m_entityPlacementList;
private:
    artist_api::Artist &artist_;
    ResourceStore &store_;
    resource_store::TextureLoader &loader_;
    void reset();

    void sonic1LoadTerrain(std::string& sZone, std::string& sZoneAct);
    void sonic1LoadStartPosition(std::string& sZoneAct);
    void sonic1LoadObjects(std::string& sZoneAct);

};