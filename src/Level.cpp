#include "Level.h"

#include "LevelInformer.h"
#include "core/game_enviroment/ResourceStore.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "core/game_enviroment/artist/SpriteFont.h"
#include "entity-creator.hpp"
#include "game-loop-ticker.h"
#include "sonic/SonicResources.h"
#include <format>
#include <cstdio>

void Level::create() {
    if (m_gameType == GameType::SONIC_1) {
        createSonic1LayeringObjects();
    }

    m_entityPool.create(new TitleCardSonic1(
        m_zoneName, m_act, 427, m_entityPool, store_, artist_));

    if (m_gameType == GameType::SONIC_1) {
        createZoneSpecific();
    }

    // Create player
    PlayerAnimations animsSonic = {
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.idle),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.boredStart),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.bored),

        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.walk),

        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.run),

        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.dash),

        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.sit),

        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.roll),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.skidStart),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.skid),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.hurt),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.die),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.push),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.lookUp),

        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.spring),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.sonic.fall),

        
        
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.killScore.n10),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.killScore.n100),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.killScore.n200),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.killScore.n500),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.killScore.n1_000),
        store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.killScore.n10_000),
    };

    // auto &animStore = st.map<SonicResources>().animations.sonic;
    m_entityPool.create(new Player(
        m_playerStartPosition, artist_, animsSonic, m_entityPool.legacy_rawPool(), m_entityPool, cam,
        m_terrain, m_input, m_audio, rings, score, store_));

    // Create camera
    auto screenSize = Size(427, 240);
    auto cameraPos =
        v2f(m_playerStartPosition.x - (float)screenSize.width / 2,
            m_playerStartPosition.y - (float)screenSize.height / 2);
    auto levelSize = m_terrain.getLayout().getSizeInPixels();

    cam.create(cameraPos, Size(levelSize.x, levelSize.y),
               (m_gameType == GameType::SONIC_3K));

    m_entityPool.init();

    // Level Variables
    ringFrame = 80.0; // Using for same animation to all rings

    tick = 0;
    rings = 0;

    end = false;
    isTimeStopped = false;
    isFadeOut = false;
    isFadeDeath = false;
    fade = 0;

    GameLoopTicker::instance().reset();

    m_terrainDrawer.allowDrawingCollisions(true);
}

auto fromMDColor(uint16_t mdcolor) {
    uint8_t r = (mdcolor >> 0) & 0b1110;
    uint8_t g = (mdcolor >> 8) & 0b1110;
    uint8_t b = (mdcolor >> 4) & 0b1110;

    return artist_api::Color::fromRGB(r << 4, g << 4, b << 4);
}

void Level::createZoneSpecific() {
    if (m_zoneNameShort == "GHZ") {
        artist_.setBackgroundColor(fromMDColor(0x08E0));

        bg.create(0x3C, 3);
        bg.addLayer(0, 32, 0.001, 0.5);
        bg.addLayer(32, 16, 0.002, 0.25);
        bg.addLayer(48, 16, 0.003, 0.125);
        bg.addLayer(64, 48, 0.005);
        bg.addLayer(112, 40, 0.060);
        const int waterDiv = 8;
        for (int i = 0; i < 104 / waterDiv; i++)
            bg.addLayer(152 + i * waterDiv, waterDiv, 0.060 + (float)i / 25.0);
    } else if (m_zoneNameShort == "LZ") {
        artist_.setBackgroundColor(fromMDColor(0x0000));
        bg.create(0x32, 7);
        bg.addLayer(0, 256, 0.005);
    } else if (m_zoneNameShort == "MZ") {
        artist_.setBackgroundColor(fromMDColor(0x00a0));
        bg.create(0x36, 2);
        bg.addLayer(0, 256, 0.005);
    } else if (m_zoneNameShort == "SBZ") {
        artist_.setBackgroundColor(fromMDColor(0x6400));
        bg.create(0x34, 4);
        bg.addLayer(0, 256, 0.005);
    } else if (m_zoneNameShort == "SLZ") {
        artist_.setBackgroundColor(fromMDColor(0x0000));
        bg.create(0x30, 1);
        bg.addLayer(0, 256, 0.005);
    } else if (m_zoneNameShort == "SYZ") {
        artist_.setBackgroundColor(fromMDColor(0x6260));
        bg.create(0x3D, 2);
        bg.addLayer(0, 256, 0.005);
    }

    // S Tubes
    if (m_zoneNameShort == "GHZ") {

        m_audio.dj().playMusic(m_audio.store().get<dj::Music>(
            m_audio.store().map<SonicResources>().music.greenHillZone));

        auto &layout = m_terrain.getLayout();

        for (int i = 0; i < layout.getHeight(); i++)
            for (int j = 0; j < layout.getWidth(); j++) {
                uint8_t chunkId = layout.getChunkId(j, i);

                int x = j * layout.getChunksRadiusPixels();
                int y = i * layout.getChunksRadiusPixels();

                if (chunkId == 0x1F)
                    m_entityPool.create(
                        new GimGHZ_STube(v2f(x + 8 + 8, y + 112), 0));
                else if (chunkId == 0x20)
                    m_entityPool.create(
                        new GimGHZ_STube(v2f(x + 248 - 8, y + 112), 1));
            }
    }
}

void Level::update() {

    if (!isFadeDeath) {
        if (!isFadeOut) {
            if (fade < 0xFF)
                fade++;
        } else {
            if (fade > 0)
                fade--;
            else
                end = true;
        }
    }

    if (isFadeDeath) {
        if (fade > 0)
            fade--;
        else {
            score = 0;
            playerDied = true;
            return;
        }
    }

    updateLevelSpecific();

    m_entityPool.update();

    // Time count
    tick++;

    if (!isTimeStopped)
        time = tick / 60;

    GameLoopTicker::instance().tick();
}

void Level::updateLevelSpecific() {
    if (m_zoneNameShort == "GHZ") {
        switch (m_act) {
        case 1:
            cam.setBottomBorder((cam.getPos().x < 0x16D0) ? 1024 : 1280);
            break;
        case 2:
            cam.setBottomBorder(
                (cam.getPos().x > 0x0EE0 && cam.getPos().x < 0x1580) ? 0x02E0
                                                                     : 1536);
        case 3:
            if (cam.getPos().x > 0x1680)
                cam.setBottomBorder(0x0400);
            else if (cam.getPos().x < 0x0400)
                cam.setBottomBorder(0x03F0);
            else
                cam.setBottomBorder(1536);
            break;
        }
    }
}

void Level::createSonic1LayeringObjects() {
    auto &layout = m_terrain.getLayout();

    for (int i = 0; i < layout.getHeight(); i++) {
        for (int j = 0; j < layout.getWidth(); j++) {
            if (!layout.isLayeringChunk(j, i))
                continue;

            int x = j * layout.getChunksRadiusPixels();
            int y = i * layout.getChunksRadiusPixels();

            m_entityPool.create(
                new LayerSwitcher(v2f(x + 128, y + 32), v2f(16, 64), 2));
            m_entityPool.create(
                new LayerSwitcher(v2f(x - 8, y + 128), v2f(16, 256), 0));
            m_entityPool.create(
                new LayerSwitcher(v2f(x + 264, y + 128), v2f(16, 256), 1));
        }
    }
}

void Level::draw() {
    cam.update();

    // bg.draw(cam, trn);

    // Terrain
    m_terrainDrawer.draw();

    // Entities
    m_entityPool.draw();

    drawHud();
}

void Level::drawHud() {
    auto &hudTex =
        store_.get<artist_api::Texture>(store_.map<SonicResources>().textures.hud);
    auto &artist = artist_;

    artist.drawSprite(
        artist_api::Sprite{
            .texture = hudTex,
            .rect = {.x = 1, .y = 60, .width = 40, .height = 16},
        },
        {.x = 16, .y = 8});
    artist.drawSprite(
        artist_api::Sprite{
            .texture = hudTex,
            .rect = {.x = 42, .y = 60, .width = 32, .height = 16},
        },
        {.x = 16, .y = 24});
    artist.drawSprite(
        artist_api::Sprite{
            .texture = hudTex,
            .rect = {.x = 75, .y = 60, .width = 40, .height = 16},
        },
        {.x = 16, .y = 40});

    auto minutes = time / 60;
    auto seconds = time % 60;

    auto &font = store_.get<artist_api::SpriteFont>(
        store_.map<SonicResources>().fonts.s1HudDigits);

    artist_.drawText(std::format("{:6d}", score), {.x = 64, .y = 8},
                               font);
    artist_.drawText(std::format("{:d}:{:02d}", minutes, seconds),
                               {.x = 56, .y = 24}, font);
    artist_.drawText(std::format("{:3d}", rings), {.x = 64, .y = 40},
                               font);
}

void Level::free() {}