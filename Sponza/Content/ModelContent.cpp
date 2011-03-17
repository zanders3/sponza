#include "stdafx.h"

#include <stdio.h>
#include "ModelContent.h"

namespace content
{
	//-----------------------------------------------------------------------
	// Model Loading
	//-----------------------------------------------------------------------

	Model::Model(const std::string& file)
	{
		FILE * pFile = fopen(file.c_str(), "rb");

		if (pFile == NULL)
		{
			perror(" ");
		}

		fread( &m_meshCount, sizeof(size_t), 1, pFile);
		
		m_meshList = new Mesh[m_meshCount];
		for (size_t i = 0; i<m_meshCount; ++i)
			m_meshList[i].Load( pFile );

		fclose(pFile);
	}

	Model::~Model()
	{
		delete[] m_meshList;
	}

#ifdef CONTENT_PIPELINE

	//-----------------------------------------------------------------------
	// Model Generation and Saving
	//-----------------------------------------------------------------------

	char * Model::errorMessage = NULL;

	Model::Model(const aiScene* scene)
	{
		m_meshCount = scene->mNumMeshes;

		m_meshList = new Mesh[m_meshCount];
		for (size_t i = 0; i<m_meshCount; ++i)
		{
			if (EnsureMesh( scene->mMeshes[i] ))
				m_meshList[i].Load( scene->mMeshes[i] );
			else
				return;
		}
	}
	
	bool Model::EnsureMesh( const aiMesh * mesh )
	{
		if (mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
			errorMessage = "Incorrect Primitive Type!";
		else if (!mesh->HasPositions())
			errorMessage = "Missing Positions!";
		else if (!mesh->HasTextureCoords( 0 ))
			errorMessage = "Missing Texture Coords!";
		else if (!mesh->HasNormals())
			errorMessage = "Missing Normals!";
		else if (!mesh->HasTangentsAndBitangents())
			errorMessage = "Missing Tangents + Binormals!";
		else
			return true;

		return false;
	}

	void Model::Save(const char * file)
	{
		FILE * pFile;

		if (fopen_s( &pFile, file, "wb" ))
		{
			errorMessage = "File Error";
			return;
		}

		fwrite( &m_meshCount, sizeof(size_t), 1, pFile);
		for (size_t i = 0; i<m_meshCount; ++i)
			m_meshList[i].Save( pFile );

		fclose(pFile);
	}
#endif

	//-----------------------------------------------------------------------
	// Mesh Loading
	//-----------------------------------------------------------------------

	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
		delete[] vertices;
		delete[] indexBuffer;
	}

	void Mesh::Load( FILE * pFile )
	{
		fread( &vertexCount, sizeof(size_t), 1, pFile );
		vertices = new Vertex[vertexCount];
		fread( vertices, sizeof(Vertex), vertexCount, pFile );

		fread( &indexCount,  sizeof(size_t), 1, pFile );
		indexBuffer = new uint16_t[indexCount];
		fread( indexBuffer, sizeof(uint16_t), indexCount, pFile );
	}

	//-----------------------------------------------------------------------
	// Mesh Generation and Saving
	//-----------------------------------------------------------------------
#ifdef CONTENT_PIPELINE
	void Mesh::Load( const aiMesh * mesh )
	{
		vertexCount = mesh->mNumVertices;
		
		vertices = new Vertex[vertexCount];
		Vertex v;
		for (size_t i = 0; i<vertexCount; ++i)
		{
			v.position = mesh->mVertices[i];
			v.normal   = mesh->mNormals[i];
			v.binormal = mesh->mBitangents[i];
			v.tangent  = mesh->mTangents[i];
			v.texCoord = mesh->mTextureCoords[0][i];
			vertices[i] = v;
		}

		indexCount = mesh->mNumFaces * 3;

		size_t j = -1;
		indexBuffer = new uint16_t[indexCount];
		for (size_t i = 0; i<mesh->mNumFaces; ++i)
		{
			indexBuffer[++j] = mesh->mFaces[i].mIndices[0];
			indexBuffer[++j] = mesh->mFaces[i].mIndices[1];
			indexBuffer[++j] = mesh->mFaces[i].mIndices[2];
		}
	}

	void Mesh::Save( FILE * pFile )
	{
		fwrite( &vertexCount, sizeof(size_t), 1, pFile );
		fwrite( vertices, sizeof(Vertex), vertexCount, pFile );

		fwrite( &indexCount,  sizeof(size_t), 1, pFile );
		fwrite( indexBuffer, sizeof(uint16_t), indexCount, pFile );
	}
#endif
}