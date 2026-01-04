#pragma once
#ifndef GAME_DATA_H
#define GAME_DATA_H



class GameData {
public:
    GameData() = default;
    virtual ~GameData() = default;

protected:
    const char* mGameName;
private:

};

#endif // GAME_DATA_H
