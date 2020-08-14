#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "stb_image.h"
#include "tiny_obj_loader.h"

struct Texture
{
	int ID, width, height, nrChannels;
	unsigned char* data;
	std::string name;
};

///@brief Static class which helps loading different file types
class FileHandler
{
public:

	///@brief Load a text file and return it as a string
	static std::string loadStrFile(const char* path)
	{

		std::ifstream file(path, std::ios::binary | std::ios::ate);
		if (!file.is_open()) {
			std::cout << "Could not read file at " << path << "!" << std::endl;
		}
		std::string source;
		source.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(&source[0], source.size());

		std::cout << "Loaded file: " << path << std::endl;

		return source;
	}

	///@brief Loads an image file using the stb_image library
	static Texture loadImage(std::string filename, int channels = 3)
	{
		Texture texture;
		std::string newFileName = "assets/textures/" + filename;
		const char* file = newFileName.c_str();
		int width, height, nrChannels;
		texture.data = stbi_load(file, &width, &height, &nrChannels, channels);
		texture.width = width;
		texture.height = height;
		texture.nrChannels = nrChannels;
		texture.name = filename;
		if (!texture.data)
			std::cout << "Failed to load texture: " << filename << std::endl;
		else
			std::cout << "Loaded texture: " << filename << " (w:" << width << ", h:" << height << ")" << std::endl;
		return texture;
	}

	///@brief Loads an obj (3d model) file using the tinyobj library
	static std::vector<float> loadObj(std::string filename)
	{
		std::vector<float> vertices = std::vector<float>();
		std::string inputfile = "assets/models/" + filename;
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

		if (!warn.empty()) {
			std::cout << warn << std::endl;
		}

		if (!err.empty()) {
			std::cerr << err << std::endl;
		}

		if (!ret) {
			exit(1);
		}

		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					vertices.insert(vertices.end(), { vx, vy, vz, nx, ny, nz, tx, ty }); //With textures
					//vertices.insert(vertices.end(), { vx, vy, vz }); //Without textures
				}
				index_offset += fv;
			}
		}
		return vertices;
	}

	FileHandler() {};
	~FileHandler() {};
};