#include <math/Real.h>
#include <math/Vector3.h>
#include <geometry/TriangleMesh.h>
using namespace cg;

class Geratriz
{
private:
	vec3f* _vertices;
	int _nVertices;
	float _escalaX;
	float _escalaY;
public:
	Geratriz() = default;

	inline auto poligonoR(int nPontos, float escalaX, float escalaY, float rotacao);

	inline auto arcoCircunf(int nPontos, float escalaX, float escalaY, float rotacao, float theta, bool fechada);

	vec3f* vertices() {
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

class Varredura
{
private:
	TriangleMesh::Data _data{};
	TriangleMesh* _varredura{};

public:
	Varredura(Geratriz geratriz) {

		_varredura = new TriangleMesh{ std::move(_data) };
	}

	inline auto espiral(Geratriz geratriz, float deslX, float nRev, int nSubdiv, float incrementAltura, float incrementLargura, bool tampa = true);

	inline auto translacaoTorcao(Geratriz geratriz, float comprimento, float deslHorizontal, float deslVertical, int nSubdiv, float escalaInicial, float escalaFinal, float torcao, bool tampa);

};