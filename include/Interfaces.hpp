#ifndef INTERFACES_H
#define INTERFACES_H
#include "dominios.hpp";

class IUAut
{
public:
  virtual bool autenticar() = 0;
  virtual ~IUAut(){};
};

class IServAut
{
public:
  virtual bool autenticar(Identificador&, Senha&) = 0;
  virtual ~IServAut() {}
};

//INTERFACES COMANDOS

#endif //INTERFACES_H
