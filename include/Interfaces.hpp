#ifndef INTERFACES_H
#define INTERFACES_H
#include "dominios.hpp";
#include "Entidades.hpp";
#include <sqlite3.h>

class IUAut
{
public:
  virtual bool autenticar() = 0;
  virtual ~IUAut(){};
};

class IServAut
{
public:
  virtual bool autenticar(Identificador &, Senha &) = 0;
  virtual ~IServAut() {}
};

class IServLogin
{
public:
  virtual Usuario Login(Identificador &, Senha &) = 0;
};

class IServMenu
{
public:
  virtual bool registrar(Nome &, Identificador &, Senha &) = 0;
  virtual Usuario login(Identificador &, Senha &) = 0;
  virtual ~IServMenu() {}
};

//INTERFACES COMANDOS (?)
#endif //INTERFACES_H
