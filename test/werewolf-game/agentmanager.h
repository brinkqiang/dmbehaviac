
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

static void SetExePath()
{
#if BEHAVIAC_CCDEFINE_MSVC
	TCHAR szCurPath[_MAX_PATH];

	GetModuleFileName(NULL, szCurPath, _MAX_PATH);

	TCHAR* p = szCurPath;

	while (_tcschr(p, L'\\'))
	{
		p = _tcschr(p, L'\\');
		p++;
	}

	*p = L'\0';

	SetCurrentDirectory(szCurPath);
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