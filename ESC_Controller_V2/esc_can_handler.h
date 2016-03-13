#ifndef ESC_CAN_HANDLER_H
#define ESC_CAN_HANDLER_H

//starts can bus with correct filter settings for motor
void esc_can_init(void);

//functions for sending values over can bus
void send_speed(uint16_t val);
void send_current(uint16_t val);
void set_error(bool stall, bool overcurrent);

//functions for reading values from can bus
uint16_t get_throttle_val(void);
uint8_t get_brake_val(void);

//actual function for reading can bus and dealing with different message id's
void read_can_bus(void);

#endif
