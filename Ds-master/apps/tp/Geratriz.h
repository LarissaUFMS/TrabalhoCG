#pragma once
#include "math/Vector3.h"


class Geratriz
{
private:
	cg::vec3f* _vertices;
	int _nVertices;
	float _escalaX;
	float _escalaY;
public:
	int _nPontos = 3;
	float _escalaXf = 1;
	float _escalaYf = 1;
	float _rotacao = 0;
	float _theta = 1;
	bool _fechada = true;

	inline cg::vec3f* poligonoR(int nPontos, float escalaX, float escalaY, float rotacao);

	inline cg::vec3f* arcoCircunf(int nPontos, float escalaX, float escalaY, float rotacao, float theta, bool fechada);

	cg::vec3f* vertices() {
		return _vertices;
	}

	int nVertices() {
		return _nVertices;
	}
	float escalaX() {
		return _escalaX;
	}
	float escalaY() {
		return _escalaY;
	}
};