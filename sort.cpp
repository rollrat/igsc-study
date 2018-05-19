#include <random>
#include <cmath>
#include <string>
#include <vector>
#include <thread>
#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <chrono>

using namespace std;

class ChronoTimer {
  using timer_type = std::chrono::high_resolution_clock;

  timer_type::time_point begin;
  timer_type::time_point end;
  std::chrono::duration<long double> gap;

public:
  void start() { begin = timer_type::now(); }
  void finish() {
    end = timer_type::now();
    gap = end - begin;
  }
  long double operator*() { return gap.count(); }
};

static mt19937_64 rnd(201805191810ull);
static uint64_t left_bound;
static uint64_t right_bound;

static uint64_t unsorted_array[100000000];
static uint64_t temporary_array[100000000];

uint64_t get_random_number() {
  return rnd() % (right_bound - left_bound + 1) + left_bound;
}

void sort(int a, int b) {
  long len = b - a;

  if (len < 1000000) {
    sort(unsorted_array + a, unsorted_array + b);
    return;
  }

  long mid = a + len / 2;
  thread t1(&sort, a, mid);
  thread t2(&sort, mid, b);
  t1.join();
  t2.join();

  int i = a;
  int j = mid;
  int k = a;

  for (; k < b; k++) {
    if (i < mid) {
      if (j < b) {
        if (unsorted_array[i] <= unsorted_array[j])
          temporary_array[k] = unsorted_array[i++];
        else
          temporary_array[k] = unsorted_array[j++];
      } else
        temporary_array[k] = unsorted_array[i++];
    } else
      temporary_array[k] = unsorted_array[j++];
  }

  memcpy(unsorted_array + a, temporary_array + a, len * 8);
}

int main() 
{
  left_bound = 0;
  right_bound = ~0-1;
  for (int i = 0; i < 100000000; i++) {
    unsorted_array[i] = get_random_number();
  }

  //ChronoTimer CT;
  //CT.start();
  //sort(unsorted_array, unsorted_array + 100000000);
  //CT.finish();
  //printf("%Lf", *CT);

  ChronoTimer CT;
  CT.start();
  sort(0, 100000000);
  CT.finish();
  printf("%Lf", *CT);

  return 0;
}
