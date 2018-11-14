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

class IServUsu
{
public:
  virtual void cadastrarUsuario(string, string, string) = 0;
  virtual void cadastraContaCorrente(string, string, int, int) = 0;
  virtual void cadastraCartaodeCredito(string, string, string) = 0;

  virtual Usuario *buscarUsuario(string) = 0;
  virtual Conta_corrente *buscarContaCorrente(string) = 0;
  virtual Cartao_de_Credito *buscar_cartao(string) = 0;

  virtual void editarUsuario(string, string, string) = 0;

  virtual void deletarUsuario(string, string) = 0;
  virtual void deletarContaCorrente(string) = 0;
  virtual void deletarCartaodeCredito(string) = 0;
};

class IServAcom
{
public:
  virtual void buscarAcomodacoes(string) = 0;
};

#endif //INTERFACES_H
