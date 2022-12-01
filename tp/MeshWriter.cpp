#include "MeshWriter.h"
#include <filesystem>


void
MeshWriter::writeOBJ(cg::TriangleMesh* mesh, const char* filename)
{  
    FILE* file;

    fopen_s(&file, filename, "w");
    auto data = mesh->data();
    int nv = data.vertexCount;
    auto v = data.vertices;
    auto nt = data.triangleCount;
    auto t = data.triangles;
    auto vn = data.vertexNormals;

    for (int i = 0; i < nv; i++) {
        fprintf(file, "v %f %f %f\n", v[i].x, v[i].y, v[i].z);
    }
    //for (int i = 0; i < nv; i++) {
    //    fprintf(file, "vn %f %f %f\n", vn[i].x, vn[i].y, vn[i].z);
    //}
    fprintf(file, "usemtl blue\n");
    for (int i = 0; i < nt; i++)
        fprintf(file, "f %d %d %d\n", t[i].v[0]+1, t[i].v[1]+1, t[i].v[2]+1);
    
    fclose(file);
}

