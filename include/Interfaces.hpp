#ifndef INTERFACES_H
#define INTERFACES_H
#include "dominios.hpp"
#include "Entidades.hpp"
#include <vector>
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

  virtual bool deletarUsuario(string) = 0;
  virtual void deletarContaCorrente(string) = 0;
  virtual void deletarCartaodeCredito(string) = 0;
};

class IServAcom
{
public:
  virtual vector<Acomodacao> buscarAcomodacoes(string) = 0;
  virtual vector<Acomodacao> buscarAcomodacoes() = 0;
  virtual vector<Acomodacao> buscarAcomodacoesParaReserva(string id) = 0;
  virtual vector<Reserva> buscarReservas(string) = 0;

  virtual void cadastrarAcomodacao(Acomodacao) = 0;
  virtual void removerAcomodacao(Acomodacao) = 0;

  virtual void cadastrarReserva(string, Acomodacao, string, string) = 0;
  virtual void removerReserva(Reserva) = 0;

  virtual bool podeCadastrarAcomodacao(string) =0;
};

#endif //INTERFACES_H
