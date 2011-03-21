#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>
#include <fstream>
#include <iostream>

using namespace std;

struct Vertex
{
	Vertex() {}
	Vertex(const aiVector3D& position, const aiVector3D& normal, const aiVector3D& tangent, const aiVector3D& texCoord) :
		mPosition(position),
		mNormal(normal),
		mTangent(tangent),
		mTexCoord(aiVector2D(texCoord.x, texCoord.y))
	{
	}

	aiVector3D mPosition;
	aiVector3D mNormal;
	aiVector3D mTangent;
	aiVector2D mTexCoord;
};

int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		cout << "Model Processor\n\nUsage: ModelProcessor.exe inputfile outputfile\n";
		return 0;
	}

	{
		Assimp::Importer importer;

		size_t flags =
			aiProcess_CalcTangentSpace			|
			aiProcess_GenNormals				|
			aiProcess_Triangulate				|
			aiProcess_MakeLeftHanded			|
			aiProcess_PreTransformVertices		|
			aiProcess_JoinIdenticalVertices		|
			aiProcess_OptimizeMeshes			|
			aiProcess_ImproveCacheLocality		|
			aiProcess_RemoveRedundantMaterials	|
			aiProcess_GenSmoothNormals 			|
			aiProcess_TransformUVCoords			|
			aiProcess_OptimizeGraph				|
			aiProcess_RemoveRedundantMaterials	|
			aiProcess_FindInvalidData			|
			aiProcess_FlipUVs					|
			aiProcess_SortByPType				;

		const aiScene* scene = importer.ReadFile( argv[1], flags );

		if ( !scene )
		{
			cout << "Error Building: \"" << argv[1] << "\"\n\n" << importer.GetErrorString() << endl;
			return 1;
		}

		std::ofstream fs(argv[2], ios::out|ios::binary|ios::trunc);

		if (!fs.good())
		{
			cout << "Error creating: \"" << argv[2] << "\"" << endl;
			return 1;
		}

		try
		{
			fs.write((char*)&scene->mNumMeshes, sizeof(size_t));
			for (size_t i = 0; i<scene->mNumMeshes; ++i)
			{
				aiMesh* pMesh = scene->mMeshes[i];

				if (!pMesh->HasNormals()) throw std::exception("Model has no Normals!");
				if (!pMesh->HasTangentsAndBitangents()) throw std::exception("Model has no Tangents or Bitangents!");
				if (!pMesh->HasTextureCoords(0)) throw std::exception("Model has no TexCoords!");

				fs.write((char*)&pMesh->mNumVertices, sizeof(size_t));

				Vertex * pVertices = new Vertex[pMesh->mNumVertices];
				for (size_t j = 0; j<pMesh->mNumVertices; ++j)
				{
					pVertices[j] = Vertex(pMesh->mVertices[j], pMesh->mNormals[j], pMesh->mTangents[j], pMesh->mTextureCoords[0][j]);
				}

				fs.write((char*)pVertices, sizeof(Vertex)*pMesh->mNumVertices);
				delete[] pVertices;

				size_t numIndices = pMesh->mNumFaces * 3;
				fs.write((char*)&numIndices, sizeof(size_t));
				
				for (size_t j = 0; j<pMesh->mNumFaces; ++j)
				{
					fs.write((char*)pMesh->mFaces[j].mIndices, sizeof(size_t) * pMesh->mFaces[j].mNumIndices);
				}
			}
		}
		catch (std::exception e)
		{
			cout << "Error Building: " << e.what() << endl;
			fs.close();
			return 1;
		}

		fs.close();
	}

	return 0;
}

