#include <stdlib.h>
#include <stdio.h>
#include "geometry.h"

int main(int argc, char** argv) {


  printf("\n pointPlusVector : (0, 0, 0) + (1, 2, 0) = ");
  printVector3D(pointPlusVector(pointXYZ(0, 0, 0), vectorXYZ(1, 2, 0)));

  printf("\n addVectors : (0.5, 1.0, -2.0) + (0.2, -1.0, 0) = ");
  printVector3D(addVectors(vectorXYZ(0.5, 1.0, -2.), vectorXYZ(0.2, -1., 0.)));

  printf("\n subVectors : (0.5, 1.0, -2.0) - (0.2, -1.0, 0) = ");
  printVector3D(subVectors(vectorXYZ(0.5, 1.0, -2.0), vectorXYZ(0.2, -1., 0)));

  printf("\n multVector : (0.5, 1.0, -2.0) * 2.0 = ");
  printVector3D(multVector(vectorXYZ(0.5, 1.0, -2.0), 2.0));

  printf("\n multVector : (0.5, 1.0, -2.0) * 0 = ");
  printVector3D(multVector(vectorXYZ(0.5, 1.0, -2.0), 0));

  printf("\n divVector : (0.5, 1.0, -2.0) / 2.0 = ");
  printVector3D(divVector(vectorXYZ(0.5, 1.0, -2.0), 2.0));

  printf("\n divVector : (0.5, 1.0, -2.0) / 0.0 = ");
  printVector3D(divVector(vectorXYZ(0.5, 1.0, -2.0), 0));

  printf("\n dot((1.0, 0.0, 0.0), (2.0, 0.0, 0.0)) = ");
  printf("%f", dot(vectorXYZ(1.0, 0, 0), vectorXYZ(2., 0., 0.)));

  printf("\n dot((1.0, 0.0, 0.0), (0.0, 1.0, 0.0)) = ");
  printf("%f", dot(vectorXYZ(0.0, 0, 0), vectorXYZ(2., 0., 0.)));

  printf("\n norm((2, 0, 0) = ");
  printf("%f", norm(vectorXYZ(2, 0, 0)));

  printf("\n norm((1, 1, 1) = ");
  printf("%f", norm(vectorXYZ(1, 1, 1)));

  printf("\n norm((0, 0, 0)) = ");
  printf("%f", norm(vectorXYZ(0, 0, 0)));

  printf("\n normalize((1, 1, 1)) = ");
  printVector3D(normalize(vectorXYZ(1, 1, 1)));

  printf("\n normalize((0, 0, 0)) = ");
  printVector3D(normalize(vectorXYZ(0, 0, 0)));

  printf("\n ");
  return EXIT_SUCCESS;
}