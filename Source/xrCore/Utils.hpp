#pragma once

#if !defined(_DEBUG) && defined(_WIN64) 
#define FOR_START(type, start, finish, counter)\
tbb::parallel_for(tbb::blocked_range<type>(start, finish), [&](const tbb::blocked_range<type>& range) {\
    for (type counter = range.begin(); counter != range.end(); ++counter)
#else
#define FOR_START(type, start, finish, counter)\
    {\
        for (type counter = 0; counter < finish; ++counter)
#endif

#if !defined(_DEBUG) && defined(_WIN64) 
#define FOR_END });
#else
#define FOR_END }
#endif
#define ACCELERATED_SORT tbb::parallel_sort 