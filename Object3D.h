#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <functional>
#include <sstream>
#include <cmath>

class Object3D
{
private:
   struct Vertex{
      double x;
      double y;
      double z;
   };

   std::vector<Vertex> vertices;
   std::vector<std::vector<int>> faces;

   void reverseVector(std::vector<int>& vec);
public:
   static Object3D createCube(const double s);
   static Object3D createSphere(const double r);
   
   Object3D() = default;
   Object3D(const std::string& fileName);
   Object3D(std::istream& inSteram);
   ~Object3D() = default;

   const int getVertexCount() const;
   const int getFaceCount() const;
   void flip();
   
   void save(const std::string& fileName) const;
   void print(std::ostream &out) const;
   Object3D cut(std::function<bool(float x, float y, float z)> f) const;
};
