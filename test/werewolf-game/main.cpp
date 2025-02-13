#include "behaviac/behaviac.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <memory>  // 用于std::shared_ptr

#include "./behaviac/exported/behaviac_generated/types/behaviac_types.h"

#if BEHAVIAC_CCDEFINE_MSVC
#include <windows.h>
#include <tchar.h>
#endif

using namespace std;
using namespace behaviac;

FirstAgent* g_agent = nullptr;  // 使用你自己的代理类 FirstAgent

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

class AgentManager
{
public:
    // 创建多个代理
    void CreateAgent(const char* pszTreeName)
    {
        auto agent = behaviac::Agent::Create<FirstAgent>();
        if (agent->btload(pszTreeName))
        {
            agent->btsetcurrent(pszTreeName);
            agents.push_back(agent);  // 将新代理添加到管理列表中
        }
        else
        {
            cout << "Failed to load behavior tree for agent!" << endl;
        }
    }

    // 更新所有代理
    void UpdateAgents()
    {
        for (auto& agent : agents)
        {
            behaviac::Workspace::GetInstance()->Update();  // 更新每个代理的行为树
        }
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

private:
    std::vector<FirstAgent*> agents;  // 使用shared_ptr管理代理的生命周期
};

bool InitBehavic(behaviac::Workspace::EFileFormat ff)
{
    behaviac::Workspace::GetInstance()->SetFilePath("../../test/werewolf-game/behaviac/exported");
    behaviac::Workspace::GetInstance()->SetFileFormat(ff);
    return true;
}

int main()
{
    SetExePath();
    cout << "Running behavior tree with multiple agents..." << endl;

    const char* szTreeName = "ParentBT";  // 设定行为树名称
    int loopCount = 10;  // 执行的循环次数

    InitBehavic(behaviac::Workspace::EFF_xml);  // 初始化Behaviac，使用XML格式

    // 创建 AgentManager 并添加多个代理
    AgentManager manager;
    manager.CreateAgent(szTreeName);
    manager.CreateAgent(szTreeName);  // 你可以继续添加更多的代理

    clock_t start = clock();  // 开始计时

    for (int i = 0; i < loopCount; ++i)
    {
        manager.UpdateAgents();  // 更新所有代理的行为树
    }

    clock_t finish = clock();  // 结束计时
    float duration = (float)(finish - start) / CLOCKS_PER_SEC;  // 计算执行时长

    cout << "Duration (seconds): " << duration << " RunCount: " << loopCount << endl;

    manager.CleanupAgents();  // 清理所有代理

    return 0;
}
