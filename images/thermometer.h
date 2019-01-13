#define THERMOMETER_WIDTH 40
#define THERMOMETER_HEIGHT 65
static PROGMEM unsigned char thermometer_data[65][5] = {
{0b11111111, 0b11000000, 0b00000011, 0b11111111, 0b11111111},
{0b11111111, 0b10000000, 0b00000001, 0b11111111, 0b11111111},
{0b11111111, 0b00000000, 0b00000000, 0b01111111, 0b11111111},
{0b11111110, 0b00000000, 0b00000000, 0b01111111, 0b11111111},
{0b11111100, 0b00000000, 0b00000000, 0b00111111, 0b11111111},
{0b11111000, 0b00000000, 0b00000000, 0b00011111, 0b11111111},
{0b11111000, 0b00000111, 0b11100000, 0b00011111, 0b11111111},
{0b11110000, 0b00001111, 0b11111000, 0b00001111, 0b11111111},
{0b11110000, 0b00011100, 0b00111000, 0b00001100, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001100, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001111, 0b11111111},
{0b11110000, 0b00111000, 0b00011100, 0b00001111, 0b11111111},
{0b11110000, 0b00111000, 0b00011100, 0b00001111, 0b11111111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001111, 0b11111111},
{0b11110000, 0b00111000, 0b00011100, 0b00001111, 0b11111111},
{0b11110000, 0b00111000, 0b00011100, 0b00001111, 0b11111111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001000, 0b00000111},
{0b11110000, 0b00111000, 0b00011100, 0b00001111, 0b11111111},
{0b11110000, 0b00111000, 0b00011100, 0b00001111, 0b11111111},
{0b11110000, 0b00111000, 0b00011100, 0b00001111, 0b11111111},
{0b11110000, 0b00111000, 0b00011100, 0b00001111, 0b11111111},
{0b11100000, 0b00111000, 0b00011100, 0b00000111, 0b11111111},
{0b11000000, 0b00111000, 0b00011100, 0b00000011, 0b11111111},
{0b11000000, 0b00111000, 0b00011100, 0b00000011, 0b11111111},
{0b10000000, 0b01111000, 0b00011110, 0b00000001, 0b11111111},
{0b10000000, 0b11111000, 0b00011111, 0b00000000, 0b11111111},
{0b00000001, 0b11100000, 0b00000111, 0b10000000, 0b11111111},
{0b00000001, 0b11000000, 0b00000011, 0b10000000, 0b11111111},
{0b00000011, 0b10000000, 0b00000001, 0b11000000, 0b01111111},
{0b00000011, 0b10000000, 0b00000001, 0b11000000, 0b01111111},
{0b00000011, 0b10000000, 0b00000000, 0b11000000, 0b01111111},
{0b00000011, 0b00000000, 0b00000000, 0b11100000, 0b01111111},
{0b00000011, 0b00000000, 0b00000000, 0b11100000, 0b01111111},
{0b00000011, 0b00000000, 0b00000000, 0b11100000, 0b01111111},
{0b00000011, 0b10000000, 0b00000000, 0b11100000, 0b01111111},
{0b00000011, 0b10000000, 0b00000001, 0b11000000, 0b01111111},
{0b00000011, 0b10000000, 0b00000001, 0b11000000, 0b01111111},
{0b00000001, 0b11000000, 0b00000011, 0b11000000, 0b11111111},
{0b00000001, 0b11100000, 0b00000111, 0b10000000, 0b11111111},
{0b10000000, 0b11110000, 0b00001111, 0b00000000, 0b11111111},
{0b10000000, 0b01111111, 0b11111110, 0b00000001, 0b11111111},
{0b11000000, 0b00111111, 0b11111100, 0b00000001, 0b11111111},
{0b11000000, 0b00001111, 0b11110000, 0b00000011, 0b11111111},
{0b11100000, 0b00000000, 0b00000000, 0b00000111, 0b11111111},
{0b11110000, 0b00000000, 0b00000000, 0b00001111, 0b11111111},
{0b11111000, 0b00000000, 0b00000000, 0b00011111, 0b11111111},
{0b11111100, 0b00000000, 0b00000000, 0b00111111, 0b11111111},
{0b11111111, 0b00000000, 0b00000000, 0b11111111, 0b11111111},
{0b11111111, 0b11000000, 0b00000011, 0b11111111, 0b11111111},
};
