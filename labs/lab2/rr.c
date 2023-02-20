#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process
{
  u32 pid;
  u32 arrival_time;
  u32 burst_time;

  TAILQ_ENTRY(process) pointers;

  /* Additional fields here */
  u32 remaining_time;
  u32 response_time;
  bool processed;
  /* End of "Additional fields here" */
};

TAILQ_HEAD(process_list, process);

u32 next_int(const char **data, const char *data_end)
{
  u32 current = 0;
  bool started = false;
  while (*data != data_end)
  {
    char c = **data;

    if (c < 0x30 || c > 0x39)
    {
      if (started)
      {
        return current;
      }
    }
    else
    {
      if (!started)
      {
        current = (c - 0x30);
        started = true;
      }
      else
      {
        current *= 10;
        current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

u32 next_int_from_c_str(const char *data)
{
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++]))
  {
    if (c < 0x30 || c > 0x39)
    {
      exit(EINVAL);
    }
    if (!started)
    {
      current = (c - 0x30);
      started = true;
    }
    else
    {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    int err = errno;
    perror("open");
    exit(err);
  }

  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED)
  {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  const char *data_end = data_start + size;
  const char *data = data_start;

  *process_size = next_int(&data, data_end);

  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL)
  {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  for (u32 i = 0; i < *process_size; ++i)
  {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
  }

  munmap((void *)data, size);
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    return EINVAL;
  }
  struct process *data;
  u32 size;
  init_processes(argv[1], &data, &size);

  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list;
  TAILQ_INIT(&list);

  u32 total_waiting_time = 0;
  u32 total_response_time = 0;

  /* Your code here */
  /*
  struct process *current_process = &data[0];
  TAILQ_INSERT_TAIL(&list, current_process, pointers);
  struct process *p;
  TAILQ_FOREACH(p, &list, pointers){
    printf("process %d is in the queue\n", p->pid);
  }
  TAILQ_REMOVE(&list, current_process, pointers);
  TAILQ_FOREACH(p, &list, pointers){
    printf("process %d is in the queue\n", p->pid);
  }
  if (TAILQ_EMPTY(&list)){
    printf("empty");
  }
  */
  struct process* current_process;
  for (u32 i = 0; i < size; i++){
    current_process = &data[i];
    //printf("Burst time of process %d is %.2f\n", current_process->pid, (float)current_process->burst_time);
    current_process->remaining_time = current_process->burst_time;
  }
  
  u32 current_time = 0;
  bool if_finished = false;
  u32 remaining_quantum = quantum_length;
  while(!if_finished){
    for (u32 i = 0; i < size; i++){
      current_process = &data[i];
      if (current_process->arrival_time == current_time){
        TAILQ_INSERT_TAIL(&list, current_process, pointers);
        current_process->response_time = 0;
        current_process->processed = false;
        struct process *p;
        TAILQ_FOREACH(p, &list, pointers){
          //printf("at time %d, process %d is in the queue\n", current_time, p->pid);
        }
        
      }
    }
    struct process *firstProcessOnQueue = TAILQ_FIRST(&list);
    if (!firstProcessOnQueue->processed){
      firstProcessOnQueue->processed = true;
      total_response_time += firstProcessOnQueue->response_time;
    }
    if (firstProcessOnQueue->remaining_time == 0 && remaining_quantum == 0){
      //printf("at time %d, process %d ended\n", current_time, firstProcessOnQueue->pid);
      
      TAILQ_REMOVE(&list, firstProcessOnQueue, pointers);
      if (!TAILQ_EMPTY(&list)){
        firstProcessOnQueue = TAILQ_FIRST(&list);
        if (!firstProcessOnQueue->processed){
          firstProcessOnQueue->processed = true;
          total_response_time += firstProcessOnQueue->response_time;
        }
        remaining_quantum = quantum_length;
        firstProcessOnQueue->remaining_time--;
        remaining_quantum--;
        struct process *p;
        TAILQ_FOREACH(p, &list, pointers){
          total_waiting_time++;
        } 
        total_waiting_time--;
        //printf("process %d's remaining time: %d\n", firstProcessOnQueue->pid, firstProcessOnQueue->remaining_time);
        //printf("process %d's remaining quantum time: %d\n", firstProcessOnQueue->pid, remaining_quantum);

      }
        
    }else if (remaining_quantum != 0 && firstProcessOnQueue->remaining_time != 0){
      
      firstProcessOnQueue->remaining_time--;
      remaining_quantum--;
      struct process *p;
      TAILQ_FOREACH(p, &list, pointers){
        total_waiting_time++;
      }
      total_waiting_time--;
      //printf("process %d's remaining time: %d\n", firstProcessOnQueue->pid, firstProcessOnQueue->remaining_time);
      //printf("process %d's remaining quantum time: %d\n", firstProcessOnQueue->pid, remaining_quantum);
      

    }else if (remaining_quantum == 0){
      //printf("at time %d, process %d preempted\n", current_time, firstProcessOnQueue->pid);
      TAILQ_REMOVE(&list, firstProcessOnQueue, pointers);
      TAILQ_INSERT_TAIL(&list, firstProcessOnQueue, pointers);
      firstProcessOnQueue = TAILQ_FIRST(&list);
      if (!firstProcessOnQueue->processed){
        firstProcessOnQueue->processed = true;
        total_response_time += firstProcessOnQueue->response_time;
      }
      remaining_quantum = quantum_length;
      firstProcessOnQueue->remaining_time--;
      remaining_quantum--;
      struct process *p;
      TAILQ_FOREACH(p, &list, pointers){
        total_waiting_time++;
      }
      total_waiting_time--;
      //printf("process %d's remaining time: %d\n", firstProcessOnQueue->pid, firstProcessOnQueue->remaining_time);
      //printf("process %d's remaining quantum time: %d\n", firstProcessOnQueue->pid, remaining_quantum);
    }
    
    else if (firstProcessOnQueue->remaining_time == 0){
      //printf("at time %d, process %d ended\n", current_time, firstProcessOnQueue->pid);
      
      TAILQ_REMOVE(&list, firstProcessOnQueue, pointers);
      if (!TAILQ_EMPTY(&list)){
        firstProcessOnQueue = TAILQ_FIRST(&list);
        if (!firstProcessOnQueue->processed){
          firstProcessOnQueue->processed = true;
          total_response_time += firstProcessOnQueue->response_time;
        }
        remaining_quantum = quantum_length;
        firstProcessOnQueue->remaining_time--;
        remaining_quantum--;
        struct process *p;
        TAILQ_FOREACH(p, &list, pointers){
          total_waiting_time++;
        } 
        total_waiting_time--;
        //printf("process %d's remaining time: %d\n", firstProcessOnQueue->pid, firstProcessOnQueue->remaining_time);
        //printf("process %d's remaining quantum time: %d\n", firstProcessOnQueue->pid, remaining_quantum);

      }
        
    }
    
    /*
    struct process *current_pointer;
    struct process *next_pointer;
    for (current_pointer = TAILQ_FIRST(&list); current_pointer != NULL; current_pointer = next_pointer){
      next_pointer = TAILQ_NEXT(current_pointer, pointers)
    }*/
    
    
    //printf("at time %d, process %d is at the head\n", current_time, firstProcessOnQueue->pid);
    
    
    
    
    /*struct process *current_element;
    struct process *next_element;
    


    for (current_element = TAILQ_FIRST(&list); current_element != NULL; current_element = next_element){
      next_element = TAILQ_NEXT(current_element, pointers);
      printf("at time %d, process %d is in the queue\n", current_time, current_element->pid);
    }*/
    
    /*
    struct process *p;
    TAILQ_FOREACH(p, &list, pointers){
      total_waiting_time++;
    }
    total_waiting_time--;*/

    struct process *p;
    TAILQ_FOREACH(p, &list, pointers){
      if (!p->processed){
        p->response_time++;
      }
    } 

    if (TAILQ_EMPTY(&list)){
      if_finished = true;
      //printf("queue empty\n");
    }
    current_time++;
  }
 


  /* End of "Your code here" */

  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}
