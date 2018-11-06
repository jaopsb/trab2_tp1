#ifndef COMANDOS_H
#define COMANDOS_H
#include "dominios.hpp"
#include "Entidades.hpp"
#include "Interfaces.hpp"
#include "Controladoras.hpp"

class CmdLogin
{
public:
  CmdLogin();
  Usuario login(Identificador &, Senha &);
};

class CmdRegistraUsuario : public ICmd
{
private:
  void insereUsuario();

public:
  void executar();
};

class CmdBuscarUsuario : public ICmd
{
private:
  Identificador *identificador;
  Senha *senha;
  RetornoLogin retorno;

public:
  CmdBuscarUsuario(Identificador *, Senha *);
  void executar();
  RetornoBase get_resultado() { return retorno; };
};

#endif // COMANDOS_H
