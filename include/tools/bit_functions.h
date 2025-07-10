#ifndef BIT_FUNCTIONS
#define BIT_FUNCTIONS

/**
 * @brief Sets a specific bit in a register to 1
 * 
 * @param reg The register or variable to modify
 * @param bit The bit position to set (0-based indexing, 0 = LSB)
 * 
 * @note This is implemented as a do-while(0) macro to ensure proper
 *       statement behavior when used in conditional statements.
 *       The macro is safe to use in all contexts where a single
 *       statement is expected.
 * 
 * @example
 *       uint8_t status = 0x00;  // 00000000
 *       SET_BIT(status, 3);     // Sets bit 3: 00001000 (0x08)
 */
#define SET_BIT(reg, bit) do {reg |= (1 << bit);} while(0)

/**
 * @brief Clears a specific bit in a register to 0
 * 
 * @param reg The register or variable to modify
 * @param bit The bit position to clear (0-based indexing, 0 = LSB)
 * 
 * @note This is implemented as a do-while(0) macro to ensure proper
 *       statement behavior when used in conditional statements.
 *       The macro is safe to use in all contexts where a single
 *       statement is expected.
 * 
 * @example
 *       uint8_t status = 0xFF;    // 11111111
 *       UNSET_BIT(status, 3);     // Clears bit 3: 11110111 (0xF7)
 */
#define UNSET_BIT(reg, bit) do {reg &= ~(1 << bit);} while(0)

#endif // !BIT_FUNCTIONS
