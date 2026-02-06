// Enter your pattern code here and click on the Play button
// below to execute your code. The results will then be visible
// in the Pattern Data view.
// 
// More information can be found in the documentation.
// 
// 
// Simple example:
// 
// import std.io;
// 
// struct Pattern {
//     u32 int;
//     float floating_point;
// };
// 
// Pattern my_pattern @ 0x00;
// std::print("0x{:08X}", my_pattern.int);
char char_array_at_0x00[0x04] @ 0x00;

u32 u32_at_0x04 @ 0x04;
u16 u16_at_0x08 @ 0x08;
u16 u16_at_0x0A @ 0x0A;
u16 u16_at_0x0C @ 0x0C;
char char_array_at_0x0E[0x04] @ 0x0E;
u32 u32_at_0x12 @ 0x12;
u8 u8_at_0x16 @ 0x16;
char char_array_at_0x19[0x07] @ 0x19;