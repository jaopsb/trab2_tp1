#ifndef INTERFACES_H
#define INTERFACES_H
#include "dominios.hpp"
#include "Entidades.hpp"
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

class IServUsu
{
public:
  virtual Usuario buscarUsuario(string, string) = 0;
};

class ICmd
{
public:
  virtual void executar() = 0;
};
#endif //INTERFACES_H
