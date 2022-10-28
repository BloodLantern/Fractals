#include <stdbool.h>
#include <math.h>

typedef struct float2
{
    float x;
    float y;
} float2;

typedef struct Fractal {
    bool display;
    float angle;
    int depth;
    float2 position;
    float size;
    int polygonSegments;
    float polygonReduction;
} Fractal;

typedef struct App
{
    Fractal fractals[5];
    float2 cameraStart;
    float2 camera;
} App;

float2 getFloat2(float x, float y);

void appInit(App* app);
void appUpdate(App* app);
void appShutdown(App* app);

void drawSquareUsingCenter(float2 pos, float width, float2 rotationCenter, float angle, unsigned int color);
void drawSquare(float2 p1, float2 p2, float2 p3, float2 p4, unsigned int color);
// filled: Remplissage ou non de la forme (cvPathFill vs cvPathStroke)
// segments: Nombre de segments qui composent le contour du cercle
void drawCircle(float x, float y, float radius, int segments, unsigned int color, bool filled);
void drawTriangle(float2 p1, float2 p2, float2 p3, unsigned int color);
void drawEquilateralTriangle(float2 pos, float sideLength, float2 rotationCenter, float angle, unsigned int color);
void drawPolygon(
    int nGones,        // le nombre de côtés du polygone régulier
    float radius,      // le rayon du cercle circonscrit au polygone
    float angleOffset, // un angle de rotation de la figure
    float2 pos, // la position du centre de la figure
    unsigned int color // la couleur de traçage
);

void squareFractal(float2 pos, float2 startingPos, float width, float angle, int depth, unsigned int color);
void drawSierpinskiTriangle(float2 pos, float2 startingPos, float sideLength, float angle, int depth, unsigned int color);
void drawPythagorasTree(
    float2 p1, // 1er point à la base du carré
    float2 p2, // 2ème point à la base du carré
    float angle,        // l’angle associé au sommet principal du triangle rectangle
    int depth,          // la profondeur de récursivité
    unsigned int color  // la couleur
);
void drawPolygonSpiral(
    int nGones,        // le nombre de côtés du polygone régulier
    float radius,      // le rayon du cercle circonscrit à la spirale
    float angleOffset, // un angle de rotation de la figure
    float2 pos, // la position du centre de la figure
    unsigned int color, // la couleur de traçage
    float kReduction,   // le coefficient de réduction de la longueur des
                        // côtés entre deux polygones successifs
    int depth           // la profondeur de récursivité
);
