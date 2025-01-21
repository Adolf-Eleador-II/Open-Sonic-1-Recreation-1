#pragma once

#include "core/game_enviroment/artist/ArtistStructs.h"
#include "new-terrain.hpp"

#include "core/Screen.h"
#include "Camera.h"

namespace terrain {

enum class SensorDirection {
    DOWN,
    RIGHT,
    UP,
    LEFT,
};

struct SensorResult {
    HexAngle      angle;
    int           distance;
    BlockSolidity solidity;
    
    bool isFlagged() { return !angle.isRotatable(); }

    static SensorResult Empty() {
        return terrain::SensorResult {
            HexAngle { 0 },
            31,
            terrain::BlockSolidity::EMPTY
        };
    }
};

//https://info.sonicretro.org/SPG:Solid_Tiles#Sensors
class Sensor {
public:
    Sensor(v2f position, SensorDirection direction, Terrain& terrain, artist_api::Artist &artist)
        : m_terrain(terrain) 
        , m_position(position)
        , m_direction(direction)
        , artist_(artist)
    {}

    void         setLayer(TerrainLayer layer) { m_layer = layer; }
    TerrainLayer getLayer()                   { return m_layer;  }

    void     setPosition(v2f position) { m_position = position; }
    v2f getPosition()                  { return m_position;     }

    void     move(v2f delta)           { m_position.x += delta.x; m_position.y += delta.y; }

    void            setDirection(SensorDirection direction) { m_direction = direction; }
    SensorDirection getDirection()                          { return m_direction;      }

    SensorResult scan() {
        auto gridPos     = getCurrentBlockPositionInGrid(m_position);
        auto blockResult = getBlock(gridPos.x, gridPos.y);

        return SensorResult {
            blockResult.block.tile.angle,
            blockResult.distance,
            (m_layer == TerrainLayer::NORMAL) 
                ? blockResult.block.solidityNormalLayer 
                : blockResult.block.solidityAlternateLayer
        };
    }

    int getDistance() {
        return scan().distance;
    }

    void draw(Camera& cam, artist_api::Color color = artist_api::Color::WHITE()) {
        float poscamx = m_position.x - cam.getPos().x;
        float poscamy = m_position.y - cam.getPos().y;

        drawDot(poscamx, poscamy, color, cam);

        auto gridPos = getCurrentBlockPositionInGrid(m_position);
        auto blockResult = getBlock(gridPos.x, gridPos.y);

        int distance = blockResult.distance;

        switch (m_direction) {
            case SensorDirection::DOWN:
                drawDot(poscamx, poscamy + distance, artist_api::Color::WHITE(), cam);
                break;
            case SensorDirection::UP:
                drawDot(poscamx, poscamy - distance, artist_api::Color::WHITE(), cam);
                break;
            case SensorDirection::LEFT:
                drawDot(poscamx - distance, poscamy, artist_api::Color::WHITE(), cam);
                break;
            case SensorDirection::RIGHT:
                drawDot(poscamx + distance, poscamy, artist_api::Color::WHITE(), cam);
                break;
        }
    }

private:
    artist_api::Artist &artist_;
    SensorDirection m_direction;
    v2f        m_position;
    Terrain&        m_terrain;
    TerrainLayer    m_layer = TerrainLayer::NORMAL;

private:
    struct BlockResult {
        ChunkBlock  block;
        v2i    gridPosition;
        int         distance;
        int         height;
    };

    BlockResult getBlock(int gridX, int gridY) {
        BlockResult baseBlock = getBlockExtended(gridX, gridY);

        if (baseBlock.height >= 1 && baseBlock.height <= 15 && checkBlockSolidity(baseBlock.block) != BlockSolidity::EMPTY)
            return baseBlock;

        // Regression
        else if (baseBlock.height >= 16 && checkBlockSolidity(baseBlock.block) != BlockSolidity::EMPTY) {
            BlockResult regressionBlock = getBlockExtended(gridX, gridY, -1);

            if (isBlockEmpty(regressionBlock))
                return baseBlock;

            return regressionBlock;
        }

        // Extension
        else if (isBlockEmpty(baseBlock)) {
            BlockResult extensionBlock = getBlockExtended(gridX, gridY, +1);

            return extensionBlock;
        }

        printf("Something strange happened :(. -> %d\n", baseBlock.height);
        return baseBlock;
    }    

    BlockResult getBlockExtended(int gridX, int gridY, int ext = 0) {
        BlockResult result = {};
        
        switch (m_direction) {
            case (SensorDirection::DOWN): {
                result.gridPosition = v2i(gridX, gridY + ext);
                result.block        = m_terrain.getBlockFromGrid(result.gridPosition.x, result.gridPosition.y, m_layer);
                result.height       = abs(m_terrain.getVerticalHeightInTile(m_position.x, result.block.tile));
                
                auto blockBottom    = (result.gridPosition.y * TERRAIN_TILE_SIZE) + TERRAIN_TILE_SIZE ;

                result.distance     = (blockBottom - result.height) - m_position.y; 
                break;
            }
            case (SensorDirection::UP): {
                result.gridPosition = v2i(gridX, gridY - ext);
                result.block        = m_terrain.getBlockFromGrid(result.gridPosition.x, result.gridPosition.y, m_layer);
                result.height       = abs(m_terrain.getVerticalHeightInTile(m_position.x, result.block.tile));

                auto blockTop       = (result.gridPosition.y * TERRAIN_TILE_SIZE);

                result.distance     = m_position.y - (blockTop + result.height); 
                break;
            }
            case (SensorDirection::LEFT): {
                result.gridPosition = v2i(gridX - ext, gridY);
                result.block        = m_terrain.getBlockFromGrid(result.gridPosition.x, result.gridPosition.y, m_layer);
                result.height       = abs(m_terrain.getHorizontalHeightInTile(m_position.y, result.block.tile));

                auto blockLeft      = (result.gridPosition.x * TERRAIN_TILE_SIZE);

                result.distance     = m_position.x - (blockLeft + result.height); 
                break;
            }
            case (SensorDirection::RIGHT): {
                result.gridPosition = v2i(gridX + ext, gridY);
                result.block        = m_terrain.getBlockFromGrid(result.gridPosition.x, result.gridPosition.y, m_layer);
                result.height       = abs(m_terrain.getHorizontalHeightInTile(m_position.y, result.block.tile));

                auto blockRight     = (result.gridPosition.x * TERRAIN_TILE_SIZE) + TERRAIN_TILE_SIZE - 1;

                result.distance     = (blockRight - result.height) - m_position.x;
                break;
            }
        }

        return result;
    }


    bool isBlockEmpty(BlockResult& blockResult) {
        return (blockResult.height == 0 || checkBlockSolidity(blockResult.block) == BlockSolidity::EMPTY);
    }

    BlockSolidity checkBlockSolidity(ChunkBlock& block) {
        return (m_layer == TerrainLayer::NORMAL) ? block.solidityNormalLayer : block.solidityAlternateLayer;
    }

    v2i getCurrentBlockPositionInGrid(const v2f& position) {
        return v2i(
            (int)(position.x / TERRAIN_TILE_SIZE),
            (int)(position.y / TERRAIN_TILE_SIZE)
        );
    }

    void drawDot(float x, float y, artist_api::Color color, Camera& cam) {
        artist_.drawRectangleRadius({.x = 0.5f, .y = 0.5f}, {
            .x = x, .y = y
        }, {
            .fillColor = color,
            .borderThickness = 0.25f,
            .borderColor = artist_api::Color::fromARGB(0xAA'00'00'00)
        });
    }
};

}
