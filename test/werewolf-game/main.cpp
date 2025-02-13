#include "behaviac/behaviac.h"
#include <iostream>
#include <ctime>
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

bool InitBehavic(behaviac::Workspace::EFileFormat ff)
{
    // 设置行为树的文件路径和格式
    behaviac::Workspace::GetInstance()->SetFilePath("../../test/werewolf-game/behaviac/exported");
    behaviac::Workspace::GetInstance()->SetFileFormat(ff);
    return true;
}

bool InitAgent(const char* pszTreeName)
{
    // 创建并加载代理
    g_agent = behaviac::Agent::Create<FirstAgent>();
    bool bRet = g_agent->btload(pszTreeName);
    if (bRet)
    {
        g_agent->btsetcurrent(pszTreeName);
    }
    return bRet;
}

void CleanupAgent()
{
    // 销毁代理
    behaviac::Agent::Destroy(g_agent);
}

void CleanupBehaviac()
{
    // 清理Behaviac工作空间
    behaviac::Workspace::GetInstance()->Cleanup();
}

int main()
{
    SetExePath();

    cout << "Running behavior tree..." << endl;

    const char* szTreeName = "ParentBT";  // 设定行为树名称
    int loopCount = 1000;  // 执行的循环次数

    InitBehavic(behaviac::Workspace::EFF_xml);  // 初始化Behaviac，使用XML格式
    if (!InitAgent(szTreeName))  // 初始化代理并加载行为树
    {
        cout << "Failed to load behavior tree: " << szTreeName << endl;
        return -1;
    }

    clock_t start = clock();  // 开始计时

    for (int i = 0; i < loopCount; ++i)
    {
        behaviac::Workspace::GetInstance()->Update();  // 更新行为树
    }

    clock_t finish = clock();  // 结束计时
    float duration = (float)(finish - start) / CLOCKS_PER_SEC;  // 计算执行时长

    cout << "Duration (seconds): " << duration << " RunCount: " << loopCount << endl;

    CleanupAgent();  // 清理代理
    CleanupBehaviac();  // 清理Behaviac

    return 0;
}
