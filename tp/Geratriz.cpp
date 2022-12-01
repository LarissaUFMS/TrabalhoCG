#include "Geratriz.h"
#include "math/Real.h"

inline cg::vec3f* Geratriz::poligonoR(int nPontos, float escalaX, float escalaY, float rotacao)
{
	float x, y, z, _x, _y;
	constexpr auto pi = cg::math::pi<float>();
	auto stepAngulo = 2 * pi / nPontos;
	auto angulo = pi / 2;
	rotacao = pi * rotacao;
	_nVertices = nPontos + 1;
	_vertices = new cg::vec3f[_nVertices];
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

inline cg::vec3f* Geratriz::arcoCircunf(int nPontos, float escalaX, float escalaY, float rotacao, float theta, bool fechada)
{
	float x, y, z, _x, _y;
	constexpr auto pi = cg::math::pi<float>();
	auto stepAngulo = theta * pi / (nPontos - 1);
	auto angulo = ((1 - theta) / 2) * pi;
	rotacao = rotacao * pi;
	_nVertices = nPontos;
	_escalaX = escalaX;
	_escalaY = escalaY;
	if (fechada)
		_nVertices++;
	_vertices = new cg::vec3f[_nVertices];
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
