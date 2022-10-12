#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "mandel.h"

class serialqueue : public queue {
private :
  int free_processor;
public :
  serialqueue(MPI_Comm queue_comm,circle *workcircle)
    : queue(queue_comm,workcircle) {
    free_processor=0;
  };
  void addtask(struct coordinate xy) {
    MPI_Status status; double contribution;

    MPI_Send(&xy,2,MPI_DOUBLE, 
	     free_processor,0,comm);
    MPI_Recv(&contribution,1,MPI_DOUBLE,
	     free_processor,0,comm, &status);
    area += contribution;
    total_tasks++;
    free_processor++;
    if (free_processor==ntids-1)
      // wrap around to the first again
      free_processor = 0;
  };
  void complete(circle *workcircle) { 
    struct coordinate xy;
    workcircle->invalid_coordinate(xy); free_processor=0;
    for (int p=0; p<ntids-1; p++)
      addtask(xy);
    t_stop = MPI_Wtime();
    printf("Area computed: %e\n by %d tasks in time %d\n",
	   area,total_tasks,t_stop-t_start);
    return; 
  };
};

int main(int argc,char **argv) {
  MPI_Comm comm;
  int ntids,mytid;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&ntids);
  MPI_Comm_rank(comm,&mytid);

  {
    circle *workcircle = new circle(.003,10000);
    serialqueue *taskqueue = new serialqueue(comm,workcircle);
    if (mytid==ntids-1)  {
      for (;;) {
	struct coordinate xy;
	workcircle->next_coordinate(xy);
	if (workcircle->is_valid_coordinate(xy))
	  taskqueue->addtask(xy);
	else break;
      }
      taskqueue->complete(workcircle);
    } else
      taskqueue->wait_for_work(comm,workcircle);
  }

  MPI_Finalize();
  return 0;
}
