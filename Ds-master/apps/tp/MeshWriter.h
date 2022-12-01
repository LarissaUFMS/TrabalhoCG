#pragma once
#include "ProxyEspiral.h"


class MeshWriter
{
public:
	static void writeOBJ(cg::TriangleMesh* mesh, const char* filename);

}; // MeshWriter

