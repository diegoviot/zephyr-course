#include <zephyr/shell/shell.h>

static int cmd_channel_get_handler(const struct shell* sh, int argc, char** argv)
{
    shell_info(sh, "Hello from cmd_channel_get_handler");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(iomico_myled_subcmd,
    SHELL_CMD_ARG(channel_get, NULL, "Get channel", cmd_channel_get_handler, 1, 0),
    SHELL_SUBCMD_SET_END,
);

SHELL_CMD_REGISTER(iomico_myled, &iomico_myled_subcmd, "Driver commands", NULL);
