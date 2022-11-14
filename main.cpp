#include "mbed.h"

#define Read 0x03
#define Chip_Erase 0xC7
#define Byte_Program 0x02
#define RDSR 0x05
#define EWSR 0x50
#define WRSR 0x01
#define WREN 0x06
#define WRDI 0x04
#define RDID 0x90
#define JEDEC_ID 0x9F
#define EBSY 0x70
#define DBSY 0x80
#define _delay wait_us(200);

DigitalOut mosi(PB_15);
DigitalIn miso(PB_14);
DigitalOut sck(PB_13);
DigitalOut cs(PB_12);
DigitalOut wp(PB_11);
DigitalOut hold(PB_10);

int a = 0x00;

// void spi_init();
void spi_write(unsigned char addr);
unsigned char read_addr();
void enable_write_status_register();
void write_status_register(unsigned char bit);
unsigned char read_status_register();
unsigned char read(char addr[]);
void write_enable();
void chip_erase();
void write(char addr[], int value);
void jedec_read_id();

int main()
{
    printf("Hello World3\n\r");
    unsigned char data;
    char addr[3] = {0};
    addr[0] = 0x00;
    addr[1] = 0x20;
    addr[2] = 0x08;
    data = 0;
    hold = 1;
    wp = 1;

    cs = 1;

    jedec_read_id();

    // spi_init();
    data = read_status_register();
    printf("read status register 0x%02x\n\r", data);

    chip_erase();
    _delay

        data = read_status_register();
    printf("read status register 0x%02x\n\r", data);
    _delay

    // write_status_register(0x02);

    write_enable();

    data = read_status_register();
    printf("read status register 0x%02x\n\r", data);

    write(addr, 0x12);

    data = read(addr);
    printf("read data 0x%02x\n\r", data);
}

void spi_write(unsigned char addr)
{
    for (int i = 0; i < 8; i++)
    {
        // mosi = addr & 0x80;
        if ((addr & 0x80) == 0x80)
        {
            mosi = 1;
        }
        else
        {
            mosi = 0;
        }
        sck = 1;
        _delay
            addr = addr << 1;
        sck = 0;
        _delay
    }
}
unsigned char read_addr()
{
    unsigned char data = 0;
    for (int i = 0; i < 8; i++)
    {
        data = data << 1;
        if (miso.read() == 1)
        {
            data = data | 0x01;
        }
        else
        {
            data = data | 0x00;
        }
        _delay
            sck = 1;
        _delay
            sck = 0;
    }
    return data;
}

// void spi_init()
// {
//     cs = 1;
//     _delay
//         spi.format(8, 3);
//     spi.frequency(1000000);
// }

void enable_write_status_register()
{
    cs = 0;
    _delay
        spi_write(EWSR);
    cs = 1;
    _delay
}

void write_status_register(unsigned char bit)
{
    enable_write_status_register();

    cs = 0;
    _delay
        spi_write(WRSR);
    spi_write(bit);
    cs = 1;
}

unsigned char read_status_register()
{
    unsigned char data;
    cs = 0;
    _delay
        spi_write(RDSR);
    spi_write(0xFF);
    data = read_addr();
    cs = 1;
    _delay return data;
}

unsigned char read(char addr[])
{
    unsigned char data;
    cs = 0;
    _delay
        spi_write(Read);
    spi_write(addr[0]);
    spi_write(addr[1]);
    spi_write(addr[2]);
    spi_write(0x00);
    data = read_addr();
    cs = 1;
    _delay return data;
}

void write_enable()
{
    cs = 0;
    _delay
        spi_write(WREN);
    cs = 1;
    _delay
}

void chip_erase()
{
    write_enable();

    cs = 0;
    wait_us(40);
    spi_write(Chip_Erase);
    _delay
        cs = 1;
    _delay
}

void write(char addr[], int value)
{
    cs = 0;
    _delay
        spi_write(Byte_Program);
    spi_write(addr[0]);
    spi_write(addr[1]);
    spi_write(addr[2]);
    spi_write(value);
    cs = 1;
    _delay
}

void jedec_read_id()
{

    unsigned char data[3] = {};

    cs = 0;
    _delay

        spi_write(JEDEC_ID);
    spi_write(0x00);

    data[0] = read_addr();
    data[1] = read_addr();
    data[2] = read_addr();

    cs = 1;
    _delay

        printf("JEDEC : 0x%02x, 0x%02x, 0x%02x\n\r", data[0], data[1], data[2]);
}
