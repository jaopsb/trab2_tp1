#ifndef STUBS_H
#define STUBS_H
#include "Entidades.hpp"
#include "dominios.hpp"
#include "Controladoras.hpp"

class CtrlServUsuStub : public CtrlServUsu
{
public:
  void buscar_usuario();
};
#endif // STUBS_H
