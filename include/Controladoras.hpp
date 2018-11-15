#ifndef CONTROLADORAS_H
#define CONTROLADORAS_H
#include "Interfaces.hpp"
#include "dominios.hpp"
#include <sqlite3.h>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

using namespace std;

/*
Classe Base do Banco de dados
Evitando mudancas desnecessarias ao banco
*/
class CtrlServ
{
private:
  static bool banco_criado;
  char *NOME_BD = "hotel.db";
  sqlite3_stmt *stmt;
  sqlite3 *banco;

public:
  bool bd_criado() { return banco_criado; }
  char *get_nome_banco() { return NOME_BD; }
  sqlite3 *get_banco() { return banco; }
  sqlite3_stmt *get_stmt() { return stmt; }
  int executa(string);
  void finaliza();
  bool init_banco();
};

class CtrlIUAut : public IUAut
{
private:
  IServAut *ctrlServAut;
  Identificador *identificador;
  Senha *senha;
  RetornoLogin retorno;

public:
  CtrlIUAut();
  RetornoLogin autenticar();
  void setCtrlServAut(IServAut *);
};

class CtrlServAut : public CtrlServ, public IServAut
{
private:
  char *NOME_BD = "hotel.db";

  Identificador *identificador;
  Senha *senha;
  sqlite3_stmt *stmt;
  sqlite3 *banco;

  bool init_banco();

public:
  Usuario *autenticar(Identificador &, Senha &);
};

class CtrlServUsu : public CtrlServ, public IServUsu
{
private:
  bool existeUsuario(string);
  bool existeContaCorrente(string);
  bool existeCartaodeCredito(string);

public:
  void cadastrarUsuario(string, string, string);
  void cadastraContaCorrente(string, string, int, int);
  void cadastraCartaodeCredito(string, string, string);

  Usuario *buscarUsuario(string);
  Conta_corrente *buscarContaCorrente(string);
  Cartao_de_Credito *buscar_cartao(string);

  void editarUsuario(string, string, string);

  void deletarUsuario(string, string);
  void deletarContaCorrente(string);
  void deletarCartaodeCredito(string);
};

class CtrlIUUsu
{
private:
  Identificador *identificador;
  Senha *senha;
  IServUsu *ctrl;
  Usuario *u;
  void cadastrar();
  void deletar();
  void editar();
  void cadastrarCC(); //conta corrente
  void deletarCC();
  void cadastrarCdC(); //cartao de credito
  void removerCdc();

public:
  CtrlIUUsu(string, string);
  const static int REGISTRAR = 1;
  const static int DEL_USU = 2;
  const static int EDIT_USU = 3;
  const static int REG_CONTAC = 4;
  const static int DEL_CONTAC = 6;
  const static int EDIT_CONTAC = 7;
  const static int REG_CARTCRED = 8;
  const static int DEL_CARTCRED = 9;

  void executa();

  void setCtrlServ(IServUsu *);
};

class CtrlServAcom : public CtrlServ, public IServAcom
{
public:
  vector<Acomodacao> buscarAcomodacoes(string);
  void cadastrarAcomodacao(Acomodacao);
};

class CtrlIUAcom
{
private:
  Identificador *identificador;
  IServAcom *ctrl;

  void cadastra();
  void buscarAcoms();

public:
  CtrlIUAcom(string);
  const static int CAD_ACOM = 1;
  const static int BUS_ACOMS = 2;

  void executa();

  void setCtrlServ(IServAcom *);
};

#endif // CONTROLADORAS_
