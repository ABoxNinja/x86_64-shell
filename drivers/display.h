#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

#include "ports.h"

void set_cursor(int offset)
{
    offset /= 2;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

int get_cursor()
{
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER,VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REGISTER);
    return offset * 2;
}

int get_row_from_offset(int offset) {return offset/(2*MAX_COLS);}
int get_offset(int col, int row){return 2*(row*MAX_COLS+col);}
int move_offset_to_nl(int offset){return get_offset(0,get_row_from_offset(offset)+1);}

void memory_copy(char *source,char *dest,int nbytes)
{
    int i;
    for (i=0;i<nbytes;i++)
    {
        *(dest+i) = *(source+i);
    }
}

void set_char_at_vmem(char character, int offset)
{
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}

int scroll_ln(int offset)
{
    memory_copy(
        (char *) (get_offset(0,1)+VIDEO_ADDRESS),
        (char *) (get_offset(0,0)+VIDEO_ADDRESS),
        MAX_COLS * (MAX_ROWS - 1)*2
    );
    for (int col = 0; col < MAX_COLS; col++)
    {
        set_char_at_vmem(' ', get_offset(col, MAX_ROWS - 1));
    }
    return offset - 2 * MAX_COLS;
}

void print_string(char *string) {
    int offset = get_cursor();
    int i = 0;
    while (string[i] != 0) {
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
            offset = scroll_ln(offset);
        }
        if (string[i] == '\n') {
            offset = move_offset_to_nl(offset);
        } else {
            set_char_at_vmem(string[i], offset);
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}

void clear_screen()
{
    for (int i = 0;i<MAX_COLS * MAX_ROWS;++i)
    {
        set_char_at_vmem(' ',i * 2);
    }
    set_cursor(get_offset(0,0));
}