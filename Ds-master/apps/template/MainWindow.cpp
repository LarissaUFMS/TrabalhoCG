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
// Source file for cg template window.
//
// Author: Paulo Pagliosa
// Last revision: 07/11/2022

#include "MainWindow.h"
#include "Geratriz.cpp"	

/////////////////////////////////////////////////////////////////////
//
// MainWindow implementation
// ==========
MainWindow::MainWindow(int width, int height) :
    Base{ "Ds template", width, height }
{
    // Put your code here. Example:
    _lineColor = cg::Color::red;
    _meshColor = cg::Color::white;
    //_animate = false;
    //_radius = 0;
    //_speed = 0.01f;
    //_incrAltura = 0.0f; //Incremento na altura por revolu��o
    //_incrLargura = 0.0f; //Incremento na Largura por revolu��o
    //_nRev = 1.0f;
    //_deslX = 2.0f;
    //_nPontos = 3;
    //_nSubdiv = 3;
    //_escalaX = 1.0f;
    //_escalaY = 1.0f;
    //_rotacao = 0.0f;
    //_theta = 1.0f;
    //_escala = 1.0f;
    //_fechada = true;
}

void
MainWindow::initialize()
{
    // Put your OpenGL initialization code here. Example:
    Base::initialize();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
}

void
MainWindow::update()
{
    // Put your scene update code here. Example:
    static float time{};

    if (_animate)
        _radius = 1 + cosf(_speed * (time += deltaTime()) * 0.5f);
}

void
MainWindow::renderScene()
{
    // Put your scene rendering code here. Example:
    using namespace cg;

    auto g3 = this->g3();

    g3->setLineColor(_lineColor);
    g3->drawArc({ -4, 0, 0 }, // center
        _radius, // radius
        { 1, 0, 0 }, // first point direction
        { 0, 0, 1 }, // normal
        180); // angle
    g3->setPolygonMode(GLGraphics3::LINE);
    g3->drawCircle({ 0, 0, 0 }, // center
        _radius, // radius
        { 0, 0, 1 }); // normal
    g3->setPolygonMode(GLGraphics3::FILL);
    g3->setMeshColor(_meshColor);
    if (_showGround)
        g3->drawXZPlane(8, 1);

    vec3f* vertices;
    auto geratriz = new Geratriz{};
    if (_poligono)
        vertices = geratriz->poligonoR(_nPontos, _escalaX, _escalaY, _rotacao);
    else
        vertices = geratriz->arcoCircunf(_nPontos, _escalaX, _escalaY, _rotacao, _theta, _fechada);
	for (int i = 1; i < geratriz->nVertices(); i++) {
		g3->drawLine(vertices[i - 1], vertices[i]);
	}
    //poligonoR(_nPontos, _escalaX, _escalaY, _rotacao);
    Varredura* varreduraObj = new Varredura(*geratriz);
    TriangleMesh* varredura;
    if(_espiral)
        varredura = varreduraObj->espiral(*geratriz, _deslX, _nRev, _nSubdivisoesV, _incrAltura, _incrLargura, _tampaV);
    else
        varredura = varreduraObj->translacaoTorcao(*geratriz, _comprimento, _deslocamentoH, _deslocamentoV, _nSubDivisoesT, _escalaInicialT, _escalaFinalT, _torcao, _tampaT);
   
    g3->drawMesh(*varredura);
}

bool
MainWindow::keyInputEvent(int key, int action, int mods)
{
    // Put your key event handler code here. Example:
    if (action != GLFW_RELEASE && mods == GLFW_MOD_ALT)
        switch (key)
        {
        case GLFW_KEY_P:
            _animate ^= true;
            return true;
        }
    return Base::keyInputEvent(key, action, mods);
}

void
MainWindow::gui()
{
    // Put your gui code here. Example:
    ImGui::SetNextWindowSize({ 600, 400 });
    ImGui::Begin("Template GUI");
    ImGui::ColorEdit3("Line Color", (float*)&_lineColor);
    ImGui::ColorEdit3("Mesh Color", (float*)&_meshColor);
    ImGui::Checkbox("Show Ground", &_showGround);
    //ImGui::Checkbox("Animate", &_animate);
    //ImGui::SliderFloat("Speed", &_speed, 0.001f, 0.01f);
    ImGui::Checkbox("Poligono Regular?", &_poligono);
    ImGui::Separator();

    ImGui::SliderInt("Num Pontos", &_nPontos, 3, 20);
    ImGui::SliderFloat("Escala em X", &_escalaX, 0.1f, 2.0f);
    ImGui::SliderFloat("Escala em Y", &_escalaY, 0.1f, 2.0f);
    ImGui::SliderFloat("Rotacao", &_rotacao, 0.0f, 2.0f);
    if (!_poligono) {
        ImGui::SliderFloat("Theta", &_theta, 0.0f, 2.0f);
        ImGui::Checkbox("Fechada?", &_fechada);
    }
    ImGui::Separator();
    
    ImGui::Checkbox("Espiral?", &_espiral);
    if (_espiral) {
        ImGui::SliderFloat("Altura", &_incrAltura, 0.0f, 10.0f);
        ImGui::SliderFloat("Largura", &_incrLargura, 0.0f, 10.0f);
        ImGui::SliderFloat("Num Revol", &_nRev, 0.1f, 20.0f);
        ImGui::SliderFloat("Desl. X", &_deslX, 2.0f, 100.0f);
        ImGui::SliderInt("SubDivisoes", &_nSubdivisoesV, 3, 40);
        ImGui::Checkbox("Tampa?", &_tampaV);
    }
    else {
        ImGui::SliderFloat("Comprimento", &_comprimento, 1.0f, 50.0f);
        ImGui::SliderFloat("Deslocamento H", &_deslocamentoH, 0.0f, 10.0f);
        ImGui::SliderFloat("Deslocamento V", &_deslocamentoV, 0.0f, 10.0f);
        ImGui::SliderInt("SubDivisoes", &_nSubDivisoesT, 1.0f, 50.0f);
        ImGui::SliderFloat("Escalal Inicial", &_escalaInicialT, 0.1f, 5.0f);
        ImGui::SliderFloat("Escalal Final", &_escalaFinalT, 0.1f, 5.0f);
        ImGui::SliderFloat("Torcao", &_torcao, -2.0f, 2.0f);
        ImGui::Checkbox("Tampa?", &_tampaT);
    }
    ImGui::Separator();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        deltaTime(),
        ImGui::GetIO().Framerate);
    ImGui::End();
}