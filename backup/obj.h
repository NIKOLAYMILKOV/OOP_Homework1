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

   std::vector<std::vector<int>> faces;
   std::vector<Vertex> vertices;

   // std::vector<std::vector<std::string>> vLines;
   // std::vector<std::vector<std::string>> fLines;

   bool distanse(const Vertex& a, const Vertex& b, const int s);
   void reverseVector(std::vector<int>& vec);
public:
   Object3D() = default;
   Object3D(const std::string& fileName);
   Object3D(std::istream inSteram);
   const int getVertexCount() const;
   const int getFaceCount() const;
   void flip();
   void save(const std::string& fileName);
   void print(std::ostream &out) const;
   Object3D cut(std::function<bool(float x, float y, float z)> f);
   static Object3D createCube(const int s);
   ~Object3D() = default;
};
