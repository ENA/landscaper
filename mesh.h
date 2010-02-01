#ifndef _MESH_H_
#define _MESH_H_

#include "matrixobj.h"

namespace space
{

//���b�V����`������
namespace mesh
{

	//RGB
	struct ColorRGB
	{
		double red,green,blue;
	};

	//RGBA
	struct ColorRGBA public ColorRGB
	{
		double alpha;
	};

	//�C���f�b�N�X��
	struct IndexColor : public ColorRGBA
	{
		unsigned int index;
	};

	//�e�N�X�`���t�@�C����
	struct TextureFileName
	{
		std::string texturefilename;
	};

	//���b�V���̂����̈�̖�
	struct MeshFace
	{
		std::vector<unsigned int> facevertexindices;
	};

	//�ʖ@��
	struct MeshNormals
	{
		std::vector< Matrix31<double> > normals;
		std::vector< MeshFace > meshfaces;
	};

	//�}�e���A��
	struct Material
	{
		ColorRGBA faceColor;
		double power;
		ColorRGB specularColor;
		ColorRGB emissiveColor;
		TextureFileName texfile;
	};

	//�e�N�X�`�����W
	struct MeshTextureCoords
	{
		std::vector< Matrix21<double> > texturecoords;
	};

	//�}�e���A�����X�g
	struct MeshMaterialList
	{
		std::vector<Material> materials;
		std::vector<unsigned int> faceindexes;
	};

	//�J���[���X�g
	struct MeshVertexColors
	{
		std::vector<IndexColor> indexcolors;
	};

	//���b�V��
	struct Mesh
	{
		std::vector< Matrix31<double> > vertices;
		std::vector< MeshFace > faces;
		
		MeshTextureCoords texcoords;
		MeshNormals normals;
		MeshMaterialList materials;
		MeshVertexColors vertexcolors;

		Mesh(){}
	};

}//end of namespace mesh

}//end of namespace space

#endif
