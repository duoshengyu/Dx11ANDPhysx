#ifndef GEOMETRYDATA_H_
#define GEOMETRYDATA_H_
//------------------------------------------------------------------------------------
// generate cube mesh data and ball mesh data
//------------------------------------------------------------------------------------
#include <vector>
#include <DirectXMath.h>

typedef unsigned short USHORT;
using namespace DirectX;

namespace MyMeshData
{
	struct Vertex
	{
		Vertex() {}
		Vertex(const XMFLOAT3& p, const XMFLOAT3& n)
			: Position(p), Normal(n){}

		Vertex(
			float px, float py, float pz,
			float nx, float ny, float nz)
			: Position(px, py, pz), Normal(nx, ny, nz){}
	
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
	};
	
	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<USHORT> Indices;
	};
	//ground
	static void CreatePlane(float width, float height, int sliceCount, MeshData& meshData)
	{
		meshData.Vertices.clear();
		meshData.Indices.clear();
		float intervalW = width / sliceCount;
		float intervalH = height / sliceCount;
		float w2 = 0.5f*width;
		float h2 = 0.5f*height;

		for (USHORT i = 0; i <= sliceCount; i++)
		{
			for (USHORT j = 0; j <= sliceCount; j++)
			{
				meshData.Vertices.push_back(Vertex(XMFLOAT3(0.0f, w2 - i * intervalH, h2 - j * intervalW), XMFLOAT3(1.0f, 0.0f, 0.0f)));
			}
		}

		USHORT base = sliceCount + 1;

		for (USHORT i = 0; i < sliceCount; i++)
		{
			for (USHORT j = 0; j < sliceCount; j++)
			{
				meshData.Indices.push_back(i * base + j);
				meshData.Indices.push_back((i + 1) * base + j);
				meshData.Indices.push_back(i * base + j +1);

				meshData.Indices.push_back(i * base + j + 1);
				meshData.Indices.push_back((i + 1) * base + j);
				meshData.Indices.push_back((i + 1) * base + j + 1);
			}
		}
	}
	//cube
	static void CreateBox(float width, float height, float depth, MeshData& meshData)
	{
		meshData.Vertices.clear();
		meshData.Indices.clear();
		//
		// Create the vertices.
		//

		float w2 = 0.5f*width;
		float h2 = 0.5f*height;
		float d2 = 0.5f*depth;

		Vertex vertices[] =
		{
			{ XMFLOAT3(-w2,  h2, -d2),  XMFLOAT3(-1.0f, 1.0f, -1.0f) },
			{ XMFLOAT3( w2,  h2, -d2),   XMFLOAT3(1.0f, 1.0f, -1.0f) },
			{ XMFLOAT3( w2,  h2,  d2),    XMFLOAT3(1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-w2,  h2,  d2),   XMFLOAT3(-1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-w2, -h2, -d2), XMFLOAT3(-1.0f, -1.0f, -1.0f) },
			{ XMFLOAT3( w2, -h2, -d2),  XMFLOAT3(1.0f, -1.0f, -1.0f) },
			{ XMFLOAT3( w2, -h2,  d2),   XMFLOAT3(1.0f, -1.0f, 1.0f) },
			{ XMFLOAT3(-w2, -h2,  d2),  XMFLOAT3(-1.0f, -1.0f, 1.0f) }
		};

		meshData.Vertices.assign(&vertices[0], &vertices[8]);

		//
		// Create the indices.
		//
		// Create index buffer
		USHORT indices[] =
		{
			3,1,0,
			2,1,3,

			0,5,4,
			1,5,0,

			3,4,7,
			0,4,3,

			1,6,5,
			2,6,1,

			2,7,6,
			3,7,2,

			6,4,5,
			7,4,6,
		};

		meshData.Indices.assign(&indices[0], &indices[36]);
	}

	static void CreateSphere(float radius, USHORT sliceCount, USHORT stackCount, MeshData& meshData)
	{
		meshData.Vertices.clear();
		meshData.Indices.clear();

		//
		// Compute the vertices stating at the top pole and moving down the stacks.
		//
		Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f);
		Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f);

		meshData.Vertices.push_back(topVertex);

		float phiStep = XM_PI / stackCount;
		float thetaStep = 2.0f*XM_PI / sliceCount;

		// Compute vertices for each stack ring
		for (USHORT i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i*phiStep;

			// Vertices of ring.
			for (USHORT j = 0; j <= sliceCount; ++j)
			{
				float theta = j*thetaStep;

				Vertex v;

				// spherical to cartesian
				v.Position.x = radius*sinf(phi)*cosf(theta);
				v.Position.y = radius*cosf(phi);
				v.Position.z = radius*sinf(phi)*sinf(theta);

				XMVECTOR p = XMLoadFloat3(&v.Position);
				XMStoreFloat3(&v.Normal, XMVector3Normalize(p));
				meshData.Vertices.push_back(v);
			}
		}

		meshData.Vertices.push_back(bottomVertex);

		//
		// Compute indices for top stack.  The top stack was written first to the vertex buffer
		// and connects the top pole to the first ring.
		//

		for (USHORT i = 1; i <= sliceCount; ++i)
		{
			meshData.Indices.push_back(0);
			meshData.Indices.push_back(i + 1);
			meshData.Indices.push_back(i);
		}
		//
		// Compute indices for inner stacks (not connected to poles).
		//

		// Offset the indices to the index of the first vertex in the first ring.
		// This is just skipping the top pole vertex.
		USHORT baseIndex = 1;
		USHORT ringVertexCount = sliceCount + 1;
		for (USHORT i = 0; i < stackCount - 2; ++i)
		{
			for (USHORT j = 0; j < sliceCount; ++j)
			{
				meshData.Indices.push_back(baseIndex + i*ringVertexCount + j);
				meshData.Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
				meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

				meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
				meshData.Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
				meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
			}
		}

		//
		// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
		// and connects the bottom pole to the bottom ring.
		//

		// South pole vertex was added last.
		USHORT southPoleIndex = (USHORT)meshData.Vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (USHORT i = 0; i < sliceCount; ++i)
		{
			meshData.Indices.push_back(southPoleIndex);
			meshData.Indices.push_back(baseIndex + i);
			meshData.Indices.push_back(baseIndex + i + 1);
		}
	}
}

#endif
