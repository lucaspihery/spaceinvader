#ifndef PUSH_BUTTON_H_
#define PUSH_BUTTON_H_

void push_button_init(void);
uint32_t push_button_is_pressed(void);

/* for interruptions */
uint32_t push_button_it_pending(void);
#define push_button_it_handler EXTI0_IRQHandler
void push_button_it_handler(void);

#endif /* PUSH_BUTTON_H_ */
