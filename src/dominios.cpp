#include "dominios.h"
#include <stdexcept>
#include <vector>
#include <iostream>

using namespace std;

/** \brief adiciona o valor da agencia.
 *
 * Deve ser um numero entre 0 e 99999
 * \param int
 * \return void()
 * \throw invalid_argument
 */
void Agencia::set_agencia(int valor)
{
  Agencia::validar(valor);

  this->agencia = valor;
}

void Agencia::validar(int valor) throw(invalid_argument)
{

  if (valor < 0 || valor >= 100000)
    throw invalid_argument("O numero da conta nao confere com a especificacao");
}

/** \brief Adiciona o valor do banco na classe banco.
 *
 * Deve ser enviado com o valor entre 0 e 999.
 * \param int
 * \return void
 * \throw invalid_argument
 */
void Banco::set_banco(int valor)
{
  Banco::validar(valor);
  this->banco = valor;
}

void Banco::validar(int valor) throw(invalid_argument)
{
  //de acordo com a especificacao o valor nao deve ultrapassar 999 nem ser menor que 0000
  if (valor < 0 || valor >= 1000)
    throw invalid_argument("O numero do banco nao confere com a especificacao");
}

/** \brief Adiciona o valor correspondente da capacidade de acomodacao.
 *
 *  Para ser valido, deve ser entre 1 e 9.
 * \param int
 * \return void
 * \throw invalid_argument
 */
void Capacidade_Acomodacao::set_capacidade_acomodacao(int valor)
{
  Capacidade_Acomodacao::validar(valor);

  this->cap_acom = valor;
}

void Capacidade_Acomodacao::validar(int valor) throw(invalid_argument)
{
  //de acordo com a especificacao, o valor deve ser entre 1 e 9
  if (valor <= 0 || valor >= 10)
    throw invalid_argument("O valor da capacidade de acomodacao nao eh valido");
}

/** \brief Adiciona o valor correspondente a diaria.
 *
 *  O valor nao pode exceder a quantia de R$ 10000,00 e nem ser menor do que R$ 1,00
 * \param float
 * \return void
 * \throw invalid_argument
 */
void Diaria::set_diaria(float valor)
{
  Diaria::validar(valor);

  this->diaria = valor;
}

void Diaria::validar(float valor) throw(invalid_argument)
{
  //diarias mais baixas que R$ 1,00 nao serao validas
  if (valor < 1.00)
    throw invalid_argument("O valor da diaria nao pode ser menor que R$ 1,00");

  //nem as acima de R$ 10000,00
  if (valor > 10000.00)
    throw invalid_argument("O valor da diaria nao pode ser maior que R$ 10000,00");
}

/** \brief Adiciona o valor da data a classe Data.
 *
 * A data deve ser descrita da forma 'DD/MMM/AAAA', não será valido anos abaixo de 2000 e acima de 2099.
 * Anos bissextos são considedrados, o mes devera ser uma das siglas validas:
 * jan,fev,mar,abr,mai,jun,jul,ago,set,out,nov,dez.
 * \param string
 * \return void
 * \throw invalid_argument
 */
void Data::set_data(string valor)
{

  Data::validar(valor);

  this->data = valor;
}

void Data::validar(string valor) throw(invalid_argument)
{
  //verifica se os valores estao vazios ou preenchidos com espaco
  if (valor.empty() || valor == " ")
    throw invalid_argument("Data nao pode ser vazia");

  string buff{""};
  vector<string> v;

  /* loop que verifica se ha / e extrai os valoes para um vetor de strings */
  for (auto n : valor)
  {
    if (n != '/')
    {

      buff += n;
    }
    else if ((n == '/') && buff != "")
    {

      v.push_back(buff);
      buff = "";
    }
  }
  if (buff != "")
    v.push_back(buff);

  //convertendo string para int -> dia e ano
  int dia_aux = stoi(v.at(0));
  int ano_aux = stoi(v.at(2));

  //salvando mes
  string mes_aux = v.at(1);

  /* validacoes de caso: */
  //se o ano eh menor que 2000
  if (ano_aux < 2000)
    throw invalid_argument("Ano nao pode ser menor que 2000");

  //se o ano eh maior que 2099
  if (ano_aux > 2099)
    throw invalid_argument("Ano nao pode ser maior que 2099");

  //se fevereiro tem 29 dias em ano bissexto
  if (dia_aux == 29 && mes_aux == "fev" && ano_aux % 4 != 0)
    throw invalid_argument("Data invalida para ano nao bissexto");

  //se o mes foi escrito conforme especificacao
  if (!(mes_aux == "jan" || mes_aux == "fev" || mes_aux == "mar" ||
        mes_aux == "abr" || mes_aux == "mai" || mes_aux == "jun" ||
        mes_aux == "jul" || mes_aux == "ago" || mes_aux == "set" ||
        mes_aux == "out" || mes_aux == "nov" || mes_aux == "dez"))
    throw invalid_argument("Mes incorreto");

  //se fevereiro tem mais de 29 dias
  if (mes_aux == "fev" && dia_aux > 29)
    throw invalid_argument("Fevereiro nao tem mais de 28 dias");

  //valida se o mes tem a quantidade correta de dias
  if ((mes_aux == "jan" || mes_aux == "mar" || mes_aux == "mai" ||
       mes_aux == "jul" || mes_aux == "ago" || mes_aux == "out" ||
       mes_aux == "dez") &&
      dia_aux > 31)
    throw invalid_argument(mes_aux + " nao tem mais de 31 dias");

  //valida se o mes tem a quantidade correta de dias
  if ((mes_aux == "abr" || mes_aux == "jun" || mes_aux == "set" || mes_aux == "nov") && dia_aux > 30)
    throw invalid_argument(mes_aux + " nao tem mais de 31 dias");
}

/**\brief Adiciona a data de validade do cartao. Deve ser na forma MM/AA.
 *
 * Para o campo MM, é aceito apenas valores de 01 a 12.
 * Para o campo AA os valores devem estar entre 00 e 99.
 *\param string
 *\return void
 * \throw invalid_argument
 */
void Data_Validade::set_data_validade(string valor)
{

  Data_Validade::validar(valor);

  this->data_val = valor;
}

void Data_Validade::validar(string valor) throw(invalid_argument)
{
  //verifica se valor esta vazio ou preenchido com apenas um espaco
  if (valor.empty() || valor == " ")
    throw invalid_argument("Data de validade nao pode ser vazia");

  string buff{""};
  vector<string> v;

  //loop que busca entre o caracter / e adiciona no vetor de string
  for (auto n : valor)
  {
    if (n != '/')
    {

      buff += n;
    }
    else if ((n == '/') && buff != "")
    {

      v.push_back(buff);
      buff = "";
    }
  }
  if (buff != "")
    v.push_back(buff);

  if (v.size() >= 3)
    throw invalid_argument("Formato de data de validade invalido");

  //convertendo string para int o mes e ano
  int mes_aux = stoi(v.at(0));
  int ano_aux = stoi(v.at(1));

  //se o mes esta entre 1 e 12
  if (mes_aux < 1 || mes_aux > 12)
    throw invalid_argument("Mes nao existe");

  //se o ano esta entre 00 e 99
  if (ano_aux < 0 || ano_aux > 99)
    throw invalid_argument("Ano nao esta sendo descrito corretamente");
}

/**\brief Guarda o valor referente a sigla de um Estado.
 *
 * Deve ser um dos valores a seguir:
 * AC,AL,AP,AM,BA,CE,DF,ES,GO,MA,MT,MS,MG,PA,PB,PR,PE,PI,RJ,RN,RS,RO,RR,SC,SP,SE,TO
 *\param string
 *\return void
 * \throw invalid_argument
 */
void Estado::set_estado(string valor)
{

  Estado::validar(valor);

  this->estado = valor;
}

void Estado::validar(string valor) throw(invalid_argument)
{
  //verifica se valor esta dentro da especificacao
  if (!(valor == "AC" || valor == "AL" || valor == "AP" ||
        valor == "AM" || valor == "BA" || valor == "CE" ||
        valor == "DF" || valor == "ES" || valor == "GO" ||
        valor == "MA" || valor == "MT" || valor == "MS" ||
        valor == "MG" || valor == "PA" || valor == "PB" ||
        valor == "PR" || valor == "PE" || valor == "PI" ||
        valor == "RJ" || valor == "RN" || valor == "RS" ||
        valor == "RO" || valor == "RR" || valor == "SC" ||
        valor == "SP" || valor == "SE" || valor == "TO"))
    throw invalid_argument("Sigla do estado nao eh valida");
}

/**\brief Funcao de insercao de um valor para o identificador.
 *
 * Deve ser enviado um valor do tipo string que seja nao vazio, com tamanho 5, qualquer outro acarreta em erro.
 * O valor nao deve conter letras maiusculas nem números ou simbolos
 * \param string
 * \return void
 * \throw invalid_argument
*/
void Identificador::set_identificador(string valor)
{
  Identificador::validar(valor);

  this->identificador = valor;
}

void Identificador::validar(string valor) throw(invalid_argument)
{

  int i = 0;
  int minuscula = 0;

  //valida se o valor esta vazio ou com apenas um espaço
  if (valor.empty() || valor == " ")
    throw invalid_argument("Identificador nao pode estar vazio");

  //valida se o tamanho esta dentro da especificacao dada
  if (valor.length() < 5 || valor.length() > 5)
    throw invalid_argument("Identificador nao confere com o tamanho especificado");

  //pesquisa se o valor contem letras minusculas
  while (valor[i])
  {

    if ((isalpha(valor[i])) && (int(valor[i]) >= 97 && int(valor[i]) <= 122))
      minuscula++;

    i++;
  }

  //se todo o documento nao conter apenas minusculas, ha o lancamento de uma excecao
  if (minuscula != 5)
    throw invalid_argument("O identificador deve ser constituido apenas de letras minusculas");
}

/**\brief Insere um valor para o Nome.
 *
 * Não deve ser vazio nem nulo, pode conter letras e números, apenas não pode conter dois espaços seguidos.
 *
 * \param string
 * \return void
 * \throw invalid_argument
*/
void Nome::set_nome(string valor)
{
  Nome::validar(valor);

  this->nome = valor;
}

void Nome::validar(string valor) throw(invalid_argument)
{

  int i = 0;

  //se existem dois espacos seguidos
  while (valor[i])
  {

    if (valor[i] == 32 && valor[i + 1] == 32)
      throw invalid_argument("Nao pode haver espaco duplo no nome");

    i++;
  }
}

/**\brief insere um numero de cartao de credito valido.
 *
 * Deve ser visto que a validação do dado é feita com base no
 * algoritmo de Luhn (<a href="https://en.wikipedia.org/wiki/Luhn_algorithm"> Algoritmo de Luhn</a>)
 * A Funcao recebe uma string com 16 caracteres, numericos contendo o numero do cartao de credito a ser guardado
 * \param string
 * \throw invalid_argument
*/
void Numero_Cartao::set_numero_cartao(string valor)
{
  Numero_Cartao::validar(valor);

  this->numero_cartao = valor;
}

void Numero_Cartao::validar(string valor) throw(invalid_argument)
{

  int soma = 0;
  int nDigits = valor.length();

  bool isSecond = false;

  for (int i = nDigits - 1; i >= 0; i--)
  {

    int d = valor[i] - 'a';

    if (isSecond == true)
      d = d * 2;

    //adicionando 2 digitos para o algoritmo funcionar corretamente
    soma += d / 10;
    soma += d % 10;

    isSecond = !isSecond;
  }

  if (soma % 10 != 0)
    throw invalid_argument("Numero do cartao invalido");
}

/**\brief insere o numero da conta corrente
 *
 * Deve ser uma string com apenas numeros, sem letras ou simbolos.
 * Tem de ter tamanho exato de 6 caracteres
 * \param string
 * \return void
 * \throw invalid_argument
*/
void Numero_Conta_Corrente::set_numero_conta_corrente(string valor)
{
  Numero_Conta_Corrente::validar(valor);

  this->numero_conta_corrente = valor;
}

void Numero_Conta_Corrente::validar(string valor) throw(invalid_argument)
{
  int i = 0;

  while (valor[i])
  {

    if (isalpha(valor[i]))
      throw("Numero da conta invalido, use apenas numeros");

    i++;
  }

  if (valor.length() != 6)
    throw invalid_argument("Conta invalida");
}

/**\brief Insere uma senha
 *
 * Deve ser seguida as regras da documentacao: <br>
 * Deve conter pelo menos 8 digitos; <br>
 * Ter pelo menos uma letra maiuscula; <br>
 * Ter pelo menos um numero; <br>
 * Ter pelo menos um simbolo; <br>
 *
 * \param string
 * \return void
 * \throw invalid_argument
*/
void Senha::set_senha(string valor)
{
  Senha::validar(valor);

  this->senha = valor;
}

void Senha::validar(string valor) throw(invalid_argument)
{

  int i = 0;
  int numero = 0;
  int simbolo = 0;
  int maiuscula = 0;

  if (valor.length() <= 8)
    throw invalid_argument("Senha deve ter pelo menos 8 digitos");

  while (valor[i])
  {

    if (isalpha(valor[i]))
    {

      if (int(valor[i]) >= 65 && int(valor[i]) <= 90)
        maiuscula++;
    }
    else if ((int(valor[i]) >= 31 && int(valor[i]) <= 47) || (int(valor[i]) >= 58 && int(valor[i]) <= 64))
    {

      simbolo++;
    }
    else
    {

      numero++;
    }
    i++;
  }

  //se nao tem numero
  if (maiuscula != 0 && numero == 0 && simbolo != 0)
    throw invalid_argument("A senha deve conter pelo menos 1 numero!");

  //se nao tem maiuscula
  if (maiuscula == 0 && numero != 0 && simbolo != 0)
    throw invalid_argument("A senha deve conter pelo menos uma letra maiuscula!");

  //se nao tem simbolo
  if (maiuscula != 0 && numero != 0 && simbolo == 0)
    throw invalid_argument("A senha deve conter pelo menos um simbolo!");

  //se nao tem nenhum alem de minuscula
  if (maiuscula == 0 && numero == 0 && simbolo == 0)
    throw invalid_argument("A senha deve conter pelo menos uma letra maiuscula, um numero e um simbolo!");
}

/**\brief insere um tipo de acomodacao
 *
 * Para inserir o valor do tipo de acomodacao deve ser: <b>
 * 1 - Apartamento <br>
 * 2 - Casa <br>
 * 3 - Flat <br>
 *
 * \param string
 * \return void
 * \throw invvalid_argument
*/
void Tipo_Acomodacao::set_tipo_acomodacao(int valor)
{
  Tipo_Acomodacao::validar(valor);

  this->tipo_acomodacao = valor;
}

void Tipo_Acomodacao::validar(int valor) throw(invalid_argument)
{
  //soh pode ser 1 ,2 ou 3
  if (valor < 0 || valor > 3)
    throw invalid_argument("Tipo de acomodacao invalido");
}
