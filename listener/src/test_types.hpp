#pragma once

#include "boost/date_time/posix_time/posix_time.hpp"

#include <sstream>

struct test_parameters {
    std::size_t subjects_count;
    std::size_t observers_count;
};

struct test_result {
    boost::posix_time::time_duration build;
    boost::posix_time::time_duration call;
    boost::posix_time::time_duration clean;
};
