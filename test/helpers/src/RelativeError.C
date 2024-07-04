#include "RelativeError.h"
#include "gtest/gtest.h"

void
EXPECT_REL_TOL(const float x1, const float x2, const float tol)
{
  EXPECT_NEAR(std::abs(x1 - x2) / x2, 0.0, tol);
}

void
EXPECT_REL_TOL(const double x1, const double x2, const double tol)
{
  EXPECT_NEAR(std::abs(x1 - x2) / x2, 0.0, tol);
}
