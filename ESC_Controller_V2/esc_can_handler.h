#ifndef ESC_CAN_HANDLER_H
#define ESC_CAN_HANDLER_H

void esc_can_init(void);
void send_speed(uint16_t val);
void send_current(uint16_t val);


#endif
