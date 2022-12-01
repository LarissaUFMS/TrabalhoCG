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
// OVERVIEW: MainWindow.cpp
// ========
// Source file for cg demo main window.
//
// Author: Paulo Pagliosa
// Last revision: 11/03/2022

#include "graphics/Application.h"
#include "reader/SceneReader.h"
#include "MainWindow.h"
#include "ProxyEspiral.h"
#include "ProxyTorcao.h"
#include "MeshWriter.h"

/////////////////////////////////////////////////////////////////////
//
// MainWindow implementation
// ==========
MeshMap MainWindow::_defaultMeshes;

inline void
MainWindow::buildDefaultMeshes()
{
  _defaultMeshes["Box"] = GLGraphics3::box();
  _defaultMeshes["Sphere"] = GLGraphics3::sphere();
  _defaultMeshes["Cylinder"] = GLGraphics3::cylinder();
  _defaultMeshes["Cone"] = GLGraphics3::cone();
  _defaultMeshes["Spiral"] = PrimitiveProxyEspiral::espiralPpoligon();
  _defaultMeshes["Torsion"] = PrimitiveProxyTorcao::torcaoPpoligon();

}

void
MainWindow::initializeScene()
{
  auto a = (float)width() / (float)height();
  auto s = createCameraObject(a, "Main Camera");

  s->transform()->setLocalPosition({0, 0, 10});
  s = createLightObject(Light::Type::Directional, "Directional Light");
  s->transform()->setLocalEulerAngles({50, 30, 0});
  createDefaultPrimitiveObject("Box");
}

void
MainWindow::beginInitialize()
{
  Assets::initialize();
  buildDefaultMeshes();
  Assets::meshes().insert(_defaultMeshes.begin(), _defaultMeshes.end());

  constexpr auto ffn = "fonts/Roboto-Regular.ttf";
  auto fonts = ImGui::GetIO().Fonts;

  fonts->AddFontFromFileTTF(Application::assetFilePath(ffn).c_str(), 16);
}

void
MainWindow::createObjectMenu()
{
  if (ImGui::BeginMenu("3D Object"))
  {
	if (ImGui::MenuItem("Box"))
	  createDefaultPrimitiveObject("Box");
	if (ImGui::MenuItem("Sphere"))
	  createDefaultPrimitiveObject("Sphere");
	if (ImGui::MenuItem("Cylinder"))
	  createDefaultPrimitiveObject("Cylinder");
	if (ImGui::MenuItem("Cone"))
	  createDefaultPrimitiveObject("Cone");
	if (ImGui::MenuItem("Spiral"))
		createDefaultSpiralObject();
	if (ImGui::MenuItem("Torsion"))
		createDefaultTorcaoObject();
	ImGui::EndMenu();
  }
}

Component*
MainWindow::addComponentMenu()
{
  Component* component = nullptr;

  if (ImGui::BeginMenu("3D Object"))
  {
	if (ImGui::MenuItem("Box"))
	  component = makeDefaultPrimitive("Box");
	if (ImGui::MenuItem("Sphere"))
	  component = makeDefaultPrimitive("Sphere");
	if (ImGui::MenuItem("Cylinder"))
	  component = makeDefaultPrimitive("Cylinder");
	if (ImGui::MenuItem("Cone"))
	  component = makeDefaultPrimitive("Cone");
	if (ImGui::MenuItem("Spiral"))
	  component = makeDefaultSpiral();
	if (ImGui::MenuItem("Torsion"))
		component = makeDefaultTorcao();

	ImGui::EndMenu();
  }
  return component;
}

void
MainWindow::readScene(const std::string& filename)
{
  try
  {
	parser::SceneReader reader;

	reader.setInput(filename);
	reader.execute();
	if (reader.scene() != nullptr)
	  SceneWindow::setScene(*reader.scene());
  }
  catch (const std::exception& e)
  {
	puts(e.what());
  }
}

inline void
MainWindow::fileMenu()
{
  if (ImGui::BeginMenu("File"))
  {
	if (ImGui::MenuItem("New Scene"))
	  newScene();
	if (ImGui::BeginMenu("Open"))
	{
	  // TODO
	  if (ImGui::MenuItem("test.scn"))
		readScene(Application::assetFilePath("scenes/test.scn"));
	  ImGui::EndMenu();
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Exit", "Alt+F4"))
	{
	  shutdown();
	}
	ImGui::EndMenu();
  }
}

inline void
MainWindow::createMenu()
{
  if (ImGui::BeginMenu("Create"))
  {
	if (ImGui::MenuItem("Material"))
	  createMaterial();
	ImGui::Separator();
	createObjectMenu();
	ImGui::EndMenu();
  }
}

inline void
MainWindow::showOptions()
{
  ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
  ImGui::showStyleSelector("Color Theme##Selector");
  ImGui::colorEdit3("Selected Wireframe", _selectedWireframeColor);
  ImGui::PopItemWidth();
}

inline void
MainWindow::mainMenu()
{
  if (ImGui::BeginMainMenuBar())
  {
	fileMenu();
	createMenu();
	if (ImGui::BeginMenu("View"))
	{
	  static const char* viewLabels[]{"Editor", "Ray Tracer"};

	  if (ImGui::BeginCombo("View", viewLabels[(int)_viewMode]))
	  {
		for (auto i = 0; i < IM_ARRAYSIZE(viewLabels); ++i)
		{
		  if (ImGui::Selectable(viewLabels[i], _viewMode == (ViewMode)i))
			_viewMode = (ViewMode)i;
		}
		ImGui::EndCombo();
		// TODO: change mode only if scene has changed
		if (_viewMode == ViewMode::Editor)
		  _image = nullptr;
	  }
	  ImGui::Separator();
	  ImGui::MenuItem("Hierarchy Window", nullptr, &_showHierarchy);
	  ImGui::MenuItem("Inspector Window", nullptr, &_showInspector);
	  ImGui::MenuItem("Camera Preview", nullptr, &_showPreview);
	  ImGui::MenuItem("Assets Window", nullptr, &_showAssets);
	  ImGui::MenuItem("Editor View Settings", nullptr, &_showEditorView);
	  ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Ray Tracing"))
	{
	  ImGui::DragInt("Max Recursion Level",
		&_maxRecursionLevel,
		1.0f,
		0,
		RayTracer::maxMaxRecursionLevel);
	  ImGui::DragFloat("Min Weight",
		&_minWeight,
		0.01f,
		RayTracer::minMinWeight,
		1.0f);
	  ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Tools"))
	{
	  if (ImGui::BeginMenu("Options"))
	  {
		showOptions();
		ImGui::EndMenu();
	  }
	  ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
  }
}

void
MainWindow::gui()
{
  mainMenu();
  if (_viewMode == ViewMode::Renderer)
	return;

  constexpr auto evw = (float)360;
  constexpr auto evh = (float)308;
  constexpr auto lft = (float)0;
  constexpr auto top = (float)23;
  auto w = (float)width();
  auto h = (float)height();
  auto evy = h - evh;

  // Hierarchy Window
  ImGui::SetNextWindowPos({lft, top});
  ImGui::SetNextWindowSize({evw, evy - top});
  hierarchyWindow();
  // Editor View
  ImGui::SetNextWindowPos({lft, evy});
  ImGui::SetNextWindowSize({evw, evh});
  editorView();

  constexpr auto iww = (float)420;
  auto rgt = w - iww;
  auto awh = h / 3;
  auto awy = h - awh;

  // Inspector Window
  ImGui::SetNextWindowPos({rgt, top});
  ImGui::SetNextWindowSize({iww, awy - top});
  inspectorWindow();
  // Assets Window
  ImGui::SetNextWindowPos({rgt, awy});
  ImGui::SetNextWindowSize({iww, awh});
  assetsWindow();
}

void
MainWindow::renderScene()
{
  if (_viewMode != ViewMode::Renderer)
	return;

  auto camera = CameraProxy::current();

  if (nullptr == camera)
	camera = editor()->camera();
  if (_image == nullptr)
  {
	_image = new GLImage{width(), height()};
	if (_rayTracer == nullptr)
	  _rayTracer = new RayTracer{*scene(), *camera};
	else
	  _rayTracer->setCamera(*camera);
	_rayTracer->setMaxRecursionLevel(_maxRecursionLevel);
	_rayTracer->setMinWeight(_minWeight);
	_rayTracer->renderImage(*_image);
  }
  _image->draw(0, 0);
}

bool
MainWindow::onResize(int width, int height)
{
  _viewMode = ViewMode::Editor;
  _image = nullptr;
  return true;
}

void
MainWindow::inspectSpiral(SceneWindow&, PrimitiveProxyEspiral& proxy) {
	if (ImGui::Checkbox("Poligono Regular?", &proxy._tipoPoligono))
		proxy.atualizarMesh();
	ImGui::Separator();
	if(	ImGui::SliderInt("Numero de Pontos", &proxy._geratriz._nPontos, 3, 40)
		||
		ImGui::SliderFloat("Escala em X", &proxy._geratriz._escalaXf, 0.1f, 2.0f)
		||
		ImGui::SliderFloat("Escala em Y", &proxy._geratriz._escalaYf, 0.1f, 2.0f)
		||
		ImGui::SliderFloat("Rotacao em torno de Z", &proxy._geratriz._rotacao, 0.0f, 2.0f)
		||
		ImGui::SliderFloat("Theta- angulo do arco", &proxy._geratriz._theta, 0.0f, 2.0f)
		||
		ImGui::Checkbox("Fechada?", &proxy._geratriz._fechada))
		{
		proxy.atualizarMesh();
	}
	ImGui::Separator();
	if (ImGui::SliderFloat("Numero de revolucoes", &proxy._nRev, 0.1f, 20.0f)
		||
		ImGui::SliderInt("Numero de subdivisoes por revolucao", &proxy._nSubdiv, 3, 40)
		||
		ImGui::SliderFloat("Largura inicial", &proxy._deslX, 2*proxy._geratriz._escalaXf, 100.0f)
		||
		ImGui::SliderFloat("Incremento de Largura", &proxy._deltinhaLargura, 0.0f, 10.0f)
		||
		ImGui::SliderFloat("Incremento de Altura", &proxy._deltinhaAltura, 0.0f, 10.0f)
		||
		ImGui::Checkbox("Tem tampa?", &proxy._tampa))
		proxy.atualizarMesh();
	if (ImGui::Button("salvar")) {
		if (proxy._tipoPoligono) 
			proxy._geratriz.poligonoR(proxy._geratriz._nPontos, proxy._geratriz._escalaXf, proxy._geratriz._escalaYf, proxy._geratriz._rotacao);
		else
			proxy._geratriz.arcoCircunf(proxy._geratriz._nPontos, proxy._geratriz._escalaXf, proxy._geratriz._escalaYf, proxy._geratriz._rotacao, proxy._geratriz._theta, proxy._geratriz._fechada);
		MeshWriter::writeOBJ(proxy.espiral(proxy._geratriz, proxy._deslX, proxy._nRev, proxy._nSubdiv, proxy._deltinhaAltura, proxy._deltinhaLargura, proxy._tampa), "../../assets/meshes/espiral.obj");
	}
	proxy.actor()->visible = proxy.sceneObject()->visible();
}

void
MainWindow::inspectTorsion(SceneWindow&, PrimitiveProxyTorcao& proxy) {

	if (ImGui::Checkbox("Poligono Regular?", &proxy._tipoPoligono))
		proxy.atualizarMesh();
	ImGui::Separator();
	if (ImGui::SliderInt("Numero de Pontos", &proxy._geratriz._nPontos, 3, 40)
		||
		ImGui::SliderFloat("Escala em X", &proxy._geratriz._escalaXf, 0.1f, 2.0f)
		||
		ImGui::SliderFloat("Escala em Y", &proxy._geratriz._escalaYf, 0.1f, 2.0f)
		||
		ImGui::SliderFloat("Rotacao em torno de Z", &proxy._geratriz._rotacao, 0.0f, 2.0f)
		||
		ImGui::SliderFloat("Theta- angulo do arco", &proxy._geratriz._theta, 0.0f, 2.0f)
		||
		ImGui::Checkbox("Fechada?", &proxy._geratriz._fechada))
	{
		proxy.atualizarMesh();
	}
	ImGui::Separator();
	if (ImGui::SliderFloat("Comprimento", &proxy._comprimento, 1.0f, 50.0f)
		||
		ImGui::SliderFloat("Deslocamento horizontal", &proxy._deslHorizontal, -proxy._geratriz._escalaXf, proxy._geratriz._escalaXf)
		||
		ImGui::SliderFloat("Deslocamento vertical", &proxy._deslVertical, -proxy._geratriz._escalaYf, proxy._geratriz._escalaYf)
		||
		ImGui::SliderInt("Numero de subdivisoes por revolucao", &proxy._nSubdiv, 3, 40)
		||
		ImGui::SliderFloat("Escala inicial", &proxy._escalaInicial, 0.1f, 5.0f)
		||
		ImGui::SliderFloat("Escala final", &proxy._escalaFinal, 0.1f, 5.0f)
		||
		ImGui::SliderFloat("Torcao da geratriz", &proxy._torcao, -2.0f, 2.0f)
		||
		ImGui::Checkbox("Tem tampa?", &proxy._tampa))
		proxy.atualizarMesh();

	if (ImGui::Button("salvar")) {

		if (proxy._tipoPoligono)
			proxy._geratriz.poligonoR(proxy._geratriz._nPontos, proxy._geratriz._escalaXf, proxy._geratriz._escalaYf, proxy._geratriz._rotacao);
		else
			proxy._geratriz.arcoCircunf(proxy._geratriz._nPontos, proxy._geratriz._escalaXf, proxy._geratriz._escalaYf, proxy._geratriz._rotacao, proxy._geratriz._theta, proxy._geratriz._fechada);
		MeshWriter::writeOBJ(proxy.torcao(proxy._geratriz, proxy._comprimento, proxy._deslHorizontal, proxy._deslVertical, proxy._nSubdiv, proxy._escalaInicial, proxy._escalaFinal, proxy._torcao, proxy._tampa), "../../assets/meshes/torcao.obj");
	}
		proxy.actor()->visible = proxy.sceneObject()->visible();
	}