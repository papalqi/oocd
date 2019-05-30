#pragma once
#include"CoreMinimal.h"
#include <map>
#include<string>


struct material_t 
{
	std::string name;

	float ambient[3];
	float diffuse[3];
	float specular[3];
	float transmittance[3];
	float emission[3];
	float shininess;
	float ior;      // index of refraction
	float dissolve; // 1 == opaque; 0 == fully transparent
	// illumination model (see http://www.fileformat.info/format/material/)
	int illum;

	int dummy; // Suppress padding warning.

	std::string ambient_texname;            // map_Ka
	std::string diffuse_texname;            // map_Kd
	std::string specular_texname;           // map_Ks
	std::string specular_highlight_texname; // map_Ns
	std::string bump_texname;               // map_bump, bump
	std::string displacement_texname;       // disp
	std::string alpha_texname;              // map_d
	map<std::string, std::string> unknown_parameter;
};

class MaterialReader
{
public:
	MaterialReader() {}
	virtual ~MaterialReader();

	virtual bool operator()(const std::string &matId,
		std::vector<material_t> &materials,
		map<std::string, int> &matMap,
		std::string &err) = 0;
};

class MaterialFileReader : public MaterialReader {
public:
	MaterialFileReader(const std::string &mtl_basepath)
		: m_mtlBasePath(mtl_basepath) {}
	virtual ~MaterialFileReader() {}
	virtual bool operator()(const std::string &matId,
		std::vector<material_t> &materials,
		map<std::string, int> &matMap,
		std::string &err);

private:
	std::string m_mtlBasePath;
};