#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <string>
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

bool CtrlIUAut::autenticar()
{
  string id, snh;
  char resp;
  bool resultado = true;
  bool fim = false;

  while (!fim)
  {
    try
    {
      cout << "LOGIN" << endl;
      cout << "identificador:";
      cin >> id;
      identificador->set_identificador(id);
      cout << "Senha:";
      cin >> snh;
      senha->set_senha(snh);

      system("cls"); //windows
      fim = true;

      resultado = ctrlServAut->autenticar(*identificador, *senha);
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

  return resultado;
}

/*************CONTROLADORA DE INTERFACE DE USUARIO - AUTENTICACAO********/

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

bool CtrlServAut::autenticar(Identificador &id, Senha &senha)
{
  bool resultado = false;

  cout << id.get_identificador() << '-' << senha.get_senha() << endl;

  if (CtrlServAut::init_banco())
  {
    cout << "banco inicializado" << endl;

    if (id.get_identificador() == "jopsb" && senha.get_senha() == "Senha123!")
    {
      resultado = true;
    }
  }
  else
  {
    cout << "erro banco" << endl;
  }

  return resultado;
}
/*************CONTROLADORA DE SERVICO - AUTENTICACAO********/
/*************CONTROLADORA DE INTEFACE DE USUARIO - MENU ******/
CtrlServMenu::CtrlServMenu()
{
  identificador = new Identificador();
  senha = new Senha();
}
void CtrlIUMenu::menu()
{
  int resp = 0;

  while (resp != 5)
  {

    cout << "Sistema de Hospedagem 2.0" << endl;
    cout << "login     - " << LOGIN << endl;
    cout << "Registrar - " << REGISTRAR << endl;
    cout << "Sair      - 5" << endl;
    cout << "Selecione uma opcao";

    cin >> resp;
    system("cls"); //windows

    switch (resp)
    {
    case LOGIN:
      CtrlIUMenu::login();
      break;
    case REGISTRAR:
      CtrlIUMenu::registrar();
      break;
    }
  }
}

void CtrlIUMenu::login()
{
  char resp;
  string id, snh;
  bool resultado;
  bool fim = false;
  identificador = new Identificador();
  senha = new Senha();

  while (!fim)
  {
    try
    {
      cout << "login" << endl;
      cout << "Identificador:";
      cin >> id;
      cout << "Senha:";
      cin >> snh;

      identificador->set_identificador(id);
      senha->set_senha(snh);

      usu = ctrlServMenu->login(*identificador, *senha);

      fim = true;
    }
    catch (const invalid_argument &ia)
    {
      cout << "ERRO - " << ia.what() << endl;
      cout << "Tentar novamente? (s/S|n/N)";
      cin >> resp;
      if (resp == 'n' || resp == 'N')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUMenu::registrar()
{
  bool fim = false;
  string nm, id, snh;
  char resp;

  nome = new Nome();
  senha = new Senha();
  identificador = new Identificador();

  while (!fim)
  {
    try
    {
      cout << "Registrar" << endl;
      cout << "Nome: ";
      getchar();
      getline(cin, nm);
      cout << "Identificador (5 caracteres):";
      cin >> id;
      cout << "Senha: (8 caracteres, pelo menos um maiusculo, 1 numero e 1 simbolo):" << endl;
      cin >> snh;

      nome->set_nome(nm);
      identificador->set_identificador(id);
      senha->set_senha(snh);

      //se tudo certo, insere no banco

      if (ctrlServMenu->registrar(*nome, *identificador, *senha))
      {
        cout << "Registrado com sucesso! Pressione enter para voltar." << getchar();
      }
      fim = true;
    }
    catch (const invalid_argument &ia)
    {
      cout << "ERRO - " << ia.what() << endl;
      cout << "Tentar de novo? (s/S|n/N):";
      cin >> resp;

      if (resp == 'n' || resp == 'N')
      {
        fim = true;
      }
    }
  }
}
/*************CONTROLADORA DE INTEFACE DE USUARIO - MENU *******/
/*************CONTROLADORA DE SERVICOS DE USUARIO - REGISTRAR***/
void CtrlIUMenu::setCtrlServMenu(IServMenu *ctrl)
{
  ctrlServMenu = ctrl;
}
/*
    FUNCAO Registrar
    Verifica se o banco foi criado,
    cria o comando SQL (stmt) que sera executado
    e executa a operacao
*/
bool CtrlServMenu::registrar(Nome &n, Identificador &id, Senha &sn)
{
  int rc;
  bool resultado = false;
  sqlite3_stmt *stmt;
  sqlite3 *banco = CtrlServ::get_banco();

  if (!CtrlServ::bd_criado())
    CtrlServ::init_banco();

  rc = sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  //TODO: criar funcao para criacao de metodos gerais
  string SQL_STMT_INSERT_USUARIO = "INSERT INTO USUARIO (nome,identificador,senha) VALUES (";
  SQL_STMT_INSERT_USUARIO += "\"" + n.get_nome() + "\",";
  SQL_STMT_INSERT_USUARIO += "\"" + id.get_identificador() + "\",";
  SQL_STMT_INSERT_USUARIO += "\"" + sn.get_senha() + "\");";

  sqlite3_prepare_v2(banco, SQL_STMT_INSERT_USUARIO.c_str(), -2, &stmt, NULL);

  rc = sqlite3_step(stmt);

  //verifica se a execucao foi feita com sucesso
  resultado = trata_retorno(rc);

  sqlite3_finalize(stmt);
  sqlite3_close(banco);

  return resultado;
};

/*************CONTROLADORA DE SERVICOS DE USUARIO - REGISTRAR***/
/*************CONTROLADORA DE SERVICOS DE USUARIO - LOGIN *****/
Usuario CtrlServMenu::login(Identificador &id, Senha &sn)
{
  int rc;
Usuario usu;
  sqlite3_stmt *stmt;
  sqlite3 *banco = CtrlServ::get_banco();

  if (!CtrlServ::bd_criado())
    CtrlServ::init_banco();

  rc = sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  string SQL_STMT_SELECT_USUARIO = "SELECT * FROM USUARIO WHERE ";
  SQL_STMT_SELECT_USUARIO += "identificador = '" + id.get_identificador() + "'";
  SQL_STMT_SELECT_USUARIO += " AND ";
  SQL_STMT_SELECT_USUARIO += "senha = '" + sn.get_senha() + "';";

  sqlite3_prepare_v2(banco, SQL_STMT_SELECT_USUARIO.c_str(), -2, &stmt, NULL);

  rc = sqlite3_step(stmt);

  if (trata_retorno(rc))
  {
    const char* id = reinterpret_cast<const char*>(sqlite3_column_text(stmt,1));
    const char* nome = reinterpret_cast<const char*>(sqlite3_column_text(stmt,2));
    const char* senha = reinterpret_cast<const char*>(sqlite3_column_text(stmt,3));

    cout << id << endl;
    cout << nome << endl;
    cout << senha << endl;

    string id_tp = "jopsb";
    string nome_tp = "joao pedro";
    string senha_tp = "Senha123!";

    //usu(id_tp,nome_tp,senha_tp);

    cout << usu.get_identificador() << endl;
    cout << usu.get_nome() << endl;
    cout << usu.get_senha() << endl;
  }

  sqlite3_finalize(stmt);
  sqlite3_close(banco);
  return usu;
}
/*************CONTROLADORA DE SERVICOS DE USUARIO - LOGIN *****/
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
