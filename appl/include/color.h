#ifndef COLOR_H
#define COLOR_H
typedef struct color_t {
    int r;
    int g;
    int b;
    int a;
} color_t;

color_t color_mult(color_t* color, float scalar);
color_t color_add(color_t* color1, color_t* color2);
color_t color_clamp(color_t* color);

#endif //COLOR_H