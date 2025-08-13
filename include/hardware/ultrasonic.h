/**
 * @file ultrasonic.h
 * @brief ultrasonic control module with HC-SR04 sensor
 * @author Nils Eckerle
 * @date 2025-08-08
 */
#ifndef ULTRASONIC_H
#define ULTRASONIC_H
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

// Servo definitions (unchanged)
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

// HC-SR04 definitions
#define SONIC_TRIG_DDR DDRB
#define SONIC_TRIG_PORT PORTB
#define SONIC_TRIG_PIN PINB
#define SONIC_TRIG_BIT 5

#define SONIC_ECHO_DDR DDRB
#define SONIC_ECHO_PORT PORTB
#define SONIC_ECHO_PIN PINB
#define SONIC_ECHO_BIT 4

typedef enum servo_rotation {
  SERVO_rotation_left = 15,
  SERVO_rotation_half_left = 19,
  SERVO_rotation_front = 24,
  SERVO_rotation_half_right = 28,
  SERVO_rotation_right = 32
} e_servo_rotation;

void SONIC_init_sonic(void);
void SONIC_init_servo(void);
void SONIC_init(void);

void SONIC_rotation(e_servo_rotation e_rotation);

#endif // !ULTRASONIC_H
