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

class AgentManager : public CDMTimerNode
{
public:
    // 创建多个代理
    void CreateAgent(const char* pszTreeName)
    {
        auto agent = behaviac::Agent::Create<GameAgent>();
        if (agent->btload(pszTreeName))
        {
            agent->btsetcurrent(pszTreeName);
            agents.push_back(agent);  // 将新代理添加到管理列表中
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
    virtual void OnTimer(uint64_t qwIDEvent)
    {
            
    }
private:
    std::vector<GameAgent*> agents;  // 使用shared_ptr管理代理的生命周期
};

bool InitBehavic(behaviac::Workspace::EFileFormat ff)
{
    std::string strPath  = DMGetRootPath() + PATH_DELIMITER + "../../test/werewolf-game/behaviac/exported";
    behaviac::Workspace::GetInstance()->SetFilePath(strPath.c_str());
    behaviac::Workspace::GetInstance()->SetFileFormat(ff);
    return true;
}


class CMain : public IDMConsoleSink,
    public IDMThread,
    public CDMThreadCtrl,
    public CDMTimerNode,
    public TSingleton<CMain>
{
    friend class TSingleton<CMain>;


    typedef enum
    {
        eTimerID_UUID = 0,
        eTimerID_CRON = 1,
        eTimerID_STOP,
    } ETimerID;

    typedef enum
    {
        eTimerTime_UUID = 1000,
        eTimerTime_STOP = 20000,
    } ETimerTime;

public:
    virtual void ThrdProc()
    {
        std::cout << "test start" << std::endl;


        SetExePath();

        InitBehavic(behaviac::Workspace::EFF_xml);
    
        const char* szTreeName = "ParentBT";  // 设定行为树名称

        AgentManager manager;
        manager.CreateAgent(szTreeName);

        bool bBusy = false;

        while (!m_bStop)
        {
            bBusy = false;

            if (CDMTimerModule::Instance()->Run())
            {
                bBusy = true;
            }

            if (manager.UpdateAgents())
            {
                bBusy = true;
            }

            if (!bBusy)
            {
                SleepMs(1);
            }
        }

        std::cout << "test stop" << std::endl;
    }

    virtual void Terminate()
    {
        m_bStop = true;
    }

    virtual void OnCloseEvent()
    {
        Stop();
    }

    virtual void OnTimer(uint64_t qwIDEvent, dm::any& oAny)
    {
        switch (qwIDEvent)
        {
        case eTimerID_UUID:
        {

        }
        break;


        default:
            break;
        }
    }
private:
    CMain()
        : m_bStop(false)
    {
        HDMConsoleMgr::Instance()->SetHandlerHook(this);
    }

    virtual ~CMain()
    {
    }

private:
    bool __Run()
    {
        return false;
    }

private:
    volatile bool m_bStop;
};

int main(int argc, char* argv[])
{
    CMain::Instance()->Start(CMain::Instance());
    CMain::Instance()->WaitFor();
    return 0;
}
