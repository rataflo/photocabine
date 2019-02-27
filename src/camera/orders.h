#ifndef orders_h
#define ordes_h

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
#define ORDER_SWUP 'H' // Test on upper switch of spider
#define ORDER_SWDOWN 'I' // Test on upper switch of spider
#define ORDER_SWROTPAIR 'J' // Test on rotation opto switch 1
#define ORDER_SWROTIMPAIR 'K' // Test on rotation opto switch 2
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

#endif
