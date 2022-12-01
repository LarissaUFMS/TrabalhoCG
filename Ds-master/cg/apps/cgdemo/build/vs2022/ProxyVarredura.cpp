#include "ProxyVarredura.h"
#include "math/Real.h"
#include "iostream"

inline auto
Geratriz::poligonoR(int nPontos, float escalaX, float escalaY, float rotacao)
{
	float x, y, z, _x, _y;
	constexpr auto pi = math::pi<float>();
	auto stepAngulo = 2 * pi / nPontos;
	auto angulo = pi / 2;
	rotacao = pi * rotacao;
	_nVertices = nPontos + 1;
	_vertices = new vec3f[_nVertices];
	_escalaX = escalaX;
	_escalaY = escalaY;
	for (int i = 0; i < _nVertices; i++, angulo += stepAngulo) {
		x = cos(angulo) * escalaX;
		y = sin(angulo) * escalaY;
		z = 0.0f;
		//rotação em torno de z
		_x = x * cos(rotacao) - y * sin(rotacao);
		_y = x * sin(rotacao) + y * cos(rotacao);
		x = _x;
		y = _y;
		_vertices[i] = { x, y, z };
	}
	return _vertices;

}

inline auto
Geratriz::arcoCircunf(int nPontos, float escalaX, float escalaY, float rotacao, float theta, bool fechada)
{
	float x, y, z, _x, _y;
	constexpr auto pi = math::pi<float>();
	auto stepAngulo = theta * pi / (nPontos - 1);
	auto angulo = ((1 - theta) / 2) * pi;
	rotacao = rotacao * pi;
	_nVertices = nPontos;
	_escalaX = escalaX;
	_escalaY = escalaY;
	if (fechada)
		_nVertices++;
	_vertices = new vec3f[_nVertices];
	for (int i = 0; i < nPontos; i++, angulo += stepAngulo) {
		x = cos(angulo) * escalaX;
		y = sin(angulo) * escalaY;
		z = 0.0f;
		//rotação em torno de z
		_x = x * cos(rotacao) - y * sin(rotacao);
		_y = x * sin(rotacao) + y * cos(rotacao);
		x = _x;
		y = _y;
		_vertices[i] = { x, y, z };
	}
	if (fechada)
		_vertices[nPontos] = _vertices[0];

	return _vertices;
}


inline auto
VarreduraProxy::espiral(Geratriz geratriz, float deslX, float nRev, int nSubdiv, float deltinhaAltura, float deltinhaLargura, bool tampa)
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

inline auto
VarreduraProxy::translacaoTorcao(Geratriz geratriz, float comprimento, float deslHorizontal, float deslVertical, int nSubdiv, float escalaInicial, float escalaFinal, float torcao, bool tampa) {

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

	float x, y, z;

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

inline auto
VarreduraProxy::executaVarredura(Geratriz geratriz, bool tipoVarredura, ParametrosVarredura p){

	if (tipoVarredura) {
		return espiral(geratriz) 
	}
	else {

	}
	
}  

