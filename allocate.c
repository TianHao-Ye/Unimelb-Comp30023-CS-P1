#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>
#include<assert.h>
#include"util.h"



int main(int argc, char * argv[]){

    /*read in command line arguments*/
    inputList *input_list = read_in_arguments(argc, argv);

    /*read in process info*/
    FILE *file;
    file = fopen(input_list->file_name, "r");
    /*create process table, read process in from file*/
    priority_queue *process_table = (priority_queue *)malloc(sizeof(priority_queue));
    assert(process_table);
    process_table->head = NULL;
    int process_number = read_process(file, &process_table);
    fclose(file);
    
    /*initialize cpu list*/
    int cpu_number = input_list->number_of_processor;
    free(input_list);

    cpu *cpu_list = (cpu *)malloc(sizeof(cpu)*cpu_number);
    initialize_cpu(cpu_list, cpu_number);
    

    /*create completed table*/
    priority_queue *completed_queue = (priority_queue *)malloc(sizeof(priority_queue));
    assert(completed_queue);
    completed_queue->head = NULL;

    /*run process*/
    /*int time = run_process(cpu_list, cpu_number, process_table, completed_queue);*/
    
    int i;
    int time = 0;
    int loaded_process_number =0;
    int finished_process_number = 0;
    int *last_running_process = (int *)malloc(sizeof(int)*cpu_number);
    assert(last_running_process);
    for(i=0; i<cpu_number; i++){
        last_running_process[i] = -1;
    }

    /*create finished table*/
    priority_queue *finished_queue = (priority_queue *)malloc(sizeof(priority_queue));
    assert(finished_queue);
    finished_queue->head = NULL;
    
    /*when there still process not arrived yet, process table not empty*/
    while((is_queue_empty(process_table) != 0) || (is_all_cpu_finished(cpu_list, cpu_number) !=0)){
        /*at this second, loop when there is a procees to arrive*/
        while(any_process_arriving(time, process_table) == 0){
            process *arriving_process = queue_pop(process_table);
            load_process(arriving_process, cpu_list, cpu_number);
            loaded_process_number++;
        }
        
        cpu_do_work(time, cpu_list, cpu_number, last_running_process, finished_queue, loaded_process_number, &finished_process_number);
        time++;
    }

    time--;
    collect_complete_info(finished_queue, completed_queue);
    free(finished_queue);
    free(last_running_process);
    /*print out performance statistics*/
    print_performance_statistics(completed_queue, time, cpu_number, process_number);

    free_cpu(cpu_list, cpu_number);
    free(cpu_list);
    cpu_list = NULL;
    free(process_table);
    process_table = NULL;
    free(completed_queue);
    completed_queue = NULL;
    return 0;
}

