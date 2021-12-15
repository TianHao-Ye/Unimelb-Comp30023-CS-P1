#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>
#include<assert.h>

typedef struct process_node process;
typedef struct cpu_node cpu;
typedef struct priority_queue priority_queue;

struct priority_queue{
    process *head;
};

struct process_node{
    int pid;
    /*subpid -1 indicate non child process, num > 0 indicate child number of pid*/
    int sub_pid;
    int arrive_time;
    int ori_execution_time;
    int execution_time;
    int remaining_time;
    int complete_time;
    int process_number;
    /*0 indicate non-para, 1 indicate para*/
    int parallelisable;
    process * next;
    process * before;
};

struct cpu_node{
    int id;
    priority_queue *queue;
    int total_remaining_time;
};

void push_remaining_time_prior(priority_queue *queue, process *p);
void push_arrive_time_prior(priority_queue **queue, process *p);
void queue_traverse(priority_queue *queue);
process *queue_pop(priority_queue *queue);
int is_queue_empty(priority_queue *queue);
int queue_size(priority_queue *queue);
void collect_complete_info(priority_queue *finished, priority_queue *completed_queue);


/*process & cpu management*/
process *construct_new_process(int process_data[], int child_number);
int run_process(cpu *cpu_list, int cpu_number, priority_queue *process_table, priority_queue *completed_queue);
int any_process_arriving(int time, priority_queue *queue);
void load_process(process *p, cpu *cpu_list, int cpu_number);
int is_all_cpu_finished(cpu *cpu_list, int cpu_number);
void initialize_cpu(cpu *cpu_list, int cpu_number);
void cpu_do_work(int time, cpu *cpu_list, int cpu_number, int last_running_process[], priority_queue *finished, int loaded_number, int *finished_number);
int is_all_subprocess_completed(process *p, priority_queue *finished);
void travsre_print_cpu(cpu *cpu_list, int cpu_number);
process *get_head_process(priority_queue *queue);
void update_runned_process_cpu_info(int cpu_index, cpu *working_cpu, process **p);
int find_lowest_workload_cpu(cpu *cpu_list, int cpu_number, int binding_index[]);
int find_child_number(process *p, int cpu_number);
int *get_process_data(process *p);
void print_running_info(int time, process *poped_p, int i);
void reset_binding_index(int binding_index[], int cpu_number);
