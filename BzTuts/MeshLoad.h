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




//���ļ�����.obj��
//��shapes�����ý�������״�������
///�������ش����ַ�����
//����.obj�ɹ�ʱ����true��
//������ʹ�����Ϣ���ص���err��
//'mtl_basepath'�ǿ�ѡ�ģ�����.mtl�ļ��Ļ�·����
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




