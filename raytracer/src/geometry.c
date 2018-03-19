/*
 * geometry.c
 * Structures et fonctions utilitaires pour manipuler des vecteurs et des points
 */
#include "geometry.h"

Point3D pointXYZ(float x, float y, float z) {
  Point3D p;
  p.x = x;
  p.y = y;
  p.z = z;
  return p;
}

Vector3D vectorXYZ(float x, float y, float z) {
  Vector3D v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

Vector3D vector(Point3D A, Point3D B) {
  Vector3D v;
  v.x = B.x - A.x;
  v.y = B.y - A.y;
  v.z = B.z - A.z;
  return v;
}

Point3D pointPlusVector(Point3D P, Vector3D V) {
  Vector3D res;
  res.x = P.x + V.x;
  res.y = P.y + V.y;
  res.z = P.z + V.z;
  return res;
}

Vector3D addVectors(Vector3D A, Vector3D B) {
  Vector3D res;
  res.x = A.x + B.x;
  res.y = A.y + B.y;
  res.z = A.z + B.z;
  return res;
}

Vector3D subVectors(Vector3D A, Vector3D B) {
  Vector3D res;
  res.x = A.x - B.x;
  res.y = A.y - B.y;
  res.z = A.z - B.z;
  return res;
}

Vector3D multVector(Vector3D A, float a) {
  Vector3D res;
  res.x = A.x * a;
  res.y = A.y * a;
  res.z = A.z * a;
  return res;
}

Vector3D divVector(Vector3D A, float a) {
  if (a == 0.)
    return A;
  Vector3D res;
  res.x = A.x / a;
  res.y = A.y / a;
  res.z = A.z / a;
  return res;
}

float dot(Vector3D A, Vector3D B) {
  return A.x * B.x + A.y * B.y + A.z * B.z;
}

float norm(Vector3D A) {
  return sqrt(pow(A.x, 2) + pow(A.y, 2) + pow(A.z, 2));
}

Vector3D normalize(Vector3D A) {
  if (A.x == 0. || A.y == 0. || A.z == 0.)
    return A;
  return divVector(A, norm(A));
}

void printVector3D(Vector3D v) {
  printf("(%f,%f,%f)", v.x, v.y, v.z);
}

void printPoint3D(Point3D p) {
  printf("(%f,%f,%f)", p.x, p.y, p.z);
}