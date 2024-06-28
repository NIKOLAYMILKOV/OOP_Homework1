#include "Object3D.h"

bool fn1(double x, double y, double z)
{
    return x == y && y == z;
}
bool fn2(double x, double y, double z)
{
    return x == 1 && y == -1 && z == 1;
}
bool fn3(double x, double y, double z)
{
    return z  >= 0.3;
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

Object3D::Object3D(const std::string& fileName)
{
    std::fstream file; 
    file.open(fileName, std::ios::in);
    if(!file.is_open())
    {
        std::cout << "File could not open" << std::endl;
        return;
    }
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
        }else if (str[0] == 'f')
        {
            std::vector<int> vec;
            int number;
            while (strstream >> number)
            {
                vec.push_back(number);
            }
            faces.push_back(vec);
        }else if (str[0] == '#')
        {
            continue;
        }else{
            throw std::invalid_argument("Invalid file");
        }
    }

    file.close();
}


Object3D::Object3D(std::istream& inSteram)
{   
    std::string line;

    while (std::getline(inSteram, line)) {
        std::istringstream strstream(line);
        std::string str;
        strstream >> str;
        if (str[0] == 'v') {
            Vertex ver;
            strstream >> ver.x >> ver.y >> ver.z;
            this->vertices.push_back(ver); 
            
        } else if (str[0] == 'f') {
            int vertex;
            std::vector<int> vec;
            while (strstream >> vertex)
            {
                vec.push_back(vertex);
            }
            
        this->faces.push_back(vec);
        }else if (str.empty())
        {
            break;
        }else if (str[0] == '#')
        {
            continue;
        }else{
            throw std::invalid_argument("Invalid input");
        }
    }
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

void Object3D::save(const std::string &fileName) const 
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

Object3D Object3D::cut(std::function<bool(float x, float y, float z)> f) const 
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

Object3D Object3D::createCube(const double s)
{
    Object3D cube;
    Vertex vertices[8] = {{0,0,0},{s,0,0},{s,0,s},{0,0,s},{0,s,0},{s,s,0},{s,s,s},{0,s,s}};
    //                       1       2       3       4       5       6       7       8
    for (size_t i = 0; i < 8; i++)
    {
        cube.vertices.push_back(vertices[i]);
    }

    std::vector<std::vector<int>> faces;
    cube.faces.push_back({1,2,3,4});
    cube.faces.push_back({5,6,7,8});
    cube.faces.push_back({2,6,5,1});
    cube.faces.push_back({2,6,7,3});
    cube.faces.push_back({3,7,8,4});
    cube.faces.push_back({4,8,5,1});
    
    return cube;
}

Object3D Object3D::createSphere(const double r)
{
    
    Object3D sphere;
    const double PI = 3.14159265358979323846 ;
    const int numSegments = 15;

    for (int i = 0; i <= numSegments; ++i) {
        float theta = i * PI / numSegments;
        for (int j = 0; j <= numSegments; ++j) {
            Vertex ver;
            float phi = j * 2 * PI / numSegments;
            ver.x = r * std::sin(theta) * std::cos(phi);
            ver.y = r * std::sin(theta) * std::sin(phi);
            ver.z = r * std::cos(theta);
            sphere.vertices.push_back(ver);
        }
    }

    for (int i = 0; i < numSegments; ++i) {
        for (int j = 0; j < numSegments; ++j) {
            std::vector<int> vec;
            int ver1 = i * (numSegments + 1) + j;
            int ver2 = ver1 + 1;
            int ver3 = ver1 + numSegments + 1;
            int ver4 = ver3 + 1;
            vec.push_back(ver1 + 1);
            vec.push_back(ver2 + 1);
            vec.push_back(ver4 + 1);
            sphere.faces.push_back(vec);
            vec.clear();
            vec.push_back(ver1 + 1);
            vec.push_back(ver4 + 1);
            vec.push_back(ver3 + 1);
            sphere.faces.push_back(vec);
        }
    }

    return sphere;
}

int main()
{
    std::ifstream file("cow.obj");
    Object3D o(file);
    
    
    o.cut(fn3);
    o.save("cow1.obj");

    // o.print(std::cout);
    // Object3D cube = Object3D::createCube(5);
    // cube.save("myCube.obj");
    // Object3D sp = Object3D::createSphere(1);
    // sp.save("sp.obj");
    // cut.flip();
    // cut.save("sp.obj");
   
    return 0;
}