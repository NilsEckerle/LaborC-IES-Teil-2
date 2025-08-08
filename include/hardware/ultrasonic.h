/**
 * @file ultrasonic.h
 * @brief ultrasonic controll module
 * @author Nils Eckerle
 * @date 2025-08-08
 */

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <avr/io.h>
#include <stdint.h>

#define SERVO_DDR DDRD
#define SERVO_PORT PORTD
#define SERVO_PIN PIND
#define SERVO_BIT 3

#define SERVO_PWM_TIMER_TCCRA TCCR2A
#define SERVO_PWM_TIMER_TCCRB TCCR2B
#define SERVO_PWM_TIMER_COMB1 COM2B1
#define SERVO_PWM_TIMER_WGM1 WGM21
#define SERVO_PWM_TIMER_WGM0 WGM20
#define SERVO_PWM_TIMER_CS2 CS22
#define SERVO_PWM_TIMER_CS1 CS21
#define SERVO_PWM_TIMER_CS0 CS20
#define SERVO_PWM_TIMER_OCRB OCR2B

typedef enum servo_rotation {
  SERVO_rotation_left = 15,
  SERVO_rotation_half_left = 19,
  SERVO_rotation_front = 24,
  SERVO_rotation_half_right = 28,
  SERVO_rotation_right = 32
} e_servo_rotation;

void SONIC_rotation(e_servo_rotation e_rotation);

void _SONIC_init_sonic();

void _SONIC_init_servo();

void SONIC_init();

#endif // !ULTRASONIC_H
