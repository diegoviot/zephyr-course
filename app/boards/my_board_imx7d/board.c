
#include <zephyr/init.h>
#include <zephyr/kernel.h>

static int board_my_board(void)
{
	printk("Board Initialized! %s\n", CONFIG_BOARD_TARGET);

	return 0;
}

SYS_INIT(board_my_board, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
