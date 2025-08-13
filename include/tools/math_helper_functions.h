/**
 * @file math_helper_functions.h
 * @brief helper functions for math calculations
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef MATH_HELPER_FUNCTIONS_H
#define MATH_HELPER_FUNCTIONS_H

/**
 * @brief Calculates the period in milliseconds from frequency
 * @param HZ Frequency in Hertz
 * @return Period in milliseconds
 */
#define CALCULATE_PERIOD_MS(HZ) (1000 / HZ)

/**
 * @brief Calculates a percentage of a maximum value
 * @param MAX Maximum value to calculate percentage of
 * @param PERCENT Percentage value (0-100)
 * @return Calculated percentage value with same type as MAX
 */
#define CALCULATE_PERCENT(MAX, PERCENT) (typeof(MAX))(MAX * PERCENT / 100)

#define ABS(X) (((X)<0) ? -(X) : (X))

#define MIN(X, Y) (((X)<(Y)) ? (X) : (Y))

#define MAX(X, Y) (((X)>(Y)) ? (X) : (Y))

#endif  // !MATH_HELPER_FUNCTIONS_H
