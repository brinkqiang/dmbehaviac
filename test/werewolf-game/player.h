#include "behaviac/behaviac.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>

using namespace std;
using namespace behaviac;

class Player {
public:
    string name;
    string role;
    bool isAlive;

    Player(string name, string role) : name(name), role(role), isAlive(true) {}

    virtual void Act(vector<Player*>& players) {}
};

class Werewolf : public Player {
public:
    Werewolf(string name) : Player(name, "Werewolf") {}

    void Act(vector<Player*>& players) override {
        // 选择一个活着的目标进行袭击
        vector<Player*> targets;
        for (auto& p : players) {
            if (p->isAlive && p->role != "Werewolf") {
                targets.push_back(p);
            }
        }
        if (!targets.empty()) {
            int targetIndex = rand() % targets.size();
            targets[targetIndex]->isAlive = false;
            cout << name << " 杀害了 " << targets[targetIndex]->name << endl;
        }
    }
};

class Villager : public Player {
public:
    Villager(string name) : Player(name, "Villager") {}

    void Act(vector<Player*>& players) override {
        // 村民参与投票
        cout << name << " 参与投票。" << endl;
    }
};

class Seer : public Player {
public:
    Seer(string name) : Player(name, "Seer") {}

    void Act(vector<Player*>& players) override {
        // 选择一个目标查看身份
        vector<Player*> targets;
        for (auto& p : players) {
            if (p->isAlive && p != this) {
                targets.push_back(p);
            }
        }
        if (!targets.empty()) {
            int targetIndex = rand() % targets.size();
            cout << name << " 预言家查看 " << targets[targetIndex]->name << " 的身份：" << targets[targetIndex]->role << endl;
        }
    }
};
