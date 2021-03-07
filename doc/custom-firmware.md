# Custom firmware
## Triggers
- Common
- Hold
- Double tap
- Double tap hold

## Quanta
Quantas are the standarized data struct for the firmware.
Quantas are to the firmware what keycodes are to the hid protocol, the adopted format for information reguarding a key.
Since not every key press is the same quantas have different types, they do follow a general format however.
A Quanta is 4 bytes long and each byte has a different purpose.
Let’s denote the Quanta as `0x44332211`, the byte represented by `44` (MSB) invariably represents the type of the quanta, while the other 3 bytes can vary depending on the quanta type.
Each type of quanta will be explained individually.

### Common
The common quanta format is analagous to the usual way a keypress work in a generic keyboard, the key is added to report on press and stays there until release.
With the intent of establishing an uniform quanta format while keeping the different types to a minimun this type can send modifiers, a single key and a layer value simultaneously.
It should be made clear that assigning all 3 jobs to a single key is hardly a sane usecase, for better or for worse, however, it is possible.
In general the key will either send just a keycode, a keycode plus a mod (useful for shortcuts such as `Ctrl-A` or `Shift-5` aka %) or just a layer.
The meaning of the bytes for this quanta are:

Quanta type value in first byte (numerical value of 01 for common)
Layer value in second byte.
Modifiers in third byte.
Keycode in fourth byte.
As an example: `0x01000104` represents `Crtl - A`, `0x01020000` = ORs 02 to the layer variable.

### Toggle
A toggle is quite similar to a the Numlock and Capslock key in an usual keyboard in that it turns something on until the key is retapped.
This can be used to toggle a modifier (like Ctrl or Shift) until the key is pressed again.
It is also useful for toggling layers.
Note that Numlock and Capslock are just a normal key under the HID implementation, the “toggling” is done by the operating system, so DO NOT use toggle on Capslock or Numlock, use Common.
The meaning of a Toggle Quanta is analagous to that of Common only difference being that the Quanta type has a value of 02.

### Flag
The Flag Quanta is used to assign a value to the flags variable. The format for the quanta is:

1st byte: value to be assigned to flag
2nd byte: flag offset
3rd byte: None
4th byte: type as usual

### Arbitrary
Returns whatever is encoded to the user in the report.
Useful if firmware wants to use keys to receive information to vary things such as led intensity of whatever.
