#include "ProxyEspiral.h"
#include "math/Vector3.h"

using namespace cg::graph;


cg::TriangleMesh*
PrimitiveProxyEspiral::espiral(Geratriz geratriz, float deslX, float nRev, int nSubdiv, float deltinhaAltura, float deltinhaLargura, bool tampa)
{
	constexpr auto pi = math::pi<float>();

	auto bx = 2 * geratriz.escalaX();
	auto by = 2 * geratriz.escalaY();

	if (deslX < 2 * bx)
		deslX = 2 * bx;
	if (deltinhaAltura < by && nRev > 1)
		deltinhaAltura = by;
	if (deltinhaLargura < bx && nRev > 1)
		deltinhaLargura = bx;

	auto deltaAltura = deltinhaAltura * nRev;
	auto deltaLargura = deltinhaLargura * nRev;

	auto nvGeratriz = geratriz.nVertices(); //numero de vertices da geratriz

	int nRotacoes = math::max(std::floor(nRev + 0.5), 1.0) * nSubdiv;//numero de rotacoes
	int nPassos = nRotacoes + 1;//numero de passos
	float theta = 0.0f;//angulo inicial para a rotação em torno de y
	auto passoTheta = 2 * pi / nSubdiv;
	auto nv = nvGeratriz * nPassos;//numero de vertices
	int nt = 2 * (nvGeratriz - 1) * nRotacoes;//numero de triangulos

	if (tampa) {
		nv += (nvGeratriz - 1) * 2;//número de vértices para cada tampa
		nt += (nvGeratriz - 3) * 2;//número de triângulos para cada tampa
	}

	deslX = (deslX - bx) / 2;

	TriangleMesh::Data data;
	data.vertexCount = nv;
	data.vertices = new vec3f[nv];
	data.vertexNormals = new vec3f[nv];
	data.triangleCount = nt;
	data.triangles = new TriangleMesh::Triangle[nt];

	auto vertices = data.vertices;
	auto vertexNormals = data.vertexNormals;
	auto triangles = data.triangles;

	vec3f* vGeratriz = geratriz.vertices();
	float x, z;

	for (int j = 0; j < nvGeratriz; j++) {
		vGeratriz[j].x += deslX;
	}
	// long index = 0;
	for (int i = 0; i < nPassos; i++) {
		for (int j = 0; j < nvGeratriz; j++) {

			x = vGeratriz[j].x;
			z = vGeratriz[j].z;

			//std::cout << "INDICE [" << index++ << "] == " << vGeratriz[j].x << " " << vGeratriz[j].y << " " << vGeratriz[j].z << '\n';
			*vertices++ = *vertexNormals++ = vGeratriz[j];


			//Rotação em torno de Y
			vGeratriz[j].x = cos(passoTheta) * x - sin(passoTheta) * z; //x = cos(angulo)  * x + sen(angulo) * z;
			vGeratriz[j].z = sin(passoTheta) * x + cos(passoTheta) * z; //z = -sen(angulo) * x+ cos(angulo) * z;

			vGeratriz[j].x += deltaLargura / nRotacoes;
			vGeratriz[j].y += deltaAltura / nRotacoes;
		}
	}

	for (int i = 0; i < nRotacoes; i++) {
		auto indiceGeratriz0 = i * nvGeratriz;
		// 0
		auto indiceGeratriz1 = indiceGeratriz0 + nvGeratriz;
		// 5

		for (int j = 0; j < nvGeratriz - 1; j++, indiceGeratriz0++, indiceGeratriz1++) {


			triangles++->setVertices(indiceGeratriz0, indiceGeratriz1, indiceGeratriz0 + 1);
			// 0 5 1
			triangles++->setVertices(indiceGeratriz0 + 1, indiceGeratriz1, indiceGeratriz1 + 1);
			// 1 5 6
		}

	}

	auto ultimaGeratriz = (vertices - nvGeratriz);
	if (tampa) {
		auto normalTampa0 = (data.vertices[2] - data.vertices[0]).cross(data.vertices[1] - data.vertices[0]);
		for (int i = 0; i < nvGeratriz - 1; i++) {
			*vertices++ = data.vertices[i];
			*vertexNormals++ = normalTampa0;
		}

		auto normalTampa1 = normalTampa0 * (-1);
		for (int k = 0; k < nvGeratriz - 1; k++) {
			*vertices++ = *(ultimaGeratriz++);
			*vertexNormals++ = normalTampa1;
		}
		int i = (nPassos)*nvGeratriz;
		int k = i + nvGeratriz - 1;
		for (int j = 1; j < nvGeratriz - 2; j++) {
			triangles++->setVertices(i, i + j, i + j + 1);
			triangles++->setVertices(k, k + j, k + j + 1);
		}
	}

	return new TriangleMesh{ std::move(data) };
}

cg::TriangleMesh* PrimitiveProxyEspiral::espiralPpoligon() {
	Geratriz poligono{};
	poligono.poligonoR(3, 1, 1, 0);
	return PrimitiveProxyEspiral::espiral(poligono, 0, 2, 40, 5, 5, true);
	}

