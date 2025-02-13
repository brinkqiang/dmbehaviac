
#ifndef AGENT_MANAGER_H
#define AGENT_MANAGER_H

#include "behaviac/behaviac.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <memory>  // 用于std::shared_ptr

#include "./behaviac/exported/behaviac_generated/types/behaviac_types.h"

#include "dmutil.h"
#include "dmtimermodule.h"
#include "dmsingleton.h"
#include "dmthread.h"
#include "dmconsole.h"
#include "dmtypes.h"

#if BEHAVIAC_CCDEFINE_MSVC
#include <windows.h>
#include <tchar.h>
#endif

#include <iostream>
#include <csignal>  // 用于 raise() 函数
#include <cstdlib>  // 用于 std::exit()
#include <cstdio>   // 用于 printf
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h> // 用于 GenerateConsoleCtrlEvent
#endif

static void sendSigIntToSelf() {
#if defined(_WIN32) || defined(_WIN64)
    // 在 Windows 上模拟 CTRL_C_EVENT (SIGINT) 信号
    if (!GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0)) {
        std::cerr << "Failed to send SIGINT signal on Windows." << std::endl;
    } else {
        std::cout << "SIGINT signal simulated on Windows." << std::endl;
    }
#else
    // 在 Unix-like 系统（Linux, macOS）上发送 SIGINT 信号
    if (raise(SIGINT) != 0) {
        std::cerr << "Failed to send SIGINT signal on Unix-like system." << std::endl;
    } else {
        std::cout << "SIGINT signal sent on Unix-like system." << std::endl;
    }
#endif
}

class AgentManager : public CDMTimerNode, public TSingleton<AgentManager>
{
public:
    enum TimerID
    {
        TimerID_RUN = 1
    };
    enum TimerCount
    {
        TimerCount_RUN = 1000
    };

    // 创建多个代理
    void CreateAgent(const char* pszTreeName)
    {
        auto agent = behaviac::Agent::Create<GameAgent>();
        if (agent->btload(pszTreeName))
        {
            agent->btsetcurrent(pszTreeName);
            agents.push_back(agent);  // 将新代理添加到管理列表中

            agent->InitGame();
        }
        else
        {
            std::cout << "Failed to load behavior tree for agent!" << std::endl;
        }
    }

    // 更新所有代理
    bool UpdateAgents()
    {
        behaviac::Workspace::GetInstance()->Update();
        return false;
    }

    // 销毁所有代理
    void CleanupAgents()
    {
        for (auto& agent : agents)
        {
            behaviac::Agent::Destroy(agent);  // 销毁代理
        }
        agents.clear();
    }

    void StartUp()
    {
        SetTimer(TimerID_RUN, TimerCount_RUN);
    }

    void EndGame()
    {
        KillTimer();
        CleanupAgents();
        sendSigIntToSelf();
    }

    virtual void OnTimer(uint64_t qwIDEvent)
    {
        switch(qwIDEvent)
        {
        case TimerID_RUN:
        {
            UpdateAgents();
        }
        break;
        }
    }
private:
    std::vector<GameAgent*> agents;  // 使用shared_ptr管理代理的生命周期
};

#endif // AGENT_MANAGER_H