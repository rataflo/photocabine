#ifndef orders_h
#define ordes_h

// Timeout
#define TIMEOUT_ORDER 1000

// ORDERS LIST
#define NO_ORDER '0'
#define RESPONSE_OK 'A'
#define ORDER_NEW_SLOT 'B'  // When camera need to process paper.
#define ORDER_PAPER_READY 'C' // When paper is in a slot ready to be processed.
#define ORDER_NB_FREE_SLOT 'D' // Get number of free slot.
#define ORDER_SPIDER_READY 'E' // Ask if spider complete her init and everything is ok.


// Test orders
#define ENTER_TEST 'F' // Enter test mode or pause the process.
#define EXIT_TEST 'G'
#define ORDER_UPSPIDER 'L' // Spider to the top
#define ORDER_DOWNSPIDER 'M' // Spider to the bottom
#define ORDER_ROTATESPIDER 'N' // Rotate spider.
#define ORDER_OPENARM 'O' // Open arm
#define ORDER_CLOSEARM 'P' // Close arm
#define ORDER_IDLEARM 'Q' // Put the arm on idle pos.
#define ORDER_DELIVERYRUN 'R' // Run the delivery indefinitely.

#define ORDER_PAUSE 'S'
#define ORDER_RESUME 'T'

// Ask infos
#define ORDER_TEMP 'U'
#define ORDER_NBSTRIP 'V'
#define ORDER_TOTCENT 'W'
#define ORDER_MODE 'X'
#define ORDER_GET_STATUS 'Y'
#define RESPONSE_STATUS_RUNNING 'Z'
#define RESPONSE_STATUS_PAUSE 'a'
#define RESPONSE_STATUS_TEST 'b'

// Modify param
#define ORDER_SET_MODE 'c'

#define ORDER_SET_TANK_TIME 'd'
#define ORDER_SET_PRICE 'e'
#define ORDER_SET_FREE_PRICE 'f'

#define ORDER_TAKE_SHOT 'g'

#define ORDER_SHUTTER 'h'
#define ORDER_SCISSOR 'i'
#define ORDER_PAPER_FEEDER 'j'

#define ORDER_FLASH 'r'
#define ORDER_START_LIGHT 's'
#define ORDER_LEDSTRIP 't'

#define ORDER_GET_PRICE 'u'
#define ORDER_GET_FREE_PRICE 'v'
#define ORDER_GET_TANK_TIME 'w'

#define ORDER_NEW_SLOT_READY 'x'  // When slot ready to accept paper
#define ORDER_TRUE '1'
#define ORDER_FALSE '2'
#endif
