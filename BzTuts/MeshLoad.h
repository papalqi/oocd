#pragma once


#include"OMaterial.h"
#include <string>
#include <vector>
#include <map>





typedef struct {
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texcoords;
	std::vector<unsigned int> indices;
	std::vector<int> material_ids; // per-mesh material ID
} mesh_t;

typedef struct {
	std::string name;
	mesh_t mesh;
} shape_t;




//从文件加载.obj。
//“shapes”将用解析的形状数据填充
///函数返回错误字符串。
//加载.obj成功时返回true。
//将警告和错误消息返回到“err”
//'mtl_basepath'是可选的，用于.mtl文件的基路径。
bool LoadObj(std::vector<shape_t> &shapes,       // [output]
	std::vector<material_t> &materials, // [output]
	std::string& err,                   // [output]
	const char *filename, const char *mtl_basepath = NULL);


bool LoadObj(std::vector<shape_t> &shapes,       // [output]
	std::vector<material_t> &materials, // [output]
	std::string& err,                   // [output]
	std::istream &inStream, MaterialReader &readMatFn);

/// Loads materials into std::map
void LoadMtl(std::map<std::string, int> &material_map, // [output]
	std::vector<material_t> &materials,       // [output]
	std::istream &inStream);




