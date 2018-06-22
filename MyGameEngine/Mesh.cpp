#include "Mesh.h"

Mesh::Mesh(const uint8_t* meshData, uint64_t VertexPointer, uint32_t VertexSize, uint64_t IndicesPointer, uint32_t IndicesSize, VertexType type, Material mat) : material(mat), num_indices(IndicesSize)
{
	VAO = MeshManager::GetVAO();
	glBindVertexArray(VAO);

	VBO = MeshManager::GetVBO();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, &meshData[VertexPointer], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //Positions
	glEnableVertexAttribArray(1); //NORMALS
	glEnableVertexAttribArray(2); //COLOR
	glEnableVertexAttribArray(3); //TEXTCOORD

	switch (type)
	{
	case Default:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (GLvoid*)12);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 36, (GLvoid*)24);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 36, (GLvoid*)28);
		break;
	case DefaultEx:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 52, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 52, (GLvoid*)12);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 52, (GLvoid*)24);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 52, (GLvoid*)28);
		break;
	case PNCCT:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 40, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 40, (GLvoid*)12);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 40, (GLvoid*)24);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 40, (GLvoid*)32);
		break;
	case PNCCTTTT:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 64, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 64, (GLvoid*)12);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 64, (GLvoid*)24);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 64, (GLvoid*)32);
		break;
	case PCCNCCTTX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 72, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 72, (GLvoid*)20);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 72, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 72, (GLvoid*)40);
		break;
	case PCCNCCT:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 48, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 48, (GLvoid*)20);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 48, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 48, (GLvoid*)40);
		break;
	case PNCTTTX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 68, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 68, (GLvoid*)12);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 68, (GLvoid*)24);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 68, (GLvoid*)28);
		break;
	case PNCTTX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 60, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 60, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 60, (GLvoid*)28);
		break;
	case PNCTTTX_2:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 68, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 68, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 68, (GLvoid*)28);
		break;
	case PNCTTTX_3:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 68, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 68, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 68, (GLvoid*)28);
		break;
	case PNCCTTX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 64, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 64, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 64, (GLvoid*)32);
		break;
	case PNCCTTX_2:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 64, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 64, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 64, (GLvoid*)32);
		break;
	case PNCCTTTX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 72, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 72, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 72, (GLvoid*)32);
		break;
	case PCCNCCTX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 64, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 64, (GLvoid*)20);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 64, (GLvoid*)40);
		break;
	case PCCNCTX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 60, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 60, (GLvoid*)20);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 60, (GLvoid*)36);
		break;
	case PCCNCT:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 44, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 44, (GLvoid*)20);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 44, (GLvoid*)36);
		break;
	case PNCCTT:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 48, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 48, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 48, (GLvoid*)32);
		break;
	case PNCCTX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 56, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 56, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 56, (GLvoid*)32);
		break;
	case PCT:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, nullptr);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 24, (GLvoid*)16);
		break;
	case PT:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, nullptr);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 20, (GLvoid*)12);
		break;
	case PTT:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, nullptr);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 28, (GLvoid*)12);
		break;
	case PNC:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 28, (GLvoid*)12);
		//glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 28, (GLvoid*)24);
		break;
	case PC:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 16, nullptr);
		break;
	case PCC:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, nullptr);
		break;
	case PCCH2H4:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, (GLvoid*)12);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 32, (GLvoid*)16);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 32, (GLvoid*)20);
		break;
	case PNCH2:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, (GLvoid*)12);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 32, (GLvoid*)24);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 32, (GLvoid*)28);
		break;
	case PNCTTTTX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 76, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 76, (GLvoid*)12);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 76, (GLvoid*)24);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 76, (GLvoid*)28);
		break;
		/*case PNCTTTT:
		break;
		case PCCNCCTT:
		break;
		case PCTT:
		break;
		case PCCCCT:
		break;
		case PCCNC:
		break;*/
	case PCCNCTT:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 52, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 52, (GLvoid*)20);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 52, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 52, (GLvoid*)36);
		break;
	case PCCNCTTX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 68, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 68, (GLvoid*)20);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 68, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 68, (GLvoid*)36);
		break;
	case PCCNCTTT:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 60, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 60, (GLvoid*)20);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 60, (GLvoid*)12);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 60, (GLvoid*)36);
		break;
		/*case PNCTT:
		break;
		case PNCTTT:
		break;*/
	default:
		printf("VERTEX\n");
		break;
	}

	EBO = MeshManager::GetVBO();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesSize * sizeof(uint16_t), &meshData[IndicesPointer], GL_STATIC_DRAW); //16 BIT INDICES max 65536
}

void Mesh::Cleanup()
{
	MeshManager::VAOs.push_back(VAO);
	MeshManager::VBOs.push_back(VBO);
	MeshManager::VBOs.push_back(EBO);
}

Mesh::~Mesh()
{

}

void Mesh::Draw(Shader* shader)
{
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.diffuseTextureID);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);
}