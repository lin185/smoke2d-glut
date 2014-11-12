
#include <GL/glew.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>

typedef struct Surface_ {
    GLuint FboHandle;
    GLuint TextureHandle;
    int NumComponents;
} Surface;

typedef struct Slab_ {
    Surface Ping;
    Surface Pong;
} Slab;


typedef struct Vector2_ {
    int X;
    int Y;
} Vector2;


#define CellSize (1.25f)
#define ViewportWidth (512)
#define ViewportHeight (512)
#define GridWidth (ViewportWidth / 2)
#define GridHeight (ViewportHeight / 2)
#define SplatRadius ((float) GridWidth / 8.0f)


static const float AmbientTemperature = 0.0f;
static const float ImpulseTemperature = 10.0f;
static const float ImpulseDensity = 1.0f;
static const int NumJacobiIterations = 40;
static const float TimeStep = 0.125f;
static const float SmokeBuoyancy = 1.0f;
static const float SmokeWeight = 0.05f;
static const float GradientScale = 1.125f / CellSize;
static const float TemperatureDissipation = 0.99f;
static const float VelocityDissipation = 0.99f;
static const float DensityDissipation = 0.9999f;
static const Vector2 ImpulsePosition = { GridWidth / 2, - (int) SplatRadius / 2};

static const int PositionSlot = 0;



Slab CreateSlab(GLsizei width, GLsizei height, int numComponents);
Surface CreateSurface(GLsizei width, GLsizei height, int numComponents);
void InitSlabOps();
GLuint CreateProgram(const char* vsKey, const char* gsKey, const char* fsKey);
void CreateObstacles(Surface dest, int width, int height);
GLuint CreateQuad();
void ClearSurface(Surface s, float value);
void SwapSurfaces(Slab* slab);
void Advect(Surface velocity, Surface source, Surface obstacles, Surface dest, float dissipation);
void Jacobi(Surface pressure, Surface divergence, Surface obstacles, Surface dest);
void SubtractGradient(Surface velocity, Surface pressure, Surface obstacles, Surface dest);
void ComputeDivergence(Surface velocity, Surface obstacles, Surface dest);
void ApplyImpulse(Surface dest, Vector2 position, float value);
void ApplyBuoyancy(Surface velocity, Surface temperature, Surface density, Surface dest);
