#include "hardware/ultrasonic.h"
#include "tools/bit_functions.h"

void SONIC_rotation(e_servo_rotation e_rotation) {
  SERVO_PWM_TIMER_OCRB = e_rotation;
}

void _SONIC_init_sonic() {

}

void _SONIC_init_servo() {
  SET_BIT(SERVO_DDR, SERVO_BIT); // set output

  // TCCRxA
  SET_BIT(SERVO_PWM_TIMER_TCCRA, SERVO_PWM_TIMER_COMB1); // clear OCxB on compare match
  // Fast PWM
  SET_BIT(SERVO_PWM_TIMER_TCCRA, SERVO_PWM_TIMER_WGM1);
  SET_BIT(SERVO_PWM_TIMER_TCCRA, SERVO_PWM_TIMER_WGM0);

  // TCCRxB
  // clk_I/O / 1024 (From prescaler)
  // Frequenz 16e6 / 1024 = 15625 Hz
  SET_BIT(SERVO_PWM_TIMER_TCCRB, SERVO_PWM_TIMER_CS2);
  SET_BIT(SERVO_PWM_TIMER_TCCRB, SERVO_PWM_TIMER_CS1);
  SET_BIT(SERVO_PWM_TIMER_TCCRB, SERVO_PWM_TIMER_CS0);
}

void SONIC_init() {
  _SONIC_init_servo();
  _SONIC_init_sonic();
}
