#include <math/Real.h>
#include <math/Vector3.h>
#include <graph/PrimitiveProxy.h>
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

class ParametrosVarredura
{
public:
	union {
		struct espiral {
			float _deslX;
			float _nRev;
			int _nSubdiv;
			float _incrementAltura;
			float _incrementLargura;
			bool _tampa;
		};

		struct torcao {
			float _comprimento;
			float _deslHorizontal;
			float _deslVertical;
			int _nSubdiv;
			float _escalaInicial;
			float _escalaFinal;
			float _torcao;
			bool _tampa;
		};
	};
	struct geratriz {
		int nPontos;
		float escalaX;
		float escalaY;
		float rotacao;
		float theta;
		bool fechada;
	};
};

class EspiralProxy: public graph::PrimitiveProxy
{
private:	
	TriangleMesh::Data _data{};
	TriangleMesh* _varredura{};

	VarreduraProxy(Geratriz geratriz, bool tipoVarredura, ParametrosVarredura parametros) :
		_varredura{ new TriangleMesh{ std::move(_data) } },
		PrimitiveProxy{ *new TriangleMeshMapper{executaVarredura(geratriz, tipoVarredura, parametros)} }	
	{
		// do nothing
	}

public:

	inline auto executaVarredura(Geratriz geratriz, bool tipoVarredura, ParametrosVarredura parametros);
	
	inline auto espiral(Geratriz geratriz, float deslX, float nRev, int nSubdiv, float incrementAltura, float incrementLargura, bool tampa);

	inline auto translacaoTorcao(Geratriz geratriz, float comprimento, float deslHorizontal, float deslVertical, int nSubdiv, float escalaInicial, float escalaFinal, float torcao, bool tampa);

	static auto New(const Geratriz& geratriz, const std::string& varreduraNome, ParametrosVarredura parametros)
	{
		return new VarreduraProxy{geratriz, varreduraNome, parametros};
	}

	void setMesh(const TriangleMesh& mesh, const std::string& meshName)
	{
		((TriangleMeshMapper*)PrimitiveProxy::mapper())->setMesh(mesh);
	}
};
