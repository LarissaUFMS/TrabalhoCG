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
// OVERVIEW: MainWindow.h
// ========
// Class definition for cg template window.
//
// Author: Paulo Pagliosa
// Last revision: 07/11/2022

#ifndef __MainWindow_h
#define __MainWindow_h

#include "graphics/GLRenderWindow3.h"


/////////////////////////////////////////////////////////////////////
//
// MainWindow: template main window class
// ==========
class MainWindow final : public cg::GLRenderWindow3
{
public:
	MainWindow(int width, int height);

private:
	using Base = cg::GLRenderWindow3;

	// Atributos da Interface
	cg::Color _lineColor;
	cg::Color _meshColor;
	float _radius{ 0 };
	float _speed{ 0 };
	bool _animate{ false };
	bool _showGround{ true };

	// Atributos da Geratriz
	bool _poligono{ true }; // Indica se será gerado um Poligo ou um Arco

		// Atributos Poligono Regular 
	int _nPontos{ 20 };
	float _escalaX{ 1 };
	float _escalaY{ 1 };
	float _rotacao{ 0 };

		// Atributos Arco de Circunferência - Inclui também os atributos do Poligono
	float _theta{ 1 };
	bool _fechada{ false };

	// Atributos da Varredura
	bool _espiral{ true };

		// Varredura por Espiral
	float _incrAltura{ 0 };
	float _incrLargura{ 0 };
	float _nRev{ 1 };
	float _deslX{ 2 };
	int _nSubdivisoesV{ 40 };
	bool _tampaV{ false };

		//Varredura por Translacao e Torcao
	float _comprimento{ 1 };
	float _deslocamentoH{ 0 };
	float _deslocamentoV{ 0 };
	int _nSubDivisoesT{ 10 };
	float _escalaInicialT{ 1 };
	float _escalaFinalT{ 1 };
	float _torcao{ 0 };
	bool _tampaT{ false };


	// Overridden method examples
	void initialize() override;
	void update() override;
	void renderScene() override;
	bool keyInputEvent(int, int, int) override;
	void gui() override;

}; // MainWindow

#endif // __MainWindow_h
