#pragma once

#include "sat/problem.h"



namespace testing {

	sat::problem solvable_problem(bool assignent_sgn);

	sat::problem two_solvable_problems(bool assignment_sgn);

	sat::problem random_problem();

}
