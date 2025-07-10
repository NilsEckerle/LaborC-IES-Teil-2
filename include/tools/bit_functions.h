#ifndef BIT_FUNCTIONS
#define BIT_FUNCTIONS

#define SET_BIT(reg, bit) do {reg |= (1 << bit);} while(0)
#define UNSET_BIT(reg, bit) do {reg &= ~(1 << bit);} while(0)

#endif // !BIT_FUNCTIONS
