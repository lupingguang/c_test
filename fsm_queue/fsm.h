#ifndef __FSM_H__
#define __FSM_H__

#include <stdio.h>
//比如我们定义了小明一天的状态如下
enum
{
    AT_HOME = 11,
    AT_SCHOOL,
    ON_BED,
};

enum
{
    EVENT_CLOCK = 1,
    EVENT_REST,
    EVENT_WORK,
    EVENT_DREAM,
};
#ifndef ENUMNAME
	#define ENUMNAME(n) case n: return #n
#endif

typedef struct FsmTable_s
{
    int event;   //表中一项的事件
    int CurState;  //表中一项当前状态
    void (*eventActFun)();  //表中一项函数指针
    int NextState;  //表中一项下一个状态
}FsmTable_t;

void GetUp();

void Go2School();

void HaveLunch();

void DoHomework();

void Go2Bed();

void Go2Home();

void DoNothing();
typedef struct FSM_s
{
    int curState;  //一个状态机的当前状态
    FsmTable_t* FsmTable; //一个状态机的状态表 
}FSM_t;

void InitFsm(FSM_t* fsm,int curstat);

/*状态机注册*/
void FSM_Regist(FSM_t* pFsm, FsmTable_t* pTable);

/*状态迁移*/
void FSM_StateTransfer(FSM_t* pFsm, int state);

/*事件处理*/
void FSM_EventHandle(FSM_t* pFsm, int event);

#endif
