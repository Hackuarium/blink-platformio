Folders:

- include/: All the headers for the project.
- lib/: All the private libraries for the project.
- scr/: All the code of the project.

File:

- platformio.ini: Document for board configuration.

# Install

Install platformIO IDE into Visual Studio: https://platformio.org/install/ide?install=vscode

## Installation problems

Some time, we have problems to install PlatformIO into Visual Studio, these problems can be solve with this:

- Install Python3-disutils: 

```bash
sudo apt-get install python3-distutils
```

- Install Rules to upload files into Lilypad USB: If PlatformIO (pio) display the next warning when you upload the code:

```bash
"Warning! Please install `99-platformio-udev.rules`.
```

Please, visit this site: https://docs.platformio.org/en/latest/faq.html#platformio-udev-rules" or run the these commands into the terminal:

```bash
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
```

* If you don't have `curl`, run this command:

```bash
sudo apt install curl
```

* Restart "udev" management tool:

```bash
sudo service udev restart
```

* Add your own `username` to the `dialout` group if you are not `root`:

```bash
sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER
```

Note: You will need to log out and log back in again (or reboot) for the user group changes to take effect.

# Open Project

1. After install and for avoid problems with this project, please try to create a new project with the same board of this project (Arduino Lilypad USB) this option download the Arduino Framework that platformIO needs to compile our project.

2. Open the project from: `PIO Home -> Open Project`. If you open the project from `Visual Studio Code -> File -> Open Folder` you cann't `Build` and `Upload` your code.

3. Check if pio don't have problems to find the include libraries like `Arduino.h`. I you have this problem, Go to point 1 again.

4. Pio detect the COM port of the Lilypad USB whe you plug in, so, in this point, you don't have any problems with `Build` and `Upload` your code into the Arduino.