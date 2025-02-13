
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>

class Player {
public:
    std::string name;
    std::string role;
    bool isAlive;

    Player(std::string name, std::string role) : name(name), role(role), isAlive(true) {}

    virtual void Act(std::vector<Player*>& players) {}
};

class Werewolf : public Player {
public:
    Werewolf(std::string name) : Player(name, "Werewolf") {}

    void Act(std::vector<Player*>& players) override {
        // 选择一个活着的目标进行袭击
        std::vector<Player*> targets;
        for (auto& p : players) {
            if (p->isAlive && p->role != "Werewolf") {
                targets.push_back(p);
            }
        }
        if (!targets.empty()) {
            int targetIndex = std::rand() % targets.size();
            targets[targetIndex]->isAlive = false;
            std::cout << name << " 杀害了 " << targets[targetIndex]->name << std::endl;
        }
    }
};

class Villager : public Player {
public:
    Villager(std::string name) : Player(name, "Villager") {}

    void Act(std::vector<Player*>& players) override {
        // 村民参与投票
        std::cout << name << " 参与投票。" << std::endl;
    }
};

class Seer : public Player {
public:
    Seer(std::string name) : Player(name, "Seer") {}

    void Act(std::vector<Player*>& players) override {
        // 选择一个目标查看身份
        std::vector<Player*> targets;
        for (auto& p : players) {
            if (p->isAlive && p != this) {
                targets.push_back(p);
            }
        }
        if (!targets.empty()) {
            int targetIndex = std::rand() % targets.size();
            std::cout << name << " 预言家查看 " << targets[targetIndex]->name << " 的身份：" 
                      << targets[targetIndex]->role << std::endl;
        }
    }
};

#endif // PLAYER_H