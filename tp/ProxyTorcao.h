#pragma once
#include "graph/PrimitiveProxy.h"
#include "geometry/TriangleMesh.h"
#include "math/Vector3.h"
#include "Geratriz.h"

namespace cg
{ // begin namespace cg

	namespace graph
	{ // begin namespace graph
		class PrimitiveProxyTorcao final : public PrimitiveProxy
		{
		public:
			bool _tipoPoligono;//Se true é Polígono se não é falso
			Geratriz _geratriz;
			float _comprimento;
			float _deslHorizontal;
			float _deslVertical;
			int _nSubdiv;
			float _escalaInicial;
			float _escalaFinal;
			float _torcao;
			bool _tampa;

			static auto New()
			{
				TriangleMesh* mesh = torcaoPpoligon();

				return new PrimitiveProxyTorcao{ *mesh };
			}

			static TriangleMesh* torcao(Geratriz geratriz, float comprimento, float deslHorizontal, float deslVertical, int nSubdiv, float escalaInicial, float escalaFinal, float torcao, bool tampa);

			static TriangleMesh* torcaoPpoligon();

			void atualizarMesh() {

				_geratriz.poligonoR(3, 1, 1, 0);
				TriangleMesh* mesh = torcao(_geratriz, _comprimento, _deslHorizontal, _deslVertical, _nSubdiv, _escalaInicial, _escalaFinal, _torcao, _tampa);

				((TriangleMeshMapper*)PrimitiveProxy::mapper())->setMesh(*mesh);
				//_meshName = "Spiral";
			}

		private:
			TriangleMesh::Data _data{};
			TriangleMesh* _varredura{};


			PrimitiveProxyTorcao(const TriangleMesh& varredura) :
				PrimitiveProxy{ *new TriangleMeshMapper{varredura} }
			{
				_tipoPoligono = true;//Se true é Polígono se não é falso
				_comprimento = 10;
				_deslHorizontal = 1;
				_deslVertical = 1;
				_nSubdiv = 40;
				_escalaInicial = 1;
				_escalaFinal = 1;
				_torcao = 1;
				_tampa = true;

			}


		};
	}
}
