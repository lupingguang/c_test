#include <fsm.h>
#include <unistd.h>
#include <stdio.h>
extern const char* print_enum_name(int);
int main()
{
    FSM_t fsm;
    InitFsm(&fsm, ON_BED);
    int event = EVENT_CLOCK; 
    printf("init fsm state %s\n", print_enum_name(fsm.curState));
    printf("-------------------------------------------\n");
    //小明的一天,周而复始的一天又一天，进行着相同的活动
   // while (1)
    {

        sleep(3);
        printf("event %s is coming...\n", print_enum_name(event));
        FSM_EventHandle(&fsm, event);
        printf("fsm current state %s\n", print_enum_name(fsm.curState));
        //test(&event); 
        sleep(2);  //休眠3秒，方便观察
 event = EVENT_CLOCK;
        printf("event %s is coming...\n", print_enum_name(event));
        FSM_EventHandle(&fsm, event);
        printf("fsm current state %s\n", print_enum_name(fsm.curState));        
        //test(&event); 
        sleep(2);  //休眠3秒，方便观察
 event = EVENT_REST;
        printf("event %s is coming...\n", print_enum_name(event));
        FSM_EventHandle(&fsm, event);
        printf("fsm current state %s\n", print_enum_name(fsm.curState)); 
        sleep(2);  //休眠3秒，方便观察
 event = EVENT_REST;
        printf("event %s is coming...\n", print_enum_name(event));
        FSM_EventHandle(&fsm, event);
        printf("fsm current state %s\n", print_enum_name(fsm.curState));
        sleep(2);  //休眠3秒，方便观察
 event = EVENT_CLOCK;
        printf("event %s is coming...\n", print_enum_name(event));
        FSM_EventHandle(&fsm, event);
        printf("fsm current state %s\n", print_enum_name(fsm.curState));        
        sleep(2);  //休眠3秒，方便观察
 event = EVENT_WORK;
        printf("event %s is coming...\n", print_enum_name(event));
        FSM_EventHandle(&fsm, event);
        printf("fsm current state %s\n", print_enum_name(fsm.curState));        
        sleep(2);  //休眠3秒，方便观察
 event = EVENT_REST;
        printf("event %s is coming...\n", print_enum_name(event));
        FSM_EventHandle(&fsm, event);
        printf("fsm current state %s\n", print_enum_name(fsm.curState));
        sleep(2);  //休眠3秒，方便观察
 event = EVENT_DREAM;
        printf("event %s is coming...\n", print_enum_name(event));
        FSM_EventHandle(&fsm, event);
        printf("fsm current state %s\n", print_enum_name(fsm.curState));        
        sleep(2);  //休眠3秒，方便观察
 event = EVENT_CLOCK;

    }

    return 0;
}
