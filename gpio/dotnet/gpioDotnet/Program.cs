using System.Device.Gpio;
using System.Device.Gpio.Drivers;

class Program
{
    static void Main()
    {
        int gpioLine = 7; // "SODIMM_55" - Example of pin A0 on Aster Carrier Board;
        int gpioChip = 0;// "/dev/gpiochip0" - Example of pin A0 on Aster Carrier Board;
        LibGpiodDriver gpiodDriver = new LibGpiodDriver(gpioChip);
        GpioController gpioController = new GpioController(PinNumberingScheme.Logical, gpiodDriver);

        gpioController.OpenPin(gpioLine, PinMode.Output);

        while (true)
        {
            gpioController.Write(gpioLine, PinValue.Low);
            Thread.Sleep(1000);
            gpioController.Write(gpioLine, PinValue.High);
            Thread.Sleep(1000);
        }
    }
}
