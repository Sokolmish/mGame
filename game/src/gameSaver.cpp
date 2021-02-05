#include "../include/gameSaver.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <system_error>

#include "../lib/json.hpp"
using json = nlohmann::json;

namespace fs = std::filesystem;


// Local static

static std::string readTextFile(const std::string &path) {
    std::ifstream ifs(path);
    std::string line;
    std::stringstream ss;
    while (std::getline(ifs, line))
        ss << line << std::endl;
    ifs.close();
    return ss.str();
}

static void writeTextFile(const std::string &path, const std::string &text) {
    std::ofstream ofs(path);
    ofs << text << std::endl;
    ofs.close();
}


// Constructor

GameSaver::GameSaver(const std::string &path) {
    savePath = path;
    chunksPath = savePath + "/chunks";
}


// Private

bool GameSaver::createDir(const std::string &path) const {
    std::error_code err_code;
    if (!fs::create_directories(path, err_code)) {
        std::cout << "ERROR: cannot create directory: " << path
                    << "(" << err_code << ")"
                    << std::endl;
        return false;
    }
    return true;
}

bool GameSaver::checkSaveStruct() const {
    std::error_code err_code;
    if (!fs::is_directory(savePath, err_code))
        return false;
    if (!fs::is_directory(chunksPath, err_code))
        return false;
    return true;
}


// Public

bool GameSaver::initSave() {
    std::error_code err_code;

    std::cout << "Initializing save..." << std::endl;

    // Create directories
    if (!createDir(savePath))
        return false;
    if (!createDir(chunksPath))
        return false;

    std::cout << "Save folder successfully initialized" << std::endl;
    return true;
}

bool GameSaver::saveChunk(int x, int y, const Chunk &chunk) {
    std::error_code err_code;

    std::cout << "Saving chunk (" << x << "," << y << ")..." << std::endl;

    if (!checkSaveStruct()) {
        std::cout << "Saving FAILED: save directory has wrong structure" << std::endl;
        return false;
    }

    std::vector<uint8_t> buff;
    const uint8_t *bytes = nullptr;

    uint16_t size = (uint16_t)chunk.data.size();
    bytes = reinterpret_cast<const uint8_t*>(&size);
    buff.push_back(bytes[0]);
    buff.push_back(bytes[1]);
    for (const auto &e : chunk.data) {
        uint16_t tmp16 = e.first;
        bytes = reinterpret_cast<const uint8_t*>(&tmp16);
        buff.push_back(bytes[0]);
        buff.push_back(bytes[1]);
        tmp16 = e.second.getId();
        bytes = reinterpret_cast<const uint8_t*>(&tmp16);
        buff.push_back(bytes[0]);
        buff.push_back(bytes[1]);
    }

    std::string filename = "x" + std::to_string(x) + "y" + std::to_string(y);
    std::ofstream ofs(chunksPath + "/" + filename, std::ios::binary | std::ios::out);
    ofs.write(reinterpret_cast<char*>(buff.data()), buff.size());
    ofs.flush();
    ofs.close();

    std::cout << "Chunk saved" << std::endl;
    return true;
}

bool GameSaver::loadChunk(int x, int y, Chunk &chunk) {
    std::string filename = "x" + std::to_string(x) + "y" + std::to_string(y);
    std::ifstream ifs(chunksPath + "/" + filename, std::ios::binary);
    std::vector<uint8_t> data;
    while (!ifs.eof())
        data.push_back(ifs.get());
    ifs.close();

    uint16_t size = *(reinterpret_cast<uint16_t*>(data.data() + 0));
    for (size_t i = 0; i < size; i++) {
        size_t ind = 2 + i * 4;
        uint16_t blockInd = *(reinterpret_cast<uint16_t*>(&data[ind]));
        uint16_t blockId = *(reinterpret_cast<uint16_t*>(&data[ind + 2]));
        chunk.data.insert({ blockInd, Block(blockId) });
    }

    return true;
}

bool GameSaver::savePlayer(const Player &player) {
    std::cout << "Saving player data" << std::endl;

    json sidebar = json::array();
    for (size_t i = 0; i < player.sidebar.size(); i++) {
        sidebar.push_back(json({
            { "Id", player.sidebar[i].Id },
            { "index", i }
        }));
    }

    json inventory = json::array();
    for (size_t i = 0; i < player.inventory.size(); i++) {
        inventory.push_back(json({
            { "Id", player.inventory[i].Id },
            { "index", i }
        }));
    }

    json playerJson = {
        { "pos", json::array({
            player.getPos().x,
            player.getPos().y,
            player.getPos().z
        })},
        { "velocity", json::array({
            player.getVelocity().x,
            player.getVelocity().y,
            player.getVelocity().z
        })},
        { "acceleration", json::array({
            player.getAcceleration().x,
            player.getAcceleration().y,
            player.getAcceleration().z
        })},
        { "yaw", player.getYaw() },
        { "pitch", player.getPitch() },
        { "isFlight", player.isFlight() },
        { "selectedItem", player.getSelectedCell() },
        { "sidebar", sidebar },
        { "inventory", inventory }
    };

    writeTextFile(savePath + "/player.json", playerJson.dump(-1));
    std::cout << "Player data saved" << std::endl;
    return true;
}

bool GameSaver::loadPlayer(Player &player) {
    std::string file = readTextFile(savePath + "/player.json");
    json playerData = json::parse(file);

    player.setPos(
        playerData["pos"][0].get<float>(),
        playerData["pos"][1].get<float>(),
        playerData["pos"][2].get<float>()
    );
    player.setVelocity(glm::vec3(
        playerData["velocity"][0].get<float>(),
        playerData["velocity"][1].get<float>(),
        playerData["velocity"][2].get<float>()
    ));
    player.setAcceleration(glm::vec3(
        playerData["acceleration"][0].get<float>(),
        playerData["acceleration"][1].get<float>(),
        playerData["acceleration"][2].get<float>()
    ));
    player.setYaw(playerData["yaw"].get<float>());
    player.setPitch(playerData["pitch"].get<float>());
    player.setFlight(playerData["isFlight"].get<bool>());
    player.selectItem(playerData["selectedItem"].get<int>());
    
    for (const auto &e : playerData["sidebar"])
        player.sidebar[e["index"].get<int>()] = e["Id"].get<uint16_t>();
    for (const auto &e : playerData["inventory"])
        player.inventory[e["index"].get<int>()] = e["Id"].get<uint16_t>();

    return true;
}
