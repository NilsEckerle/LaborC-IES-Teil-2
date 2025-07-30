/**
 * @file math_helper_functions.h
 * @brief helper functions for math calculations
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef MATH_HELPER_FUNCTIONS_H
#define MATH_HELPER_FUNCTIONS_H

/**
 * @brief calculates the period in milli seconds
 * @param HZ - in HZ how often it should appear
 */
#define CALCULATE_PERIOD_MS(HZ) \
	(1000/HZ)

#endif // !MATH_HELPER_FUNCTIONS_H
