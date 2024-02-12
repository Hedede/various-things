#ifndef RENDERER_H
#define RENDERER_H

namespace graphics {
void initialize();
void load();
void clear();
void render();
void reshape(int w, int h);
extern double c1x,c1y,c1r,c1o;
extern double c2x,c2y,c2r;
} // namespace graphics

#endif // RENDERER_H
