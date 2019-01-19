#include "Mesh.h"
#include "YdrLoader.h"

Mesh::Mesh(const uint8_t* meshData, grmGeometry* drawGeom, Material mat)
	: material(mat)
	, num_indices(drawGeom->IndexBufferPointer->IndicesCount)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(2, VBO);
	//////////////
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, drawGeom->VertexBufferPointer->VertexCount * drawGeom->VertexBufferPointer->VertexStride, &meshData[drawGeom->VertexBufferPointer->DataPointer1], GL_STATIC_DRAW);

	//const Position = 0; //	Positions
	//const Normals = 1; //	NORMALS
	//const Color = 2; //	COLOR
	//const TexCoord = 3; //	TEXTCOORD

	const std::vector<VertexAttributes>* Attributes;

	switch (drawGeom->VertexBufferPointer->InfoPointer->Flags)
	{
		case Default:
			Attributes = &DefaultAttrib;
			break;
		case DefaultEx:
			Attributes = &DefaultExAttrib;
			break;
		case PNCCT:
			Attributes = &PNCCTAttrib;
			break;
		case PNCCTTTT:
			Attributes = &PNCCTTTTAttrib;
			break;
		case PCCNCCTTX:
			Attributes = &PCCNCCTTXAttrib;
			break;
		case PCCNCCT:
			Attributes = &PCCNCCTAttrib;
			break;
		case PNCTTTX:
			Attributes = &PNCTTTXAttrib;
			break;
		case PNCTTX:
			Attributes = &PNCTTXAttrib;
			break;
		case PNCTTTX_2:
			Attributes = &PNCTTTX_2Attrib;
			break;
		case PNCTTTX_3:
			Attributes = &PNCTTTX_3Attrib;
			break;
		case PNCCTTX:
			Attributes = &PNCCTTXAttrib;
			break;
		case PNCCTTX_2:
			Attributes = &PNCCTTX_2Attrib;
			break;
		case PNCCTTTX:
			Attributes = &PNCCTTTXAttrib;
			break;
		case PCCNCCTX:
			Attributes = &PCCNCCTXAttrib;
			break;
		case PCCNCTX:
			Attributes = &PCCNCTXAttrib;
			break;
		case PCCNCT:
			Attributes = &PCCNCTAttrib;
			break;
		case PNCCTT:
			Attributes = &PNCCTTAttrib;
			break;
		case PNCCTX:
			Attributes = &PNCCTXAttrib;
			break;
		case PCT:
			Attributes = &PCTAttrib;
			break;
		case PT:
			Attributes = &PTAttrib;
			break;
		case PTT:
			Attributes = &PTTAttrib;
			break;
		case PNC:
			Attributes = &PNCAttrib;
			break;
		case PC:
			Attributes = &PCAttrib;
			break;
		case PCC:
			Attributes = &PCCAttrib;
			break;
		case PCCH2H4:
			Attributes = &PCCH2H4Attrib;
			break;
		case PNCH2:
			Attributes = &PNCH2Attrib;
			break;
		case PNCTTTTX:
			Attributes = &PNCTTTTXAttrib;
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
			Attributes = &PCCNCTTAttrib;
			break;
		case PCCNCTTX:
			Attributes = &PCCNCTTXAttrib;
			break;
		case PCCNCTTT:
			Attributes = &PCCNCTTTAttrib;
			break;
			/*case PNCTT:
			break;
			case PNCTTT:
			break;*/
		default:
			Attributes = &WaterAttrib;
			//printf("VERTEX\n");
			break;
	}

	for (auto& Attribute : *Attributes)
	{
		glEnableVertexAttribArray(Attribute.index);
		glVertexAttribPointer(Attribute.index, Attribute.size, Attribute.type, GL_FALSE, Attribute.stride, Attribute.pointer);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, drawGeom->IndexBufferPointer->IndicesCount * sizeof(uint16_t), &meshData[drawGeom->IndexBufferPointer->IndicesPointer], GL_STATIC_DRAW); //	16 BIT INDICES max 65536
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, VBO);
}

void Mesh::Draw(GLuint tex)
{
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.diffuseTextureID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);
}
