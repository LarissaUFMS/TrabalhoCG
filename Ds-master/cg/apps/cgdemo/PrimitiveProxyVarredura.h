//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2022 Paulo Pagliosa.                              |
//|                                                                 |
//| This software is provided 'as-is', without any express or       |
//| implied warranty. In no event will the authors be held liable   |
//| for any damages arising from the use of this software.          |
//|                                                                 |
//| Permission is granted to anyone to use this software for any    |
//| purpose, including commercial applications, and to alter it and |
//| redistribute it freely, subject to the following restrictions:  |
//|                                                                 |
//| 1. The origin of this software must not be misrepresented; you  |
//| must not claim that you wrote the original software. If you use |
//| this software in a product, an acknowledgment in the product    |
//| documentation would be appreciated but is not required.         |
//|                                                                 |
//| 2. Altered source versions must be plainly marked as such, and  |
//| must not be misrepresented as being the original software.      |
//|                                                                 |
//| 3. This notice may not be removed or altered from any source    |
//| distribution.                                                   |
//|                                                                 |
//[]---------------------------------------------------------------[]
//
// OVERVIEW: PrimitiveProxy.h
// ========
// Class definition for primitive proxy.
//
// Author: Paulo Pagliosa
// Last revision: 24/01/2022

#ifndef __PrimitiveProxy_h
#define __PrimitiveProxy_h

#include "graph/ComponentProxy.h"
#include "graphics/Actor.h"
#include "graphics/TriangleMeshMapper.h"
#include <math/Real.h>
#include <math/Vector3.h>
#include <geometry/TriangleMesh.h>

namespace cg
{ // begin namespace cg

namespace graph
{ // begin namespace graph


/////////////////////////////////////////////////////////////////////
//
// PrimitiveProxy: primitive proxy class
// ==============
class PrimitiveProxy: public ComponentProxy<PrimitiveMapper>
{
public:
  static auto New(const PrimitiveMapper& mapper)
  {
    return new PrimitiveProxy{mapper};
  }

  PrimitiveMapper* mapper() const
  {
    return _object;
  }

  Actor* actor() const
  {
    return _actor;
  }

protected:
  Reference<Actor> _actor;

  PrimitiveProxy(const PrimitiveMapper& mapper):
    ComponentProxy<PrimitiveMapper>{"Primitive", mapper}
  {
    // do nothing
  }

  void afterAdded() override;
  void beforeRemoved() override;
  void update() override;
  void setVisible(bool value) override;

}; // PrimitiveProxy

inline auto
asPrimitive(Component* component)
{
  return dynamic_cast<PrimitiveProxy*>(component);
}


/////////////////////////////////////////////////////////////////////
//
// TriangleMeshProxy: triangle mesh proxy class
// =================
class SweeperMeshProxy final: public PrimitiveProxy
{
public:
  static auto New(const TriangleMesh& mesh, const std::string& meshName)
  {
    return new SweeperMeshProxy{mesh, meshName};
  }

  const char* const meshName() const
  {
    return _meshName.c_str();
  }

  //void setMesh(const TriangleMesh& mesh, const std::string& meshName)
  //{
  //  ((TriangleMeshMapper*)PrimitiveProxy::mapper())->setMesh(mesh);
  //  _meshName = meshName;
  //}

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

private:
  std::string _meshName;

  SweeperMeshProxy(const TriangleMesh& mesh, const std::string& meshName):
    PrimitiveProxy{*new TriangleMeshMapper{mesh}},
    _meshName{meshName}
  {
    // do nothing
  }

}; // TriangleMeshProxy

} // end namepace graph

} // end namespace cg

#endif // __PrimitiveProxy_h
