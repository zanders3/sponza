#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>
#include <fstream>
#include <iostream>

using namespace std;

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
			cerr << "Error Building: \"" << argv[1] << "\"\n\n" << importer.GetErrorString() << endl;
			return 1;
		}

		std::fstream fs(argv[2], std::ios::binary | std::ios::out);

		if (fs.rdstate() & ifstream::badbit)
		{
			cerr << "Error creating: \"" << argv[2] << "\"" << endl;
			return 1;
		}

		fs << scene->mNumMeshes;
		for (size_t i = 0; i<scene->mNumMeshes; ++i)
		{
			aiMesh* pMesh = scene->mMeshes[i];

			fs << pMesh->mNumVertices;
			for (size_t j = 0; j<pMesh->mNumVertices; ++j)
			{
				fs << pMesh->mVertices[j].x << pMesh->mVertices[j].y << pMesh->mVertices[j].z;
				fs << pMesh->mNormals[j].x  << pMesh->mNormals[j].y  << pMesh->mNormals[j].z;
				fs << pMesh->mTangents[j].x << pMesh->mTangents[j].y << pMesh->mTangents[j].z;
				fs << pMesh->mBitangents[j].x << pMesh->mBitangents[j].y << pMesh->mBitangents[j].z;
				fs << pMesh->mTextureCoords[0][j].x << pMesh->mTextureCoords[0][j].y;
			}
		}

		fs.close();
	}

	return 0;
}

