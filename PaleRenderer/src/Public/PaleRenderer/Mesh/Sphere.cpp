#include "stdafx.h"
#include "PaleRenderer/Mesh/Sphere.h"

namespace PaleRdr
{
	std::vector<std::shared_ptr<IMesh>> Sphere::getMeshes()
	{
		if (!m_bInit)
		{
			__calculateSphere();
			m_bInit = false;
		}
		std::vector<std::shared_ptr<IMesh>> meshes;
		std::vector<CVertex> vertices;
		for (unsigned int i = 0; i < m_Position.size(); ++i)
		{
			CVertex vertex = { m_Position[i], m_Normal[i], m_TexCoord[i], glm::vec3(0.0), glm::vec3(0.0) };
			vertices.push_back(vertex);
		}

		meshes.push_back(IMesh::Create(vertices, m_Indices, 0, GL_TRIANGLE_STRIP));
		return meshes;
	}

	void Sphere::__calculateSphere()
	{
		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359f;
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				m_Position.push_back(glm::vec3(xPos, yPos, zPos));
				m_TexCoord.push_back(glm::vec2(xSegment, ySegment));
				m_Normal.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					m_Indices.push_back(y * (X_SEGMENTS + 1) + x);
					m_Indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					m_Indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					m_Indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
	}
}