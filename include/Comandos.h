#ifndef COMANDOS_H
#define COMANDOS_H
#include "dominios.hpp";
#include "Entidades.hpp";

class Comandos
{
public:
  Comandos();
  virtual ~Comandos();

protected:
private:
};

class CmdLogin
{
public:
  CmdLogin();
  Usuario login(Identificador &, Senha &);
};

#endif // COMANDOS_H
