#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>
#include <glob.h>

int main(int argc, char *argv[]) {
    const char* gpiolineName = NULL; // "SODIMM_55" - Example of pin A0 on Aster Carrier Board;
    const char* gpioChip = NULL; // "/dev/gpiochip0" - Example of pin A0 on Aster Carrier Board;

    if (gpiolineName == NULL || gpioChip == NULL) {
        glob_t glob_result;
        glob("/dev/gpiochip*", GLOB_TILDE, NULL, &glob_result);
        for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
            const char* gpioChip = glob_result.gl_pathv[i];
            struct gpiod_chip* chip = gpiod_chip_open(gpioChip);
            
            int num_lines = gpiod_chip_num_lines(chip);
            printf("\n\n%s - %d lines\n", gpiod_chip_name(chip), num_lines);

            for (unsigned int line_offset = 0; line_offset < num_lines; line_offset++) {
                struct gpiod_line* line = gpiod_chip_get_line(chip, line_offset);

                const char* line_name = gpiod_line_name(line);
                if (line_name == NULL) {
                    line_name = "unnamed";
                }

                const char* line_consumer;
                if (gpiod_line_is_used(line)) {
                    line_consumer = gpiod_line_consumer(line);
                } else {
                    line_consumer = "unused";
                }

                const char* line_direction;
                if (gpiod_line_direction(line) == GPIOD_LINE_DIRECTION_INPUT) {
                    line_direction = "input";
                } else {
                    line_direction = "output";
                }

                printf("Line %-2u:  %-15s  %-20s  %-10s\n", line_offset, line_name, line_consumer, line_direction);

                gpiod_line_release(line);
            }

            gpiod_chip_close(chip);
        }
        globfree(&glob_result);
    } else {
        struct gpiod_line* gpioline = gpiod_line_find(gpiolineName);

        if (gpioline == NULL) {
            printf("Invalid line name.\n");
        } else {
            gpiod_line_request_output(gpioline, "GPIO application", 0);
            while (1) {
                gpiod_line_set_value(gpioline, 0);
                usleep(1000000);
                gpiod_line_set_value(gpioline, 1);
                usleep(1000000);
            }
        }
    }

    return 0;
}
