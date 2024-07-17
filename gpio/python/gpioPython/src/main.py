#!python3

import time
import gpiod
from glob import glob

if __name__ == "__main__":
    gpiolineName = None # "SODIMM_55" - Example of pin A0 on Aster Carrier Board
    gpioChip = None # "/dev/gpiochip0"  - Example of pin A0 on Aster Carrier Board

    if (gpiolineName == None) or (gpioChip == None):
        for gpioChip in glob("/dev/gpiochip*"):
            chip = gpiod.Chip(gpioChip)
            info = chip.get_info()
            print(f"\n{info.name} - {info.num_lines} lines")
            
            for line_offset in range(info.num_lines):
                line = chip.get_line_info(line_offset)
                line_name = line.name
                if line_name == None:
                    line_name = "unnamed  "

                if line.consumer:
                    line_consumer = line.consumer
                else: 
                    line_consumer = "unused"

                if line.direction == "1":
                    line_direction = "input"
                else:  
                    line_direction = "output"
                print(f"Line {line_offset:<2}:  {line_name:<15}  {line_consumer:<20}  {line_direction:<10}")

    else:
        chip = gpiod.Chip(gpioChip)
        line_offset = chip.line_offset_from_id(gpiolineName)

        with gpiod.request_lines(
            gpioChip,
            consumer="GPIO application",
            config={
                line_offset: gpiod.LineSettings(
                    direction = gpiod.line.Direction.OUTPUT, 
                    output_value = gpiod.line.Value.ACTIVE
                )
            },
        ) as request:
            while True:
                value = gpiod.line.Value.INACTIVE
                print(f"{gpiolineName} = {value}")
                request.set_value(line_offset, value)
                time.sleep(1)

                value = gpiod.line.Value.ACTIVE
                print(f"{gpiolineName} = {value}")
                request.set_value(line_offset, value)
                time.sleep(1)