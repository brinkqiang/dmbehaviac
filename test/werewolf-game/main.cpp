
#include "agentmanager.h"


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

        AgentManager::Instance()->CreateAgent(szTreeName);
        AgentManager::Instance()->StartUp();
        bool bBusy = false;

        while (!m_bStop)
        {
            bBusy = false;

            if (CDMTimerModule::Instance()->Run())
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
