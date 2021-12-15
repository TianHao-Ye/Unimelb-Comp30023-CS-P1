#include"process_cpu.h"

#define MAXCHAR 100

typedef struct inputs inputList;

struct inputs{
    char *file_name;
    int number_of_processor;
    int own_scheduler;
};

/*file management*/
inputList *read_in_arguments(int argc, char *argv[]);        
int read_process(FILE *file, priority_queue **queue);

void print_performance_statistics(priority_queue *finished, int makespan, int cpu_number, int process_number);

void free_cpu(cpu *cpu_list, int cpu_number);