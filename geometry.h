#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
  int x_delta;
  int y_delta;
} rel_point_t, *rel_point_ptr;

typedef struct {
  double x;
  double y;
} point_t, *point_ptr, vector_t, *vector_ptr;

point_t point(double x, double y);
vector_t vector(double x, double y);
vector_t points_vector(const point_ptr a, const point_ptr b);
point_t relative_point(const point_ptr p, const rel_point_ptr rel_p);
double point_distance(const point_ptr p1, const point_ptr p2);
double random_angle(void);
point_t random_point_around(const point_ptr p, int min_tolerance,
                            int max_tolerance);

#endif  // GEOMETRY_H_
