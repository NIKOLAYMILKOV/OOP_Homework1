#include "obj.h"

bool eq(double x, double y, double z)
{
    return x == y && y == z;
}
bool eq1(double x, double y, double z)
{
    return x == 1 && y == -1 && z == 1;
}
bool eq2(double x, double y, double z)
{
    return x  <= 0.3;
}

Object3D::Object3D(const std::string& fileName)
{
    std::fstream file;
    file.open(fileName, std::ios::in);
    if(!file.is_open()) return;
    std::string line;
    std::string str;

    while (std::getline(file, line))
    {
        std::stringstream strstream(line);
        strstream >> str;
        
        if (str[0] == 'v')
        {
            Vertex vertex;
            strstream >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }

        if (str[0] == 'f')
        {
            std::vector<int> vec;
            int number;
            while (strstream >> number) // str[str.length() - 1] != "/n"
            {
                vec.push_back(number);
            }
            faces.push_back(vec);
        }   
    }

    file.close();
}



bool Object3D::distanse(const Vertex &a, const Vertex &b, const int s)
{
    int distance = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.x, 2));

    return distance == s;
}

void Object3D::reverseVector(std::vector<int> &vec)
{
    for (size_t i = 0; i < vec.size()/2; i++)
    {
        int temp = vec[i];
        vec[i] = vec[vec.size() - 1 - i];
        vec[vec.size() - 1 - i] = temp;
    }
}

Object3D::Object3D(std::istream inSteram)
{

}

const int Object3D::getVertexCount() const
{
    return vertices.size();
}

const int Object3D::getFaceCount() const
{
    return faces.size();
}

void Object3D::flip()
{
    for(int i = 0; i < this->faces.size(); i++)
    {
       reverseVector(faces[i]);
    }
}

void Object3D::save(const std::string &fileName)
{
    std::ofstream f(fileName);

    this->print(f);
    f.close();

    std::cout << "Model is saved in: " << fileName << std::endl;
}

void Object3D::print(std::ostream &out) const
{
    for(Vertex ver : vertices)
    {
        out << "v " << ver.x << " " << ver.y << " " << ver.z <<std::endl;
    }

    for(std::vector<int> vec : faces)
    {
        out << "f ";
        for (int num : vec)
        {
            out << num << " ";
        }
        out << std::endl;
    }
}

Object3D Object3D::cut(std::function<bool(float x, float y, float z)> f)
{
    Object3D result;
    std::vector<int> indexVector;

    for (size_t i = 0; i < this->vertices.size(); i++)
    {
        result.vertices.push_back(this->vertices[i]);
        
        if (!f(this->vertices[i].x, this->vertices[i].y, this->vertices[i].z))
        {
            indexVector.push_back(i + 1);
        }
        
    }

    bool isValidVertex = true;
    for(size_t i = 0; i < faces.size(); i++)
    {
        for (size_t j = 0; j < faces[i].size(); j++)
        {
            for (size_t k = 0; k < indexVector.size(); k++)
            {
                if (indexVector[k] == faces[i][j])
                {
                    isValidVertex = false;
                    break;
                }
            }
            if (!isValidVertex) break;
        }

        if (isValidVertex) result.faces.push_back(this->faces[i]);
        isValidVertex = true;
    }

    return result;
}

// Object3D Object3D::createCube(const int s)
// {
//     Object3D cube;
//     Vertex vertices[8] = {{0,0,0},{s,0,0},{s,0,s},{0,0,s},{0,s,0},{s,s,0},{s,s,s},{0,s,s}};
//     //                       1       2       3       4       5       6       7       8
//     for (size_t i = 0; i < 8; i++)
//     {
//         cube.vertices.push_back(vertices[i]);
//     }

//     std::vector<std::vector<int>> faces;
//     cube.faces.push_back({1,2,3,4});
//     cube.faces.push_back({5,6,7,8});
//     cube.faces.push_back({2,6,5,1});
//     cube.faces.push_back({2,6,7,3});
//     cube.faces.push_back({3,7,8,4});
//     cube.faces.push_back({4,8,5,11});
    
//     return cube;
// }

int main()
{
    Object3D o("cow.obj");
    Object3D cut = o.cut(eq2);
    o.print(std::cout);
   // Object3D cube = Object3D::createCube(5);
    // cube.save("myCube.obj");

    cut.flip();
    cut.save("cut_cow.obj");
    
    return 0;
}