#include <stdbool.h>
#include "platform.h"

bool full(bool *array, int n, int edges)
{
  for (int i = 0; i < edges; i++)
    if (array[i] == 0)
      return false;
  return true;
}

void next(bool *array, int n)
{
  int last_true = n - 1;
  while (!array[last_true])
    last_true--;
  int to_increment = last_true;
  while (array[to_increment - 1])
    to_increment--;
  int num_to_move_back = last_true - to_increment;
  array[to_increment - 1] = 1;
  for (int i = to_increment; i < n - num_to_move_back; i++)
    array[i] = 0;
  for (int i = n - num_to_move_back; i < n; i++)
    array[i] = 1;
}

platform new_platform(int n)
{
  platform new_p;
  new_p.n = n;
  new_p.min_m = n - 1;
  new_p.max_m = n * MAX_K / 2;
}

void extend_platform(platform *old_p, platform *new_p)
{
  int num_to_extend = max_m - min_m + 1;
  for (int i = 0; i < num_to_extend; i++)
    extend_rc(old_p->optimal_constructions[i], new_p);
}

void extend_rc(road_construction *rc, platform *new_p)
{
  for (int edges = 0; edges < MAX_K; edges++)
  {
    bool *roads = malloc(rc->n * sizeof(bool));
    for (int i = rc->n - MAX_K; i < rc->n; i++)
    {
      do
      {
        next(roads, rc->n);
     
        road_construction new_rc;
        new_rc.n = rc->n + 1;
        new_rc.m = rc->m + edges;
        new_rc.degree = malloc((rc->n + 1) * sizeof(int));
        for (int i = 0; i < rc->n; i++)
          new_rc.degree[i] = rc->degree[i];
        new_rc.degree[rc->n] = edges;
        new_rc.roads = malloc((rc->n + 1) * (rc->n + 1) * sizeof(int));
        for (int i = 0; i < rc->n; i++)
          for (int j = 0; j < i; j++)
            new_rc.roads[rc->n * i + j] = new_rc.roads[rc->n * j + i] 
              = rc->roads[rc->n * i + j];
        for (int i = 0; i < rc->n; i++)
          if (roads[i])
          {
            new_rc.roads[(rc->n + 1) * rc->n + i] = 
              new_rc.roads[(rc->n + 1)*i + rc->n] = 1;
            new_rc.degree[i]++;
          }
          
        // if maximum degree exceeded for some building, skip this combination 
        // of road construction
        bool max_k_exceeded = false;
        for (int i = 0; i < rc->n; i++)
          if (new_rc.degree[i] > MAX_K)
          {
            free(new_rc.degree);
            free(new_rc.roads);
            max_k_exceeded = true;
            break;
          }
        if (max_k_exceeded)
          continue;
        
        
        // TODO: calculate optimality here
        
        add_rc(new_rc, new_p);
      }
      while (!full(roads, rc->n, edges));
    }
    free(roads);
  }
}

void add_rc(road_construction rc, platform *new_p)
{
  int index = rc.m - new_p->n;
  if (new_p->optimal_constructions[index].optimality > rc.optimality)
  {
    free(new_p->optimal_constructions[index].degree);
    free(new_p->optimal_constructions[index].roads);
    new_p->optimal_constructions[index] = rc;
  }
  else 
  {
    free(rc.degree);
    free(rc.roads);
  }
}
