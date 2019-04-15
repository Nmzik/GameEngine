#include "Geometry.h"

#include "YdrLoader.h"

Geometry::Geometry(const uint8_t* meshData, grmGeometry* drawGeom, Material mat, uint32_t Shader)
    : material(mat)
    , num_indices(drawGeom->IndexBufferPointer->IndicesCount)
{
    switch (Shader)
    {
        case 1530399584:  //{cutout.sps}
        case 3190732435:  //{cutout_um.sps}
        case 3959636627:  //{cutout_tnt.sps}
        case 2219447268:  //{cutout_fence.sps}
        case 3091995132:  //{cutout_fence_normal.sps}
        case 3187789425:  //{cutout_hard.sps}
        case 3339370144:  //{cutout_spec_tnt.sps}
        case 1264076685:  //{normal_cutout.sps}
        case 46387092:    //{normal_cutout_tnt.sps}
        case 748520668:   //{normal_cutout_um.sps}
        case 807996366:   //{normal_spec_cutout.sps}
        case 3300978494:  //{normal_spec_cutout_tnt.sps}
        case 582493193:   //{trees_lod.sps} //not billboarded..
        case 2322653400:  //{trees.sps}
        case 3334613197:  //{trees_tnt.sps}
        case 3192134330:  //{trees_normal.sps}
        case 1224713457:  //{trees_normal_spec.sps}
        case 1229591973:  //{trees_normal_spec_tnt.sps}
        case 4265705004:  //{trees_normal_diffspec.sps}
        case 2245870123:  //{trees_normal_diffspec_tnt.sps}
            Shader = 1;   //Transparency
            break;
        default:
            Shader = 2;  //OBJECT
            break;
    }

    ShaderName = Shader;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, VBO);
    //////////////
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 drawGeom->VertexBufferPointer->VertexCount * drawGeom->VertexBufferPointer->VertexStride,
                 &meshData[drawGeom->VertexBufferPointer->DataPointer1],
                 GL_STATIC_DRAW);

    //	const Position = 0; //	Positions
    //	const Normals = 1; //	NORMALS
    //	const Color = 2; //	COLOR
    //	const TexCoord = 3; //	TEXTCOORD

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
            //	printf("VERTEX\n");
            break;
    }

    for (auto& Attribute : *Attributes)
    {
        glEnableVertexAttribArray(Attribute.index);
        glVertexAttribPointer(Attribute.index, Attribute.size, Attribute.type, GL_FALSE, Attribute.stride, Attribute.pointer);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 drawGeom->IndexBufferPointer->IndicesCount * sizeof(uint16_t),
                 &meshData[drawGeom->IndexBufferPointer->IndicesPointer],
                 GL_STATIC_DRAW);  //	16 BIT INDICES max 65536
}

Geometry::~Geometry()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBO);
}
