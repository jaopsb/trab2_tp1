#include "Interfaces.hpp"
#include "dominios.hpp"

#include <stdexcept>
#include <iostream>
#include <cstdlib>

using namespace std;

class CtrlIUAut : public IUAut
{
private:
  const static int LOGIN = 1;
  const static int REGISTRAR = 2;

  IServAut *iServAut;
  Identificador *identificador;
  Senha *senha;

  void login();
  void registar();

public:
  bool autenticar(string, string);
  void menu();
  void setCtrlServAut(IServAut *iServAut)
  {
    this->iServAut = iServAut;
  }
};
