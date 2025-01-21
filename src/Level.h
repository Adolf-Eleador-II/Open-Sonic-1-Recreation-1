#pragma once

#include <list>
#include <memory>
#include <vector>

#include "core/game_enviroment/artist/ArtistStructs.h"
#include "entities/Entity.h"
#include "entity-creator.hpp"
#include "entity-placement.hpp"
#include "entity-pool.hpp"
#include "game-loop-ticker.h"
#include "terrain-loader.hpp"

#include "entities/_index.hpp"

#include "Camera.h"
#include "Bg.h"
#include "LevelInformer.h"

#include "player-sensor.hpp"

#define TILE_HEIGHTS_BUFF_SIZE 4096
#define TILE_ANGLES_BUFF_SIZE  256

enum class GameType {
    SONIC_1,
    SONIC_CD,
    SONIC_2,
    SONIC_3K,
};

class Level {
public:
    Level(
        terrain::Terrain& terrain,
        std::vector<EntityPlacement>& entities,
        GameType gameType,
        IInputMgr& input, 
        Audio& audio,
        std::string& zoneName,
        std::string& zoneNameShort,
        int act,
        v2f playerStartPosition,
        terrain::Store<terrain::Tile>& storeTile,
        std::unique_ptr<artist_api::Texture> texBlocks,
        ResourceStore &store,
        artist_api::Artist &artist,
        resource_store::TextureLoader &loader
    ) 
        : m_terrain(terrain)
        , m_entityPool(cam)
        , m_input(input)
        , m_audio(audio) 
        , m_gameType(gameType)
        , m_zoneName(zoneName)
        , m_zoneNameShort(zoneNameShort)
        , m_act(act)
        , m_playerStartPosition(playerStartPosition)
        , m_terrainDrawer(cam, m_terrain.getChunkStore(), m_terrain.getLayout(), 255, storeTile, std::move(texBlocks), artist, loader)
        , bg(m_terrainDrawer)
        , store_(store)
        , artist_(artist)
    {
        EntityCreatorSonic1 ec(m_entityPool, m_terrain, store, artist);
        
        for (auto& plc : entities) {
            m_entityPool.create(ec.create(plc));
        }
    }
    void create();
    void free();

    bool isPlayerDied() { return playerDied; }
    bool isEnded() { return end; }

    void update();

    void draw();

    terrain::TerrainDrawer& getTerrainDrawer() { return m_terrainDrawer; } 

private:
    artist_api::Artist &artist_;
    ResourceStore &store_;
    terrain::Terrain &m_terrain;

    EntityPool  m_entityPool;
    GameType    m_gameType;
    std::string m_zoneName;
    std::string m_zoneNameShort;
    int         m_act;

    IInputMgr&  m_input;
    Audio&      m_audio;

    v2f m_playerStartPosition;

    terrain::TerrainDrawer m_terrainDrawer;
    Bg bg;

    Camera cam;

    std::list<Entity*>::iterator it;

    int rings = 0;
    int score = 0;
    int time = 0;
    int tick = 0;
    bool isTimeStopped = false;
    bool isFadeOut = false;
    bool isFadeDeath = false;

    bool playerDied = false;

    float ringFrame;

    bool end;

    uint8_t fade;

private:
    void drawHud();
    void createZoneSpecific();
    void updateLevelSpecific();
    
    void createSonic1LayeringObjects();

};