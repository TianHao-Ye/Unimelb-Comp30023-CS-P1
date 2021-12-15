#include"util.h"

int read_process(FILE *file, priority_queue **queue){
    char str[MAXCHAR];
    int position = 0;
    int process_number = 0;

    if (file == NULL){
        printf("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    
    while (fgets(str, MAXCHAR, file) != NULL){
        char is_parallelisable[2] = "p";
        int process_data[4];
        /* Extract the first token*/
        char *token = strtok(str, " ");
       
        process_data[position++] = atoi(token);
        /*loop through the string to extract all other tokens*/
        while(token != NULL) {
            token = strtok(NULL, " ");
            
            if(token != NULL){
                if(position != 3){
                    process_data[position++] = atoi(token);
                }
                else{
                    if(strcmp(token, is_parallelisable)==1){
                        process_data[position] = 1;
                    }
                    else{
                        process_data[position] = 0;
                    }
                }
            }
        }
        process *new_process = construct_new_process(process_data, 1);
        push_arrive_time_prior(queue, new_process);
        process_number++;
        position = 0;
    }
    return process_number;
}

inputList *read_in_arguments(int argc, char *argv[]){
    int i;
    inputList *input_list = (inputList *)malloc(sizeof(inputList));
    assert(input_list);
    char *file_indicator = "-f";
    char *processors_indicator = "-p";
    char *scheduler_indicator = "-c";
    
    for(i=0; i < argc; i++){
        char* argument = argv[i];
        if(strcmp(argument, scheduler_indicator) == 0){
            input_list->own_scheduler = 1;
        }
        else if(strcmp(argument, file_indicator) == 0){
            input_list->file_name= argv[i+1];
        }
        else if(strcmp(argument, processors_indicator) == 0){
            input_list->number_of_processor = atoi(argv[i+1]);
        }
        else{
            continue;
        }
    }
    if(input_list->own_scheduler != 1){
        input_list->own_scheduler = 0;
    }

    return input_list;
}

void free_cpu(cpu *cpu_list, int cpu_number){
    int i;
    for(i=0; i< cpu_number; i++){
        free(cpu_list[i].queue);
        cpu_list[i].queue =NULL;
    }
}

void print_performance_statistics(priority_queue *completed_queue, int makespan, int cpu_number, int process_number){
    process *poped_process;
    double avg_turnaround;
    double avg_overhead;
    int total_turnaround = 0;
    double total_overhead = 0;
    double max_overhead = -1;

    while(is_queue_empty(completed_queue) != 0){
        poped_process = queue_pop(completed_queue);
        int turnaround = poped_process->complete_time -poped_process->arrive_time;
        double overhead = (double)turnaround /poped_process->ori_execution_time;
        if(overhead >max_overhead){
            max_overhead = overhead;
        }
        total_turnaround += turnaround;
        total_overhead += overhead;
        free(poped_process);
        poped_process = NULL;
    }
    avg_turnaround = (double)total_turnaround /process_number;
    avg_overhead = (double)total_overhead /process_number;
    printf("Turnaround time %.0lf\n", ceil(avg_turnaround));
    printf("Time overhead %.2f %.2f\n", max_overhead, avg_overhead);
    printf("Makespan %d", makespan);
}

