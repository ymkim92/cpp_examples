#include <stdio.h>
#include "lwrb.h"

void test_init()
{
    lwrb_t rb;
    uint8_t buffer[5];
    uint8_t *ret;
    uint8_t ret_size;

    uint8_t wbuf[] = "123";
    printf("Address of buffer: %p\n", buffer);
    lwrb_init(&rb, buffer, 5);
    lwrb_write(&rb, wbuf, 3);

    ret = lwrb_get_linear_block_read_address(&rb);
    ret_size = lwrb_get_linear_block_read_length(&rb);
    printf("Address of ret: %p, size: %d\n", ret, ret_size);
    printf("------------------------\n");
}

void test_turnaround()
{
    lwrb_t rb;
    uint8_t tx_buffer[5];
    uint8_t rx_buffer[5];
    uint8_t *ret;
    uint8_t ret_size;

    uint8_t wbuf[] = "123";
    printf("Address of buffer: %p\n", tx_buffer);
    lwrb_init(&rb, tx_buffer, 5);
    lwrb_write(&rb, wbuf, 3);
    // 1/2/3/./.
    // ^r    ^w
    lwrb_read(&rb, rx_buffer, rb.size);
    // ././././.
    //       ^rw
    lwrb_write(&rb, wbuf, 3);
    // 3/././1/2
    //   ^w  ^r

    printf("Address of rb.buff: %p\n", rb.buff);
    ret = lwrb_get_linear_block_read_address(&rb);
    ret_size = lwrb_get_linear_block_read_length(&rb);
    printf("Address of ret: %p, size: %d\n", ret, ret_size);

    lwrb_read(&rb, rx_buffer, ret_size);
    // 3/./././.
    // ^r^w  
    ret = lwrb_get_linear_block_read_address(&rb);
    ret_size = lwrb_get_linear_block_read_length(&rb);
    printf("Address of ret: %p, size: %d\n", ret, ret_size);
    printf("------------------------\n");
}

void test_turnaround2()
{
    lwrb_t rb;
    uint8_t tx_buffer[5];
    uint8_t rx_buffer[5];
    uint8_t *ret;
    uint8_t ret_size;

    uint8_t wbuf[] = "123";
    printf("Address of buffer: %p\n", tx_buffer);
    lwrb_init(&rb, tx_buffer, 5);
    lwrb_write(&rb, wbuf, 3);
    // 1/2/3/./.
    // ^r    ^w
    lwrb_read(&rb, rx_buffer, rb.size);
    // ././././.
    //       ^rw
    lwrb_write(&rb, wbuf, 3);
    // 3/././1/2
    //   ^w  ^r

    lwrb_read(&rb, rx_buffer, rb.size);
    // ././././.
    //   ^rw  
    ret = lwrb_get_linear_block_read_address(&rb);
    ret_size = lwrb_get_linear_block_read_length(&rb);
    printf("Address of ret: %p, size: %d\n", ret, ret_size);
    printf("------------------------\n");
}

int main()
{
    test_init();
    test_turnaround();
    test_turnaround2();
    return 0;
}