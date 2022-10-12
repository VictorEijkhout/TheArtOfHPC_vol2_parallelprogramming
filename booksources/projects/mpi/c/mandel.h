#ifndef MANDEL_H
#define MANDEL_H 1

#include <mpi.h>

struct coordinate { double x,y; } ;
int belongs(struct coordinate xy,int itbound);

class circle {
public:
  double step; int infty;
private:
  double xmin,xmax,ymin,ymax, x,y;
 public :
  circle(double stp,int bound);
  void next_coordinate(struct coordinate& xy);
  int is_valid_coordinate(struct coordinate xy);
  void invalid_coordinate(struct coordinate& xy);
};

class queue {
protected:
  int ntids,total_tasks,t_start,t_stop;
  MPI_Comm comm; circle *workcircle;
  double area;
public:
  queue(MPI_Comm queue_comm,circle *disccircle) {
    comm = queue_comm; workcircle = disccircle;
    total_tasks = 0;
    MPI_Comm_size(comm,&ntids);
    area = 0.;
    t_start = MPI_Wtime();
  };
  void addtask(struct coordinate xy);
  void complete();
  void wait_for_work(MPI_Comm,circle*);
};

void scan_circle(queue *taskqueue);

#endif
