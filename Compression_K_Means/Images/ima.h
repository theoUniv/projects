#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>

#define GL_SILENCE_DEPRECATION
/* pour Bocal 
#include <GL/gl.h> 
#include <GL/glut.h>
#include <GL/freeglut.h>  
*/
/* pour Mac */
#include <GLUT/glut.h>


struct Color_t {
  int r, g, b;
  int frequence;
  int is_last;
  int r_centroid;
  int g_centroid;
  int b_centroid;
};

typedef struct Color_t Color;

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    GLubyte *data;
};
typedef struct Image Image;
typedef unsigned short utab [3][3][3];

int ImageLoad_PPM(char *filename, Image *image);
void imagesave_PPM(char *filename, Image *image);
void upsidedown(Image *im);
void vert(Image *image);
void vert_moy(Image *image);
void vert_uniforme(Image *image);
void gris_uniform (Image *image);
void rouge_origin(Image *image);
void swap_green_red(Image *image);
GLubyte * create_tab(Image *im);
GLubyte * RLE_compress(GLubyte * tab, int taille);
size_t GLUBYTE_lenght(GLubyte * tab);
void write_tab_in_file(GLubyte * tab, int size_of_tab);
Color * build_CLUT(Image *im);
int is_in_CLUT(Color * CLUT, GLubyte r, GLubyte g, GLubyte b, int size_of_tab);
void print_CLUT(Color * CLUT);
void sort_CLUT(Color * CLUT);
int distance_RGB(Color color1, Color color2) ;
void RLE_transform(Color * CLUT, int k, Image *im);
int get_size_of_CLUT(Color * CLUT);
Color * get_K_colors_from_CLUT(int k, Color * CLUT, int min_distance);
Color get_nearest_centroid(Color * centroids, int nb_centroids, Color color);
void set_centroids_for_image_colours(Color * centroids, Color * image_representation, int nb_centroids, Image * im);
void update_centroid_from_colours(Color *centroide, Color * image_colour_tab, Image * im);
void update_colours_from_centroid(Color *centroide, Color * image_colour_tab, Image * im);
Color * build_color_table_from_image(Image *im);
void update_image_with_color_vector(Image *im, Color * vector);
void k_means(Image * im, int nb_centroids, int min_distance_centroid, int nb_iteration, int dithering_enabled);
int ColorToInt(Color couleur);
void dithering(Image *im, int seuil);