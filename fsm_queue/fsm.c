#include <fsm.h>

const char* print_enum_name(int n)
{
        switch(n) {
                ENUMNAME(EVENT_CLOCK);
                ENUMNAME(EVENT_REST);
                ENUMNAME(EVENT_WORK);
                ENUMNAME(EVENT_DREAM);
                ENUMNAME(AT_HOME);
                ENUMNAME(AT_SCHOOL);
                ENUMNAME(ON_BED);
                default: return "unknow enum";
        }
}


FsmTable_t XiaoMingTable[] =
{
    //{到来的事件，当前的状态，将要要执行的函数，下一个状态}
    { EVENT_CLOCK,    ON_BED,        GetUp,        AT_HOME },
    { EVENT_CLOCK,    AT_HOME,       Go2School,    AT_SCHOOL },
    { EVENT_REST,     AT_SCHOOL,     HaveLunch,    AT_SCHOOL },
    { EVENT_CLOCK,    AT_SCHOOL,     Go2Home,      AT_HOME },
    { EVENT_WORK,     AT_HOME,       DoHomework,   AT_HOME },
    { EVENT_REST,     AT_HOME,       Go2Bed,       ON_BED },
    { EVENT_DREAM,    ON_BED,        DoNothing,    ON_BED },
    //add your codes here
};


void GetUp()
{ printf("xiao ming get up\n");}

void Go2School()
{ printf("xiao ming go to school\n");}

void HaveLunch()
{ printf("xiao ming have lunch\n");}

void DoHomework()
{ printf("xiao ming do homework\n");}

void Go2Bed()
{ printf("xiao ming go to bed\n");}

void Go2Home()
{ printf("xiao ming go to home\n");}


void DoNothing()
{ printf("xiao ming do notingi\n");}

void InitFsm(FSM_t* fsm, int curstat)
{
  FSM_Regist(fsm, XiaoMingTable);
  fsm->curState = curstat;
}

/*状态机注册*/
void FSM_Regist(FSM_t* pFsm, FsmTable_t* pTable)
{
    pFsm->FsmTable = pTable;
}

/*状态迁移*/
void FSM_StateTransfer(FSM_t* pFsm, int state)
{
    pFsm->curState = state;
}

/*事件处理*/
void FSM_EventHandle(FSM_t* pFsm, int event)
{
    FsmTable_t* pActTable = pFsm->FsmTable;
    void (*eventActFun)() = 0;  //函数指针初始化为空指针
    int NextState;
    int CurState = pFsm->curState;
    int flag = 0; //标识是否满足条件
    /*获取当前动作函数*/
    for (int i = 0; i <  sizeof(XiaoMingTable) / sizeof(FsmTable_t); eventActFun++,i++)
    {
        //当且仅当指定当前状态下的指定的事件到来，我才执行它
        if (event == pActTable[i].event)
        {  
            if(CurState == pActTable[i].CurState)
            {
                flag = 1;
                eventActFun = pActTable[i].eventActFun;
		NextState = pActTable[i].NextState;
                break;
            }
            //某些事件到来，基于当前状态可以不对事件做处理
	    else
	    {
                //printf("[event handler],for stat:%s,event:%s should be ignored!\n",
                //       print_enum_name(pActTable[i].CurState), print_enum_name(event));
                // break;
            }
        }
    }

    if (flag) //如果满足条件了
    {
        /*动作执行*/
        if (eventActFun)
        {
            eventActFun();
        }

        //跳转到下一个状态
        FSM_StateTransfer(pFsm, NextState);
    }
    else
    {
        // do nothing
    }
}
