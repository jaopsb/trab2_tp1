#include <iostream>
#include <Testes.hpp>
#include "Controladoras.hpp"
using namespace std;

int main()
{
  IUAut *ctrl;

  ctrl = new CtrlIUAut();

  ctrl->menu();

  return 0;
}
