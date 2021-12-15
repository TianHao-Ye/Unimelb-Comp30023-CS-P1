#include"process_cpu.h"

process *queue_pop(priority_queue *queue){
    process *poped_process = queue->head;
    /*if there is no node to pop, return null*/
    if(!poped_process){
        return NULL;   
    }
    queue->head = queue->head->next;
    /*if there is no node left after pop*/
    if(!(queue->head)){
        return poped_process;
    }
    /*otherwise there is >=1 nodes left after pop*/
    queue->head->before = NULL;
    poped_process->next = NULL;

    return poped_process;
}

/*return 0 if empty, return 1 if non-empty*/
int is_queue_empty(priority_queue *queue){
    if(!queue->head){
        return 0;
    }
    return 1;
}

int queue_size(priority_queue *queue){
    int size = 0;
    process *p = queue->head;
    while(p){
        size++;
        p = p->next;    
    }
    return size;
}

void queue_traverse(priority_queue *queue){
    process *p = queue->head;
    
    while(p){
        printf("pid= %d\n", p->pid);
        printf("subid= %d\n", p->sub_pid);
        printf("child number= %d\n", p->process_number);
        printf("arrive_time= %d\n", p->arrive_time);
        printf("complete_time= %d\n", p->complete_time);
        printf("execution_time= %d\n", p->execution_time);
        printf("\n");
        p = p->next;    
    }
}

void push_arrive_time_prior(priority_queue **q, process *p){
    priority_queue * queue= *q;
    process *current_c = queue->head;
    /*if queue is empty*/
    if(! queue->head){
        queue->head = p;
        return;
    }
    
    /*if push arrive time < head arrive time, insert before head node*/
    if(p->arrive_time < queue->head->arrive_time){
        queue->head->before = p;
        p->next = queue->head;
        queue->head= p;
    }
    /*if push arrive time == arrive head time, check exe time*/
    else if(queue->head->arrive_time == p->arrive_time){
        /*if push exe time < head exe time, insert before head*/
        if(p->execution_time < queue->head->execution_time){
            queue->head->before = p;
            p->next = queue->head;
            queue->head= p;
        }
        /*else check later nodes exe time, insert in proper position*/
        else{
            /*if next exist and next exe time < p's exe time*/
            while(current_c->next && (p->arrive_time == current_c->next->arrive_time) 
            && (p->execution_time > current_c->next->execution_time)){
                current_c = current_c->next;
            }
            /*if next exist and next pid < p's pid*/
            
            while(current_c->next && (p->arrive_time == current_c->next->arrive_time) 
            && (p->execution_time == current_c->next->execution_time) && (p->pid > current_c->next->pid)){
                current_c = current_c->next;
            }
            
            /*insert after current_c*/
            if(!current_c->next){
                p->before = current_c;
                current_c->next = p;
            }
            else{
                p->next = current_c->next;
                current_c->next->before = p;
                p->before = current_c;
                current_c->next = p;

            }
        }
        
    }
    /*if push arrive time > head time, check later node*/
    else{
        /* find first node of same time*/
        while(current_c->next && (p->arrive_time > current_c->next->arrive_time)){
            current_c = current_c->next;
        }
        /*insert in order of exe time among same execution time*/
        while(current_c->next && (p->arrive_time == current_c->next->arrive_time) 
        && (p->execution_time > current_c->next->execution_time)){
            current_c = current_c->next;
        }
        
        while(current_c->next && (p->arrive_time == current_c->next->arrive_time) 
        && (p->execution_time == current_c->next->execution_time) && (p->pid > current_c->next->pid)){
            current_c = current_c->next;
        }
        
        /*insert after current_c*/
        if(!current_c->next){
            p->before = current_c;
            current_c->next = p;
        }
        else{
            p->next = current_c->next;
            current_c->next->before = p;
            p->before = current_c;
            current_c->next = p;
        }
    }    
}



void push_remaining_time_prior(priority_queue *queue, process *p){
    
    process *current_c = queue->head;
    /*if queue is empty*/
    if(! queue->head){
        queue->head = p;
        return;
    }
    
    /*if push time < head time, insert before head node*/
    if(p->remaining_time < queue->head->remaining_time){
        queue->head->before = p;
        p->next = queue->head;
        queue->head= p;
    }
    /*if push time == head time, check pid*/
    else if(queue->head->remaining_time == p->remaining_time){
        /*if push pid < head pid, insert before head*/
        if(p->pid < queue->head->pid){
            queue->head->before = p;
            p->next = queue->head;
            queue->head= p;
        }
        /*else check later nodes pid, insert in proper position*/
        else{
            /*if next exist and next pid < p's pid*/
            while(current_c->next && (p->remaining_time == current_c->next->remaining_time) 
            && (p->pid > current_c->next->pid)){
                current_c = current_c->next;
            }
            /*insert after current_c*/
            if(!current_c->next){
                p->before = current_c;
                current_c->next = p;
            }
            else{
                p->next = current_c->next;
                current_c->next->before = p;
                p->before = current_c;
                current_c->next = p;

            }
        }
        
    }
    /*if push time > head time, check later node*/
    else{
        /* find first node of same time*/
        while(current_c->next && (p->remaining_time > current_c->next->remaining_time)){
            current_c = current_c->next;
        }
        /*insert in order of pid among same remaining time*/
        while(current_c->next && (p->remaining_time == current_c->next->remaining_time) 
        && (p->pid > current_c->next->pid)){
            current_c = current_c->next;
        }
        /*insert after current_c*/
        if(!current_c->next){
            p->before = current_c;
            current_c->next = p;
        }
        else{
            p->next = current_c->next;
            current_c->next->before = p;
            p->before = current_c;
            current_c->next = p;
        }
    }    
}

void collect_complete_info(priority_queue *finished, priority_queue *completed_queue){
    while(is_queue_empty(finished) != 0){
        int i;
        int last_complete_time =-1;
        process *first_process = queue_pop(finished);
        int child_number = first_process->process_number;
        last_complete_time = first_process->complete_time;
        for(i =0; i<child_number-1; i++){
            process *next_child = queue_pop(finished);
            if(next_child->complete_time>last_complete_time){
                last_complete_time = next_child->complete_time;
            }
            free(next_child);
        }
        first_process->complete_time = last_complete_time;
        first_process->sub_pid = -1;
        push_arrive_time_prior(&completed_queue, first_process);
    }
}


int run_process(cpu *cpu_list, int cpu_number, priority_queue *process_table, priority_queue *completed_queue){
    int i;
    int time = 0;
    int loaded_process_number =0;
    int finished_process_number = 0;
    int last_running_process[cpu_number];

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
    return time;
}

void travsre_print_cpu(cpu *cpu_list, int cpu_number){
    int i;
    for(i =0; i< cpu_number; i++){
        priority_queue *queue =cpu_list[i].queue;
        printf("cpu id: %d\n", cpu_list[i].id);
        if(!(queue->head)){
            printf("status: idle\n");
        }
        else{
            queue_traverse(cpu_list[i].queue);
        }
        printf("total remaining time: %d\n", cpu_list[i].total_remaining_time);
    }
}

int is_all_sub_process_done(process *p, priority_queue *queue){

    return 1;
}

void cpu_do_work(int time, cpu *cpu_list, int cpu_number, int *last_running_process, priority_queue *finished, int loaded_number, int *finished_number){
    int i, j, k;
    process *poped_p;

    /*index 0 finished, index 1 running events*/
    process *events[2][cpu_number];
    for(j =0; j<2; j++){
        for(k=0; k<cpu_number;k++){
            events[j][k] = NULL;
        }
    }
    
    for(i =0; i< cpu_number; i++){
        poped_p = queue_pop(cpu_list[i].queue);
        /*cpu not idle*/
        if(poped_p){
            /*popped (sub)process completed at this second*/
            if(poped_p->remaining_time == 0){
                poped_p->complete_time = time;
                push_remaining_time_prior(finished, poped_p);
                if(is_all_subprocess_completed(poped_p, finished) == 1) {   
                    events[0][i] = poped_p;
                    (*finished_number)++;                 
                }
                /*find next runnable process in this this cpu*/
                process *next_poped_p = queue_pop(cpu_list[i].queue);
                if(next_poped_p){
                    events[1][i] = next_poped_p;
                }
            }
            else{
                events[1][i] = poped_p;
            }           
        }
    }
    
    for(j=0; j<2; j++){
        for(k=0; k<cpu_number;k++){
            if(events[j][k]){
                if(j == 0){
                    printf("%d,%s,pid=%d,proc_remaining=%d\n", time, "FINISHED", events[j][k]->pid, loaded_number-*finished_number);
                    events[j][k] = NULL;
                }
                else{
                    if(events[j][k]->pid != last_running_process[k]){    
                        print_running_info(time, events[j][k], k);
                    }
                    update_runned_process_cpu_info(k, cpu_list, &events[j][k]);
                    last_running_process[k] = events[j][k]->pid;
                    events[j][k] = NULL;
                }
            }
        }
    }
}


/*return 0 if incomplete, return 1 if complete*/
int is_all_subprocess_completed(process *p, priority_queue *finished){
    int completed_num = 0;
    process *temp = finished->head;
    while(temp){
        if(temp->pid == p->pid){
            completed_num++;
        }
        temp = temp->next;
    }
    return completed_num == p->process_number;
}

void print_running_info(int time, process *poped_p, int i){
    printf("%d,%s,", time, "RUNNING");
    if(poped_p->sub_pid < 0){
        printf("pid=%d,", poped_p->pid);
    }
    else{
        printf("pid=%.1f,", poped_p->pid+0.1*poped_p->sub_pid);
    }
    printf("remaining_time=%d,cpu=%d\n", poped_p->remaining_time,i);
}

void update_runned_process_cpu_info(int cpu_index, cpu *working_cpu, process **p){
    (*p)->remaining_time -= 1;
    working_cpu[cpu_index].total_remaining_time -= 1;
    push_remaining_time_prior(working_cpu[cpu_index].queue, (*p));
}

/*return 0 yes, return 1 no*/
int is_all_cpu_finished(cpu *cpu_list, int cpu_number){
    int i;
    for(i =0; i< cpu_number; i++){
        if(is_queue_empty(cpu_list[i].queue) != 0){
            return 1;
        }
    }
    return 0;
}

int find_lowest_workload_cpu(cpu *cpu_list, int cpu_number, int binding_index[]){
    int lowest_index = -1;
    int lowest_time = INT_MAX;
    int i;
    for(i=0; i< cpu_number; i++){
        if((cpu_list[i].total_remaining_time < lowest_time) && (binding_index[i] != 1)){
            lowest_time = cpu_list[i].total_remaining_time;
            lowest_index = i;
        }
    }
    binding_index[lowest_index] = 1;
    return lowest_index;
}

void reset_binding_index(int binding_index[], int cpu_number){
    int i;
    for (i=0; i<cpu_number; i++){
        binding_index[i] =- 0;
    }
}

void load_process(process *p, cpu *cpu_list, int cpu_number){
    /*index indicate whether its cpu is load child already*/
    int binding_index[cpu_number];
    int child_number;
    int cpu_index;
    /*if process non-parallelisable, or there is only one cpu*/
    if(p->parallelisable == 0 || (p->parallelisable == 1 && cpu_number == 1)){
        child_number = 1;
        p->process_number = child_number;
        cpu_index = find_lowest_workload_cpu(cpu_list, cpu_number, binding_index);
        push_remaining_time_prior(cpu_list[cpu_index].queue, p);
        cpu_list[cpu_index].total_remaining_time += p->execution_time;
        reset_binding_index(binding_index, cpu_number);
    }
    /*else the process paralleisable and there are multiple cpus*/
    else{
        child_number = find_child_number(p, cpu_number);
        int i;
        int *process_data = get_process_data(p);       
        for(i=0; i< child_number; i++){
            process *child_process = construct_new_process(process_data, child_number);
            child_process->sub_pid = i;
            child_process->process_number = child_number;
            cpu_index = find_lowest_workload_cpu(cpu_list, cpu_number, binding_index);
            push_remaining_time_prior(cpu_list[cpu_index].queue, child_process);
            cpu_list[cpu_index].total_remaining_time += child_process->execution_time;
        }
        reset_binding_index(binding_index, cpu_number);
        free(process_data);
        free(p);
    }
}

int find_child_number(process *p, int cpu_number){
    int k =1;
    while((((double)p->execution_time/k) >=1) && k <=cpu_number){
        k++;
    }
    k--;
    return k;
}

process *get_head_process(priority_queue *queue){
    process *p =queue->head;
    if(!p){
        return NULL;
    }
    return p;
}

/*return 0 if there is a process arriving, return 1 when not*/
int any_process_arriving(int time, priority_queue *queue){
    process *head_process = get_head_process(queue);
    /*if no process in process table*/
    if(! head_process){
        return 1; 
    }
    /*if head arrive time equal to current time*/
    if(head_process->arrive_time == time){
        return 0;
    }
    /*still have processes in process table but not arriing*/
    return 1;
}

void initialize_cpu(cpu *cpu_list, int cpu_number){
    int i =0;

    for(i = 0; i < cpu_number; i++){
        cpu_list[i].id = i;
        cpu_list[i].queue = (priority_queue *)malloc(sizeof(priority_queue));
        assert(cpu_list[i].queue);
        cpu_list[i].queue->head = NULL;
        cpu_list[i].total_remaining_time = 0;
    }
}

int *get_process_data(process *p){
    int *process_data = (int *)malloc(4 *sizeof(int));
    assert(process_data);
    process_data[0] = p->arrive_time;
    process_data[1] = p->pid;
    process_data[2] = p->execution_time;
    process_data[3] = p->parallelisable;
    return process_data;
}

process *construct_new_process(int process_data[], int child_number){
    process * new_node = (process *)malloc(sizeof(process));
    assert(new_node);
    
    new_node->arrive_time = process_data[0];
    new_node->pid = process_data[1];
    new_node->sub_pid = -1;
    new_node->ori_execution_time = process_data[2];
    if(child_number == 1){
        new_node->execution_time = process_data[2];
    }
    else{
        new_node->execution_time = (int)ceil(((double)process_data[2] /child_number)+1);
    }
    new_node->remaining_time = new_node->execution_time;
    new_node->parallelisable = process_data[3];
    new_node->complete_time = -1;
    new_node->process_number = -1;
    new_node->next = NULL;
    new_node->before = NULL;
    
    return new_node;
}