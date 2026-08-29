#include "buildhistogram.h"


BuildHistogram::BuildHistogram(size_t step_mhz, double similary, double start_size_window_part_stan_dev)
    : step_mhz_(step_mhz),
      similary_(similary),
      start_size_window_part_stan_dev_(start_size_window_part_stan_dev)
{}
