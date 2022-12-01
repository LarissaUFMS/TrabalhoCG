#include "ProxyTorcao.h"
#include "math/Vector3.h"

using namespace cg::graph;

cg::TriangleMesh*
PrimitiveProxyTorcao::torcao(Geratriz geratriz, float comprimento, float deslHorizontal, float deslVertical, int nSubdiv, float escalaInicial, float escalaFinal, float torcao, bool tampa) {

	auto bx = 2 * geratriz.escalaX();
	auto by = 2 * geratriz.escalaY();

	if (deslHorizontal < -bx)
		deslHorizontal = -bx;
	else if (deslHorizontal > bx)
		deslHorizontal = bx;

	if (deslVertical < -by)
		deslVertical = -by;
	else if (deslVertical > by)
		deslVertical = by;

	int nvGeratriz = geratriz.nVertices();
	auto vGeratriz = geratriz.vertices();
	float nPassos = nSubdiv + 1;
	int nv = nPassos * nvGeratriz;
	int nt = 2 * (nPassos - 1) * (nvGeratriz - 1); // Cada passo gera uma seção transversal da superfície com nvGeratriz adicionado à malha de triângulos

	if (tampa) {
		nv += (nvGeratriz - 1) * 2;//número de vértices para cada tampa
		nt += (nvGeratriz - 3) * 2;//número de triângulos para cada tampa
	}

	TriangleMesh::Data data;
	data.vertexCount = nv;
	data.vertices = new vec3f[nv];
	data.vertexNormals = new vec3f[nv];
	data.triangleCount = nt;
	data.triangles = new TriangleMesh::Triangle[nt];

	auto vertices = data.vertices;
	auto vertexNormals = data.vertexNormals;
	auto triangles = data.triangles;

	float x, y;

	for (int i = 0; i < nvGeratriz; i++) {
		vGeratriz[i].y += deslVertical;
		vGeratriz[i].x += deslHorizontal;
		vGeratriz[i].x *= escalaInicial;
		vGeratriz[i].y *= escalaInicial;
	}
	constexpr auto pi = math::pi<float>();

	float passoEscala = (escalaFinal - escalaInicial) / nSubdiv;
	float anguloTorcao = 2 * pi * (torcao / nSubdiv);

	for (int i = 0; i < nPassos; i++) {
		for (int j = 0; j < nvGeratriz; j++) {
			*vertices++ = *vertexNormals++ = vGeratriz[j];
			vGeratriz[j].x *= (escalaInicial + passoEscala);
			vGeratriz[j].y *= (escalaInicial + passoEscala);
			vGeratriz[j].z += comprimento / nSubdiv;
			x = vGeratriz[j].x;
			y = vGeratriz[j].y;
			vGeratriz[j].x = x * cos(anguloTorcao) - y * sin(anguloTorcao);
			vGeratriz[j].y = x * sin(anguloTorcao) + y * cos(anguloTorcao);

		}
	}

	for (int i = 0; i < nPassos - 1; i++) {
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

cg::TriangleMesh* PrimitiveProxyTorcao::torcaoPpoligon() {
	Geratriz poligono{};
	poligono.poligonoR(3, 1, 1, 0);
	return PrimitiveProxyTorcao::torcao(poligono, 10, 1, 1, 40, 1, 1, 1, true);
}