#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

struct vec3 {
  float x{0.f};
  float y{0.f};
  float z{0.f};

  vec3() = default;

  vec3(float a_x, float a_y, float a_z) : x{a_x}, y{a_y}, z{a_z} {}
};

struct face {
  unsigned int vi1;
  unsigned int ni1;

  unsigned int vi2;
  unsigned int ni2;

  unsigned int vi3;
  unsigned int ni3;

  face() = default;

  face(unsigned int a_vi1, unsigned int a_ni1, unsigned int a_vi2,
       unsigned int a_ni2, unsigned int a_vi3, unsigned int a_ni3)
      : vi1{a_vi1}, ni1{a_ni1}, vi2{a_vi2}, ni2{a_ni2}, vi3{a_vi3}, ni3{a_ni3} {
  }
};

struct obj {
  std::vector<vec3> vertices;
  std::vector<vec3> normals;
  std::vector<face> faces;

  std::string name;

  obj() = default;
};

std::optional<obj> load_obj(const std::string filePath) {
  auto boxPath = std::filesystem::current_path() / filePath;
  std::ifstream boxFile{boxPath.c_str()};

  if (boxFile.is_open()) {
    std::string fileLine;
    obj result;

    while (std::getline(boxFile, fileLine)) {
      std::istringstream lineToParse{fileLine};
      std::string entityType{};

      lineToParse >> entityType;

      if (entityType == "#") {
        continue;
      } else if (entityType == "o") {
        lineToParse >> result.name;
      } else if (entityType == "v") {
        float x, y, z;
        lineToParse >> x >> y >> z;
        result.vertices.push_back({x, y, z});
      } else if (entityType == "vn") {
        float nx, ny, nz;
        lineToParse >> nx >> ny >> nz;
        result.normals.push_back({nx, ny, nz});
      } else if (entityType == "f") {
        unsigned int vi[3], ni[3];
        for (size_t k = 0; k < 3; ++k) {
          std::string f, l;

          lineToParse >> f;
          std::istringstream sf{f};

          // parsing vertex index
          std::getline(sf, l, '/');
          vi[k] = std::stoi(l);

          // parsing texture coordinate index (skipped for now)
          std::getline(sf, l, '/');

          // parsing normal index
          std::getline(sf, l);
          ni[k] = std::stoi(l);
        }

        result.faces.push_back({vi[0], ni[0], vi[1], ni[1], vi[2], ni[2]});
      } else {
        std::cout << "ERROR: Can't parse line: " << fileLine << std::endl;
      }
    }

    boxFile.close();
    return result;
  } else {
    std::cout << "ERROR: Unable to open a file " << boxPath.c_str()
              << std::endl;
    return {};
  }
}
