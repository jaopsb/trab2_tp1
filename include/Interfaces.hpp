#ifndef INTERFACES_H
#define INTERFACES_H
#include "dominios.hpp";
#include "Entidades.hpp";
#include "Controladoras.hpp";
#include <sqlite3.h>

class IUAut
{
public:
  virtual RetornoLogin autenticar() = 0;
  virtual ~IUAut(){};
};

class IServAut
{
public:
  virtual Usuario *autenticar(Identificador &, Senha &) = 0;
  virtual ~IServAut() {}
};

class IServLogin
{
public:
  virtual Usuario Login(Identificador &, Senha &) = 0;
};

//INTERFACES COMANDOS (?)
#endif //INTERFACES_H
