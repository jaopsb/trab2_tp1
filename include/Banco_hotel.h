#ifndef BANCO_HOTEL_H
#define BANCO_HOTEL_H
#include <string>
#include "Data.h"
#include "sqlite3.h"
#include "Usuario.h"
#include "Retorno.h"
#include "Acomodacao.h"

class Banco_hotel
{
    public:
        Banco_hotel(char*);
        virtual ~Banco_hotel();

        Retorno* cadastra_usuario(Usuario);
        void cadastra_acomodacao(Acomodacao);
        void cadastra_contacorrente(string,int,int);
        void cadastra_cartao(string,Data);

        Usuario buscar_usuario(string,string);
    protected:

    private:
        sqlite3* bd;
        int instancia;
        sqlite3_stmt *stmt;
        char* NOME_BD = "hotel.db";

        bool existe_usuario(string,string);
};

#endif // BANCO_HOTEL_H
