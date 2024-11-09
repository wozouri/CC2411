#include "run.h"

int main()
{
    lcd_init();
    touch_init();
    mmap_init();
    ledORbee_init();

    Main();

    close(lcd_fd);
    close(touch_fd);
    munmap(p, N * M * 4);
    close(led_fd);
    close(bee_fd);

    return 0;
}
