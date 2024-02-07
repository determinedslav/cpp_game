typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long s64;
typedef unsigned long u64;

#define global_variable static
#define internal static

inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

u32 color_white = 0xffffff;
u32 color_black = 0x000000;
u32 color_arena = 0xffaa33;
u32 color_borders = 0xff6611;
u32 color_score = 0xcceecc;
u32 color_text = 0xffff11;
u32 color_text_unfocused = 0xffff99;
u32 color_button = 0xff1111;
u32 color_button_unfocused = 0xff9999;

const char* char_1[5] = {
	"  0",
	"  0",
	"  0",
	"  0",
	"  0",
};

const char* char_2[5] = {
	"000",
	"  0",
	"000",
	"0  ",
	"000",
};

const char* char_3[5] = {
	"000",
	"  0",
	"000",
	"  0",
	"000",
};

const char* char_4[5] = {
	"0 0",
	"0 0",
	"000",
	"  0",
	"  0",
};

const char* char_5[5] = {
	"000",
	"0  ",
	"000",
	"  0",
	"000",
};

const char* char_6[5] = {
	"000",
	"0  ",
	"000",
	"0 0",
	"000",
};

const char* char_7[5] = {
	"000",
	"  0",
	"  0",
	"  0",
	"  0",
};

const char* char_8[5] = {
	"000",
	"0 0",
	"000",
	"0 0",
	"000",
};

const char* char_9[5] = {
	"000",
	"0 0",
	"000",
	"  0",
	"000",
};

const char* char_0[5] = {
	"000",
	"0 0",
	"0 0",
	"0 0",
	"000",
};

const char* char_p[5] = {
	"000",
	"0 0",
	"000",
	"0  ",
	"0  ",
};

const char* char_test[5] = {
	"0000",
	"0  0",
	"0  0",
	" 00 ",
	"0000",
};