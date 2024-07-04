#pragma once
#include <cstdlib>

/**
 * For floating point comparison ensuring the absolute relative error
 * between the numbers is within a given tolerance
 */
///@{
void EXPECT_REL_TOL(const float x1, const float x2, const float tol = 1e-6);
void EXPECT_REL_TOL(const double x1, const double x2, const double tol = 1e-6);
///@}
