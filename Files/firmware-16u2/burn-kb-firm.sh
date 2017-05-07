sudo dfu-programmer atmega16u2 erase
sudo dfu-programmer atmega16u2 flash --debug 1 Arduino-keyboard-0.3.hex
sudo dfu-programmer atmega16u2 reset