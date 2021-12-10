#include <stdio.h>
#include <stdint.h>

/* VULNCON{rc5_w45_f1r57_publ15h3d_1n_1994} 
   break at 0b06 */

const uint32_t key[] = {
    0xda5b7b51, 0x4d9d5d49,
    0xa4014ee0, 0x1054dbd0,
    0xf3f2382c, 0x6632c593,
    0xdadd4bea, 0xdac71d8a,
    0x00cad909, 0x9701d57f,
    0xf402391f, 0x6c9e102f, /* */
    0x386b99c4, 0x023bf64b,
    0xeab6f404, 0x8b7a5821,
    0x30612cd2, 0x5414e5db,
    0x7dc6d4b9, 0x1ffcc1ff,
    0x5547e4b6, 0x89585227,
    0x6d1997b8, 0x76fffbbf
};

void encrypt(uint16_t* block)
{
    uint32_t x = (block[1] << 16) | block[0];
    uint32_t y = (block[3] << 16) | block[2];
    x += 0xd359ac74;
    y += 0x2d7bd7df;
    for(int i = 0; i < 12; i++)
    {
        uint32_t count = y & 0x1f;
        uint32_t xy = x ^ y;
        xy = (xy << count) | (xy >> (32 - count));
        xy += key[2*i];
        x = xy;
        count = x & 0x1f;
        xy = x ^ y;
        xy = (xy << count) | (xy >> (32 - count));
        xy += key[2*i + 1];
        y = xy;
    }
    block[0] = x & 0xffff;
    block[1] = x >> 16;
    block[2] = y & 0xffff;
    block[3] = y >> 16;
}

void decrypt(uint16_t* block)
{
    uint32_t x = (block[1] << 16) | block[0];
    uint32_t y = (block[3] << 16) | block[2];
    for(int i = 11; i >= 0; i--)
    {
        uint32_t xy = y;
        uint32_t count = x & 0x1f;
        xy -= key[2*i + 1];
        xy = (xy >> count) | (xy << (32 - count));
        y = xy ^ x;
        xy = x;
        count = y & 0x1f;
        xy -= key[2*i];
        xy = (xy >> count) | (xy << (32 - count));
        x = xy ^ y;
    }
    x -= 0xd359ac74;
    y -= 0x2d7bd7df;
    block[0] = x & 0xffff;
    block[1] = x >> 16;
    block[2] = y & 0xffff;
    block[3] = y >> 16;
}


int main()
{
    char text[] = {
        0xbc, 0x96, 0x54, 0xc0, 0xb8, 0x5e, 0xd4, 0xea,
        0x71, 0xe5, 0x30, 0x31, 0x3c, 0x69, 0xfc, 0x4a,
        0xf1, 0x9c, 0xd3, 0x2e, 0x4d, 0xa8, 0xd8, 0xde,
        0xe3, 0x9e, 0xd5, 0x65, 0xe7, 0x14, 0x01, 0xfc,
        0xca, 0xea, 0x9a, 0x66, 0x15, 0x73, 0xf4, 0xc6,
        0x49, 0x9d, 0x7c, 0x08, 0xef, 0xe0, 0xaf, 0x80,
        0xc0, 0x3c, 0x02, 0x26, 0xef, 0x33, 0xf9, 0x18,
        0xe3, 0xe5, 0x34, 0xeb, 0x16, 0x70, 0x14, 0xc9,
        0x00 };
//    encrypt((uint16_t*)text);
    for(int i = 14; i >= 0; i--)
    {
        decrypt((uint16_t*)(text + 4*i));
    }
    puts(text);
    return 0;
}
