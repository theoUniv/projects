#include <unistd.h>     
#include <math.h>
#include "ima.h"
#include <unistd.h>
#include <limits.h>


Image *image;

#define ESCAPE 27

void Keyboard(unsigned char key, int x, int y)  {
  switch(key){
  case ESCAPE :
    exit(0);                   
    break;
  default:
    fprintf(stderr, "Unused key\n");
  }
}

void Mouse(int button, int state, int x, int y) {
  switch(button){
  case GLUT_LEFT_BUTTON:
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    break;    
  }
  glutPostRedisplay();
}

int Init(char *s) {
  image = (Image *) malloc(sizeof(Image));
  if (image == NULL) {
    fprintf(stderr, "Out of memory\n");
    return(-1);
  }
  if (ImageLoad_PPM(s, image)==-1) 
	return(-1);
  printf("tailles %d %d\n",(int) image->sizeX, (int) image->sizeY);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);
  
  return (0);
}
int ReInit() {
  /*
  if (ImageLoad_PPM(s, image)==-1) 
	return(-1);
  */
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);
  
  return (0);
}

void Display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  //GLint w = glutGet(GLUT_WINDOW_WIDTH);
  //GLint h = glutGet(GLUT_WINDOW_HEIGHT);
  glDrawPixels(image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, 
	       image->data);

  glFlush();
}

void Reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble)h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void menuFunc(int item) {
  char s[256];
  /*
  Color test3;
  test3.r = 200;
  test3.g = 200;
  test3.g = 200;
  Color test2;
  test2.r = 0;
  test2.g = 0;
  test2.g = 0;
  printf("RESULT = %f", distance_RGB(test2, test3));
*/

  switch(item){
  printf("TEST");
  case 0:
    free(image);
    exit(0);
    break;
  case 1:
    printf("Vert \n");
    vert(image);
    Display();
    break;
  case 2:
    printf("Vert uniforme \n");
    vert_uniforme(image);
    Display();
    break;
  case 3:
    printf("Gris uniforme \n");
    gris_uniform(image);
    Display();
    break;
  case 4:
    printf("Swap red and green \n");
    swap_green_red(image);
    Display();
    break;
  case 5:
    printf("Entrer le nom pour l'image dans cette taille\n");
    scanf("%s", &s[0]);
    imagesave_PPM(s, image);
    break;
  case 6:
    printf("Taille de l image : %d %d\n", (int) image->sizeX, (int) image->sizeY);
    break;
  case 7:
    printf("Vert moye \n");
    vert_moy(image);
    Display();
  case 8:
    printf("Rouge origin \n");
    rouge_origin(image);
    Display();


  case 9:
    printf("\n");
    GLubyte * tableau = create_tab(image);
    printf("\n");

    RLE_compress(tableau, (image->sizeX*image->sizeY)*3);

    Color * CLUT = build_CLUT(image) ;
    print_CLUT(CLUT);
    sort_CLUT(CLUT);
    print_CLUT(CLUT);
    RLE_transform(CLUT, 10, image);
    //printf("\n\nVoici la taille de la CLUT, %d", get_size_of_CLUT(CLUT));
    //print_CLUT(test_k_colors);
    Display();
    break;
  case 10:
    k_means(image, 100, 150, 10, 0);
    //k_means(Image * im, int nb_centroids, int min_distance_centroid, int nb_iteration, int dithering_enabled);
    //print_CLUT(color_table);
    //Color * image_representation = set_centroids_for_image_colours(list_of_centroids, 10, image);
  default:
    break;
  }
}

int main(int argc, char **argv) {  

  if (argc<2) {
    fprintf(stderr, "Usage : palette nom_de_fichier\n");
    exit(0);
  }

  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(640,480);  
  glutInitWindowPosition(100, 100);  
  glutCreateWindow("VPUP8");  

  Init(argv[1]);

  glutCreateMenu(menuFunc);
  glutAddMenuEntry("Quit", 0);
  glutAddMenuEntry("Vert", 1);
  glutAddMenuEntry("Vert uniforme", 2);
  glutAddMenuEntry("Gris uniforme", 3);
  glutAddMenuEntry("Swap G and R", 4);
  glutAddMenuEntry("Save", 5);
  glutAddMenuEntry("Infos", 6);
  glutAddMenuEntry("vert_moy", 7);
  glutAddMenuEntry("Rouge origin", 8);
  glutAddMenuEntry("Compresser_RLE", 9);
  glutAddMenuEntry("Compresser_K_MEANS", 10);



  glutAttachMenu(GLUT_LEFT_BUTTON);

  glutDisplayFunc(Display);  
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);
  
  glutMouseFunc(Mouse);

  glutMainLoop();  

  return 1;
}
