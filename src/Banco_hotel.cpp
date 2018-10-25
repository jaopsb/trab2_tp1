#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include "Retorno.h"
#include "Banco_hotel.h"
#include "Acomodacao.h"

using namespace std;

using Record = vector<string>;
using Records = vector<Record>;

//instrucoes para criacao de tabelas do banco
char *SQL_STMT_CREATE_USUARIO = "CREATE TABLE IF NOT EXISTS 'USUARIO' ( `ID` INTEGER PRIMARY KEY AUTOINCREMENT,`LOGIN` TEXT NOT NULL, `NOME` TEXT NOT NULL, `SENHA` TEXT NOT NULL ); ";
char *SQL_STMT_CREATE_ACOMODACAO = "CREATE TABLE IF NOT EXISTS `ACOMODACAO` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `tipo` INTEGER NOT NULL, `capacidade` INTEGER NOT NULL, `cidade` TEXT NOT NULL, `estado` INTEGER NOT NULL, `diaria` REAL NOT NULL );";
char *SQL_STMT_CREATE_CARTAO = "CREATE TABLE IF NOT EXISTS 'CARTAO' ( `ID` INTEGER PRIMARY KEY AUTOINCREMENT,`NUMERO` TEXT NOT NULL, `DT_VALIDADE` TEXT NOT NULL, `ID_USUARIO` INTEGER NOT NULL );";
char *SQL_STMT_CREATE_CONTACORRENTE = "CREATE TABLE IF NOT EXISTS 'CONTACORRENTE' ( `ID` INTEGER PRIMARY KEY AUTOINCREMENT, `NUMERO` TEXT NOT NULL, `AGENDA` INTEGER NOT NULL, `BANCO` INTEGER NOT NULL, `ID_USUARIO` INTEGER NOT NULL );";
char *SQL_STMT_CREATE_RESERVA = "CREATE TABLE IF NOT EXISTS `RESERVA` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `id_usuario` INTEGER NOT NULL, `id_acomodacao` INTEGER NOT NULL, `data_incio` TEXT NOT NULL, `data_fim` TEXT NOT NULL );";

bool Banco_hotel::existe_usuario(string login_in, string senha_in)
{
    bool resultado = false;

    sqlite3_open(NOME_BD, &bd);

    string SQL_SELECT_USUARIO = "SELECT * FROM USUARIO WHERE login = '" + login_in + "' AND senha = '" + senha_in + "';";

    //preparando comando
    sqlite3_prepare_v2(bd, SQL_SELECT_USUARIO.c_str(), -2, &stmt, NULL);

    //executando SQL
    if (sqlite3_step(stmt) == SQLITE_OK)
    {
        resultado = !resultado;
    }

    return resultado;
}

Banco_hotel::Banco_hotel(char *nome_bd)
{
    /*
    //implementando padrao singleton
    if(instancia > 0)
    {
        throw "Erro, apenas um banco deve estar rodando!";
    }else
    {
        instancia++;
    }
*/
    char *errmsg;
    vector<char *> sql;

    sql.push_back(SQL_STMT_CREATE_USUARIO);
    sql.push_back(SQL_STMT_CREATE_ACOMODACAO);
    sql.push_back(SQL_STMT_CREATE_CARTAO);
    sql.push_back(SQL_STMT_CREATE_CONTACORRENTE);
    sql.push_back(SQL_STMT_CREATE_RESERVA);

    //inicia o banco
    if (sqlite3_open(NOME_BD, &bd) != SQLITE_OK)
        throw sqlite3_errmsg(bd);

    try
    {
        for (int i = 0; i < sql.size(); i++)
        {
            sqlite3_prepare_v2(bd, sql.at(i), -2, &stmt, NULL);

            if (sqlite3_step(stmt) == SQLITE_ERROR)
                throw sqlite3_errmsg(bd);
        }
    }
    catch (char *msg)
    {
        throw msg;
        sqlite3_free(msg);
    }

    sqlite3_free(errmsg);
    sqlite3_close(bd);
}

/*
    Funcao de cadastro de usuarios
        Funcao que recebe um objeto de usuario, e insere no banco;
*/
Retorno *Banco_hotel::cadastra_usuario(Usuario usu)
{
    int rc;
    Retorno *retorno = new Retorno();

    if (!existe_usuario(usu.get_login(), usu.get_senha()))
    {
        string SQL_STMT_INSERT_USUARIO = "INSERT INTO USUARIO (nome,login,senha) VALUES (";

        rc = sqlite3_open(NOME_BD, &bd);

        SQL_STMT_INSERT_USUARIO += "'" + usu.get_nome() + "'";
        SQL_STMT_INSERT_USUARIO += ',';
        SQL_STMT_INSERT_USUARIO += "'" + usu.get_login() + "'";
        SQL_STMT_INSERT_USUARIO += ',';
        SQL_STMT_INSERT_USUARIO += "'" + usu.get_senha() + "');";

        //preparando o comando SQL
        rc = sqlite3_prepare_v2(bd, SQL_STMT_INSERT_USUARIO.c_str(), -2, &stmt, NULL);

        //executando comando
        if (sqlite3_step(stmt) != SQLITE_OK)
        {
            retorno = new Retorno(false, sqlite3_errmsg(bd));
        }

        sqlite3_finalize(stmt);
        sqlite3_close(bd);
    }
    else
    {
        retorno = new Retorno(false, "Login ja Cadastrado!");
    }

    return retorno;
}

/*
    Funcao de buscar um usuario;
    Deve devolver um objeto usuario completo com todos os valores que serao usados no sistema
*/
Usuario Banco_hotel::buscar_usuario(string login_in, string senha_in)
{
    string SQL_STMT_SELECT_USUARIO = "SELECT * FROM USUARIO WHERE LOGIN = '" + login_in + "' and SENHA = '" + senha_in + "';";

    sqlite3_open(NOME_BD, &bd);

    //preparando o statement
    int rc = sqlite3_prepare_v2(bd, SQL_STMT_SELECT_USUARIO.c_str(), -1, &stmt, NULL);

    //executando comando SQL
    rc = sqlite3_step(stmt);

    if (!rc)
    {
        throw sqlite3_errmsg(bd);
        sqlite3_finalize(stmt);
    }

    Usuario u;

    //fazendo cast de const char* do sqlite3 para string
    u.set_nome(string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))));
    u.set_login(string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2))));
    u.set_senha(string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3))));

    sqlite3_close(bd);
    return u;
}

Banco_hotel::~Banco_hotel()
{
}
