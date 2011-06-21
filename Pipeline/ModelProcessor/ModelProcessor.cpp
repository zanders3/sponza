#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>
#include <fstream>
#include <iostream>
#include <hash_map>

using namespace std;

struct Vertex
{
	Vertex() {}
	Vertex(const aiVector3D& position, const aiVector3D& normal, const aiVector3D& tangent, const aiVector3D& binormal, const aiVector3D& texCoord) :
		mPosition(position),
		mNormal(normal),
		mTangent(tangent),
		mBiNormal(binormal),
		mTexCoord(aiVector2D(1.0f - texCoord.y, texCoord.x))
	{
	}

	aiVector3D mPosition;
	aiVector3D mNormal;
	aiVector3D mTangent;
	aiVector3D mBiNormal;
	aiVector2D mTexCoord;
};

std::string LoadTexture(aiMaterial* pMat, aiTextureType type)
{
	aiString path;
	if (pMat->GetTexture(type, 0, &path) == aiReturn_SUCCESS)
	{
		std::string pathStr = std::string(path.data, path.length);

		cout << "DEPENDENCY " << pathStr << endl;

		return pathStr;
	}
	else
	{
		return std::string();
	}
}

void WriteString(std::ofstream& fs, std::string& str)
{
	size_t size = str.size() + 1;
	fs.write((char*)&size, sizeof(size_t));
	fs.write(str.c_str(), sizeof(char)*str.size());
	
	static const char end = '\0';
	fs.write(&end, 1);
}

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
			aiProcess_OptimizeGraph				|
			aiProcess_FindInvalidData			|
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
			if (!scene->HasMaterials()) throw std::exception("Model has no Materials!");

			//---------------------------------------------------
			//Material Data
			//---------------------------------------------------
			fs.write((char*)&scene->mNumMaterials, sizeof(size_t));
			for (size_t i = 0; i<scene->mNumMaterials; ++i)
			{
				aiMaterial* pMat = scene->mMaterials[i];
				
				std::string diffuse = LoadTexture(pMat, aiTextureType_DIFFUSE);
				WriteString(fs, diffuse);

				std::string normal = LoadTexture(pMat, aiTextureType_NORMALS);
				
				if (normal.empty())
					normal = LoadTexture(pMat, aiTextureType_HEIGHT);

				WriteString(fs, normal);
			}

			//---------------------------------------------------
			//Mesh Data
			//---------------------------------------------------
			fs.write((char*)&scene->mNumMeshes, sizeof(size_t));
			for (size_t i = 0; i<scene->mNumMeshes; ++i)
			{
				aiMesh* pMesh = scene->mMeshes[i];

				if (!pMesh->HasNormals()) throw std::exception("Model has no Normals!");
				if (!pMesh->HasTangentsAndBitangents()) throw std::exception("Model has no Tangents or Bitangents!");
				if (!pMesh->HasTextureCoords(0)) throw std::exception("Model has no TexCoords!");

				//---------------------------------------------------
				//Material ID
				//---------------------------------------------------
				fs.write((char*)&pMesh->mMaterialIndex, sizeof(size_t));

				//---------------------------------------------------
				//Write out Vertices Data
				//---------------------------------------------------
				fs.write((char*)&pMesh->mNumVertices, sizeof(size_t));

				Vertex * pVertices = new Vertex[pMesh->mNumVertices];
				for (size_t j = 0; j<pMesh->mNumVertices; ++j)
				{
					pVertices[j] = Vertex(pMesh->mVertices[j], pMesh->mNormals[j], pMesh->mTangents[j], pMesh->mBitangents[j], pMesh->mTextureCoords[0][j]);
				}

				fs.write((char*)pVertices, sizeof(Vertex)*pMesh->mNumVertices);
				delete[] pVertices;

				//---------------------------------------------------
				//Write out Indices Data
				//---------------------------------------------------
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

