#ifndef __GAME_SAVER_H__
#define __GAME_SAVER_H__

#include <string>

#include "gameWorld.hpp"
#include "player.hpp"

class GameSaver {
private:
    std::string savePath;
    std::string chunksPath;

    bool createDir(const std::string &path) const;
    bool checkSaveStruct() const;

public:
    GameSaver(const std::string &path);

    bool initSave();

    bool saveChunk(int x, int y, const Chunk &chunk);
    bool loadChunk(int x, int y, Chunk &chunk);

    bool savePlayer(const Player &player);
    bool loadPlayer(Player &player);
};

#endif
