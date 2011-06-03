#pragma once
#include <stdint.h>
#include <string>

#ifdef CONTENT_PIPELINE
#include <assimp.h>
#include <aiScene.h>
#endif

namespace content
{

struct Vector3
{
	Vector3() { }
#ifdef CONTENT_PIPELINE
	Vector3( const aiVector3D& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}	
#endif

	float x, y, z;
};

struct Vector2
{
	Vector2() { }
#ifdef CONTENT_PIPELINE
	Vector2( const aiVector3D& v )
	{
		x = v.x;
		y = v.y;
	}	
#endif

	float x, y;
};

struct Vertex
{
	Vector3 position, normal, binormal, tangent;
	Vector2 texCoord;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	Vertex *			vertices;
	size_t				vertexCount;
	static const size_t vertexStride = sizeof(Vertex);

	uint16_t *			indexBuffer;
	size_t				indexCount;

#ifdef CONTENT_PIPELINE
	void Load( const aiMesh * mesh );
	void Save( FILE * file );
#endif
	void Load( FILE * file );
};

class Model
{
public:
	Model(const std::string& file);
#ifdef CONTENT_PIPELINE
	Model(const aiScene* scene);
#endif
	~Model();

	size_t		m_meshCount;
	Mesh *		m_meshList;

#ifdef CONTENT_PIPELINE
	static char * errorMessage;

	void Save(const char * file);

	bool EnsureMesh( const aiMesh * mesh );
	char * GetErrorString() { return errorMessage; }
#endif
};

}