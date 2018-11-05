#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <string>
#include "Comandos.h"
#include "dominios.hpp"
#include "Entidades.hpp"
#include "Controladoras.hpp"

/************* SCRIPTS CRIACAO DO BANCO ************/
char *SQL_STMT_CREATE_USUARIO = "CREATE TABLE IF NOT EXISTS `USUARIO` ( `ID` INTEGER, `IDENTIFICADOR` TEXT NOT NULL UNIQUE, `NOME` TEXT NOT NULL, `SENHA` TEXT NOT NULL, PRIMARY KEY(`IDENTIFICADOR`,`ID`));";
char *SQL_STMT_CREATE_ACOMODACAO = "CREATE TABLE IF NOT EXISTS `ACOMODACAO` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `tipo` INTEGER NOT NULL, `capacidade` INTEGER NOT NULL, `cidade` TEXT NOT NULL, `estado` INTEGER NOT NULL, `diaria` REAL NOT NULL );";
char *SQL_STMT_CREATE_CARTAO = "CREATE TABLE IF NOT EXISTS 'CARTAO' ( `ID` INTEGER PRIMARY KEY AUTOINCREMENT,`NUMERO` TEXT NOT NULL, `DT_VALIDADE` TEXT NOT NULL, `ID_USUARIO` INTEGER NOT NULL );";
char *SQL_STMT_CREATE_CONTACORRENTE = "CREATE TABLE IF NOT EXISTS 'CONTACORRENTE' ( `ID` INTEGER PRIMARY KEY AUTOINCREMENT, `NUMERO` TEXT NOT NULL, `AGENDA` INTEGER NOT NULL, `BANCO` INTEGER NOT NULL, `ID_USUARIO` INTEGER NOT NULL );";
char *SQL_STMT_CREATE_RESERVA = "CREATE TABLE IF NOT EXISTS `RESERVA` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `id_usuario` INTEGER NOT NULL, `id_acomodacao` INTEGER NOT NULL, `data_incio` TEXT NOT NULL, `data_fim` TEXT NOT NULL );";
/************ SCRIPTS CRIACAO DO BANCO *************/

/***************FUNCOES AUXILIARES************/
bool trata_retorno(int rc)
{
  switch (rc)
  {
  case SQLITE_DONE:
  case SQLITE_ROW:
  case SQLITE_OK:
    return true;
    break;
  case SQLITE_CONSTRAINT:
    throw invalid_argument("Identificador ja utilizado!");
    break;
  case SQLITE_ERROR:
    throw invalid_argument("Erro no servi�o!");
    break;

  default:

    throw invalid_argument(to_string(rc));

    break;
  }
}

/***************FUNCOES AUXILIARES************/

/*************CONTROLADORA DE INTERFACE DE USUARIO - AUTENTICACAO********/

CtrlIUAut::CtrlIUAut()
{
  identificador = new Identificador();
  senha = new Senha();
}

void CtrlIUAut::setCtrlServAut(IServAut *iServAut)
{
  ctrlServAut = iServAut;
}

RetornoLogin CtrlIUAut::autenticar()
{
  string id, snh;
  char resp;
  bool resultado = true;
  bool fim = false;

  while (!fim)
  {
    try
    {
      cout << "identificador:";
      cin >> id;
      identificador->set_identificador(id);
      cout << "Senha:";
      cin >> snh;
      senha->set_senha(snh);

      system("cls"); //windows
      fim = true;

      //TODO retornar o usuario
      Usuario *usu = ctrlServAut->autenticar(*identificador, *senha);
      retorno.set_usuario(usu);
    }
    catch (const invalid_argument &ia)
    {
      cout << "ERRO - " << ia.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        resultado = false;
        fim = true;
      }
    }
  }

  retorno.set_resultado(resultado);

  return retorno;
}

/*************CONTROLADORA DE INTERFACE DE USUARIO - AUTENTICACAO********/

/************* CONTROLADORA DE INTERFACE DE USUARIO - USUARIO ***********/

void CtrlIUUsu::setCtrlServUsu(CtrlServUsu *serv)
{
  ctrl = serv;
}

void CtrlIUUsu::executa()
{
  ctrl = new CtrlServUsu();

  int opt = 0;
  while (opt != 5)
  {
    cout << "Painel Usuario" << endl;
    cout << "Registrar Usuario - " << CtrlIUUsu::REGISTRAR << endl;
    cout << "Editar Dados      - " << CtrlIUUsu::EDIT_USU << endl;
    cout << "Sair              - 5" << endl;
    cout << "Selecione a opcao: ";
    cin >> opt;

    switch (opt)
    {
    case CtrlIUUsu::EDIT_USU:
      CtrlIUUsu::editarUsuario();
      break;
    case CtrlIUUsu::REGISTRAR:
      break;
    default:
      cout << "NENHUMA RESPOSTA" << endl;
      break;
    }
  }
}

CtrlIUUsu::CtrlIUUsu(string id, string sn)
{
  identificador = new Identificador();
  identificador->set_identificador(id);
  senha = new Senha();
  senha->set_senha(sn);
}

void CtrlIUUsu::editarUsuario()
{
  cmd = new CmdBuscarUsuario(identificador, senha);
  cmd->executar();
}

void CtrlServUsu::buscarUsuario()
{
  cout << "Serv - Editar Usuario" << endl;
  if (!CtrlServ::bd_criado())
    CtrlServ::init_banco();

  cout << "OK" << endl;
}

/************* CONTROLADORA DE INTERFACE DE USUARIO - USUARIO ***********/

/*************CONTROLADORA DE SERVICO - AUTENTICACAO********/
bool CtrlServAut::init_banco()
{
  char *errmsg;
  bool resultado = true;
  vector<char *> sql;

  sql.push_back(SQL_STMT_CREATE_USUARIO);
  sql.push_back(SQL_STMT_CREATE_ACOMODACAO);
  sql.push_back(SQL_STMT_CREATE_CARTAO);
  sql.push_back(SQL_STMT_CREATE_CONTACORRENTE);
  sql.push_back(SQL_STMT_CREATE_RESERVA);

  //inicia o banco
  if (sqlite3_open(NOME_BD, &banco) != SQLITE_OK)
  {
    throw sqlite3_errmsg(banco);
  }

  try
  {
    //inicia o banco de dados com as tabelas
    for (int i = 0; i < sql.size(); i++)
    {
      sqlite3_prepare_v2(banco, sql.at(i), -2, &stmt, NULL);

      if (sqlite3_step(stmt) == SQLITE_ERROR)
        throw sqlite3_errmsg(banco);
    }
  }
  catch (char *msg)
  {
    cout << msg << endl;
    delete msg;
    resultado = false;
  }

  sqlite3_free(errmsg);
  sqlite3_close(banco);

  return resultado;
}

Usuario *CtrlServAut::autenticar(Identificador &id, Senha &senha)
{
  int rc;
  Usuario *usu;
  sqlite3 *banco = CtrlServ::get_banco();

  if (!CtrlServ::bd_criado())
    CtrlServ::init_banco();

  rc = sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  string SQL_STMT_SELECT_USUARIO = "SELECT * FROM USUARIO WHERE ";
  SQL_STMT_SELECT_USUARIO += "identificador = '" + id.get_identificador() + "'";
  SQL_STMT_SELECT_USUARIO += " AND ";
  SQL_STMT_SELECT_USUARIO += "senha = '" + senha.get_senha() + "';";

  sqlite3_prepare_v2(banco, SQL_STMT_SELECT_USUARIO.c_str(), -2, &stmt, NULL);

  rc = sqlite3_step(stmt);

  if (trata_retorno(rc))
  {
    const char *id = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    const char *nome = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    const char *senha = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

    cout << id << endl;
    cout << nome << endl;
    cout << senha << endl;

    usu = new Usuario("jopsb", "Joao Pedro", "Senha123!");
  }

  sqlite3_finalize(stmt);
  sqlite3_close(banco);
  return usu;
}
/*************CONTROLADORA DE SERVICO - AUTENTICACAO********/

/****CONTROLADORA DE SERVICOS BASE ********/

bool CtrlServ::banco_criado = false;

bool CtrlServ::init_banco()
{
  bool resultado = true;
  sqlite3_stmt *stmt;
  vector<char *> sql;

  if (!CtrlServ::banco_criado)
  {

    sql.push_back(SQL_STMT_CREATE_USUARIO);
    sql.push_back(SQL_STMT_CREATE_ACOMODACAO);
    sql.push_back(SQL_STMT_CREATE_CARTAO);
    sql.push_back(SQL_STMT_CREATE_CONTACORRENTE);
    sql.push_back(SQL_STMT_CREATE_RESERVA);

    //inicia o banco
    if (sqlite3_open(NOME_BD, &banco) != SQLITE_OK)
    {
      throw sqlite3_errmsg(banco);
    }

    try
    {
      //inicia o banco de dados com as tabelas
      for (int i = 0; i < sql.size(); i++)
      {
        sqlite3_prepare_v2(banco, sql.at(i), -2, &stmt, NULL);

        if (sqlite3_step(stmt) == SQLITE_ERROR)
          throw sqlite3_errmsg(banco);
      }
    }
    catch (char *msg)
    {
      cout << msg << endl;
      sqlite3_free(msg);
      resultado = false;
    }

    sqlite3_close(banco);
    banco_criado = true;
  } //banco ja criado
  return resultado;
}
/****CONTROLADORA DE SERVICOS BASE ********/

int CtrlServ::executa(string sql)
{
  int rc;
  sqlite3 *banco = CtrlServ::get_banco();

  if (!CtrlServ::bd_criado())
    CtrlServ::init_banco();

  rc = sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  if (trata_retorno(rc))
  {

    sqlite3_prepare_v2(banco, sql.c_str(), -2, &stmt, NULL);

    rc = sqlite3_step(stmt);
  }

  return rc;
}
