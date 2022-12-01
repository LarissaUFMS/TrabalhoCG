#pragma once
#include "graph/PrimitiveProxy.h"
#include "geometry/TriangleMesh.h"
#include "Geratriz.h"
#include "Geratriz.cpp"

namespace cg
{ // begin namespace cg

namespace graph
{ // begin namespace graph



class PrimitiveProxyEspiral final : public PrimitiveProxy
{
public:
	bool _tipoPoligono;//Se true é Polígono se não é falso
	float _deslX;
	float _nRev;
	int _nSubdiv;
	float _deltinhaAltura;
	float _deltinhaLargura;
	bool _tampa;
	Geratriz _geratriz;

	int _nPassos;

	static auto New()
	{
		TriangleMesh* mesh = espiralPpoligon();

		return new PrimitiveProxyEspiral{ *mesh };
	}

	static TriangleMesh* espiral(Geratriz geratriz, float deslX, float nRev, int nSubdiv, float deltinhaAltura, float deltinhaLargura, bool tampa);

	static TriangleMesh* espiralPpoligon();

	void atualizarMesh() {

		if(_tipoPoligono)
			_geratriz.poligonoR(_geratriz._nPontos, _geratriz._escalaXf, _geratriz._escalaYf, _geratriz._rotacao);
		else
			_geratriz.arcoCircunf(_geratriz._nPontos, _geratriz._escalaXf, _geratriz._escalaYf, _geratriz._rotacao, _geratriz._theta, _geratriz._fechada);
		TriangleMesh* mesh = espiral(_geratriz ,_deslX, _nRev, _nSubdiv, _deltinhaAltura, _deltinhaLargura, _tampa);

		((TriangleMeshMapper*)PrimitiveProxy::mapper())->setMesh(*mesh);
		//_meshName = "Spiral";
	}

	auto getVarredura() {
		return _varredura;
	}

private:
	TriangleMesh::Data _data{};
	TriangleMesh* _varredura{};
	

	PrimitiveProxyEspiral(const TriangleMesh& varredura) :
		PrimitiveProxy{ *new TriangleMeshMapper{varredura} }
		{
			_tipoPoligono = true;//Se true é Polígono se não é falso
			_deslX = 0;
			_nRev = 2;
			_nSubdiv = 40;
			_deltinhaAltura = 5;
			_deltinhaLargura = 5;
			_tampa = true;

		}

};




} // end namespace graph

} // end namespace cg