#include <iostream>
#include <stdexcept>
#include "dominios.hpp"
#include "Entidades.hpp"
#include "Testes.hpp"

using namespace std;

/**\brief tem_teste(). Funcao geral para realizar um teste.
*
* Cada classe utiliza a função para enviar a funcao de teste e padronizar a saida, seja sucesso ou erro
* Recebe uma string com o nome da funcao e uma funcao estatica por pornteiro.
*\param string, void(*f)(void)
*\return void
*/
void Teste::temp_teste(string nome_func, void (*f)(void))
{
  try
  {

    cout << "Teste " << nome_func;
    (*f)();
    cout << " - OK" << endl;
  }
  catch (const invalid_argument &ia)
  {
    cout << " - Erro ao realizar o teste " << nome_func << ": " << ia.what() << endl;
  }
}

/**\brief run(). Funcao que roda todos os testes das classes
 *
 * Para Cada classe implementada nas classes ou Entidades ou Dominios, a funcao run() roda seus suites.
 * Para rodar testes separados, use a funcao rodar_suite() de cada classe separadamente.
 * \return void
*/
void Teste::run() {}

/**\brief run(). Funcao que roda todos os testes dos Dominios
 *
 * Para cada classe implementada na classe Dominos, a funcao run() chama todas as funcoes roda_suite()
 *
 * \param void
 * \return void
*/
void Teste_Dominios::run()
{
  Teste_Agencia t_agencia;
  Teste_Banco t_banco;
  Teste_Capacidade_Acomodacao t_cap_acm;
  Teste_Data t_data;
  Teste_Data_Validade t_data_val;
  Teste_Diaria t_diaria;
  Teste_Estado t_estado;
  Teste_Identificador t_identificador;
  Teste_Nome t_nome;
  Teste_Numero_Cartao t_num_cartao;
  Teste_Numero_Conta_Corrente t_num_cc;
  Teste_Senha t_senha;
  Teste_Tipo_Acomodacao t_tipo_acm;

  t_agencia.rodar_suite();
  t_banco.rodar_suite();
  t_cap_acm.rodar_suite();
  t_data.rodar_suite();
  t_data_val.rodar_suite();
  t_diaria.rodar_suite();
  t_estado.rodar_suite();
  t_identificador.rodar_suite();
  t_nome.rodar_suite();
  t_num_cartao.rodar_suite();
  t_num_cc.rodar_suite();
  t_senha.rodar_suite();
  t_tipo_acm.rodar_suite();
}

void Teste_Entidades::run()
{
  Teste_Usuario t_usuario;
  Teste_Acomodacao t_acomodacao;
  Teste_Cartao_de_Credito t_cartao_credito;
  Teste_Conta_Corrente t_conta_corrente;

  t_usuario.rodar_suite();
  t_acomodacao.rodar_suite();
  t_cartao_credito.rodar_suite();
  t_conta_corrente.rodar_suite();
}

/******* SUITES DE TESTES *******/

/**\brief rodar_suite().Executa todos os testes de sucesos e erro para cada classe.
 *
 * Nos Dominios, é verificado tanto a inserção de um valor de acordo com a especificação de cada função
 * como também um valor fora da especificação, verificando se há um erro esperado.
 * \param bool
 * \return true se todos os Teste rodarem corretamente e false se algum der erro
*/
bool Teste::rodar_suite()
{
  return false;
};

bool Teste_Usuario::rodar_suite()
{
  try
  {
    temp_teste("Cria Usuario com sucesso", testa_usuario_ok);
    temp_teste("Cria Usuario com erro", testa_usuario_erro);
  }
  catch (const invalid_argument &ia)
  {
    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Acomodacao::rodar_suite()
{
  try
  {
    temp_teste("Cria Acomodacao com sucesso", testa_acomodacao_ok);
    temp_teste("Cria Acomodacao com erro", testa_acomodacao_erro);
  }
  catch (const invalid_argument &ia)
  {
    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Cartao_de_Credito::rodar_suite()
{
  try
  {
    temp_teste("Cria Cartao de Credito com sucesso", testa_cartao_ok);
    temp_teste("Cria Cartao de Credito com erro", testa_cartao_erro);
  }
  catch (const invalid_argument &ia)
  {
    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Conta_Corrente::rodar_suite()
{
  try
  {
    temp_teste("Cria Conta Corrente com sucesso", testa_conta_corrente_ok);
    temp_teste("Cria Conta Corrente com erro", testa_conta_corrente_erro);
  }
  catch (const invalid_argument &ia)
  {
    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

/****************** TESTES DOS DOMINIOS *********************/
bool Teste_Agencia::rodar_suite()
{

  try
  {

    temp_teste("Insere agencia com sucesso", testa_agencia_ok);
    temp_teste("Insere agencia com erro", testa_agencia_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Banco::rodar_suite()
{

  try
  {

    temp_teste("Insere banco com sucesso", testa_banco_ok);
    temp_teste("Insere banco com erro", testa_banco_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Capacidade_Acomodacao::rodar_suite()
{

  try
  {

    temp_teste("Insere capacidade de acomodacao com sucesso", testa_capacidade_acomodacao_ok);
    temp_teste("Insere capacidade de acomodacao com erro", testa_capacidade_acomodacao_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Data::rodar_suite()
{

  try
  {

    temp_teste("Insere data com sucesso", testa_data_ok);
    temp_teste("Insere data com erro", testa_data_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Data_Validade::rodar_suite()
{

  try
  {

    temp_teste("Insere data validade com sucesso", testa_data_validade_ok);
    temp_teste("Insere data validade com erro", testa_data_validade_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Diaria::rodar_suite()
{

  try
  {

    temp_teste("Insere diaria com sucesso", testa_diaria_ok);
    temp_teste("Insere diaria com erro", testa_diaria_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Estado::rodar_suite()
{

  try
  {

    temp_teste("Insere estado com sucesso", testa_estado_ok);
    temp_teste("Insere estado com erro", testa_estado_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Identificador::rodar_suite()
{

  try
  {

    temp_teste("Insere identificador com sucesso", testa_identificador_ok);
    temp_teste("Insere identificador com erro", testa_identificador_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Nome::rodar_suite()
{

  try
  {

    temp_teste("Insere nome com sucesso", testa_nome_ok);
    temp_teste("Insere nome com erro", testa_nome_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Numero_Cartao::rodar_suite()
{

  try
  {

    temp_teste("Insere numero cartao com sucesso", testa_numero_cartao_ok);
    temp_teste("Insere numero cartao com erro", testa_numero_cartao_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Numero_Conta_Corrente::rodar_suite()
{

  try
  {

    temp_teste("Insere numero conta corrente com sucesso", testa_numero_conta_corrente_ok);
    temp_teste("Insere numero conta corrente com erro", testa_numero_conta_corrente_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Senha::rodar_suite()
{

  try
  {

    temp_teste("Insere senha com sucesso", testa_senha_ok);
    temp_teste("Insere senha com erro", testa_senha_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

bool Teste_Tipo_Acomodacao::rodar_suite()
{

  try
  {

    temp_teste("Insere tipo de acomodacao com sucesso", testa_tipo_acomodacao_ok);
    temp_teste("Insere tipo de acomodacao com erro", testa_tipo_acomodacao_erro);
  }
  catch (const invalid_argument &ia)
  {

    cout << ia.what() << endl;
    resultado = false;
  }

  return resultado;
}

/******** TESTES DE SUCESSO E ERRO UNITARIOS - ENTIDADES *********/

/*\brief Validação da inserção de valores para um Usuario que correspondem a especificação*/
void Teste_Usuario::testa_usuario_ok()
{
  Usuario u = Usuario("ideok", "joao", "Senha123!");
}

/*\brief Validação da inserção de valores para um Usuario que não correspondem a especificação*/
void Teste_Usuario::testa_usuario_erro() throw(invalid_argument)
{
  bool resultado = false;

  try
  {
    Usuario u = Usuario("2ashdhysdauy", "ijadsiajsd   ", "a");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {
    throw invalid_argument("Teste set_usuario_erro() falhou");
  }
}

/*\brief Validação da inserção de valores para uma Acomodação que correspondem a especificação*/
void Teste_Acomodacao::testa_acomodacao_ok()
{
  Acomodacao acom = Acomodacao("teste acomodacao ok",1, "Brasilia", 150.40, "DF", "ideok", 1,"10/jun/1994","10/jun/2004");
}

/*\brief Validação da inserção de valores para uma Acomodação que não correspondem a especificação*/
void Teste_Acomodacao::testa_acomodacao_erro() throw(invalid_argument)
{
  bool resultado = false;

  try
  {
    Acomodacao acom = Acomodacao("teste acomodacao erro",0, "uhdshasiu", 0, "yduyda", "iadshydasyhi189", 0,"13/reuhr/1239","31/dsjd/39393");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {
    throw invalid_argument("teste set_acomodacao_erro() falhou");
  }
}

/*\brief Validação da inserção de valores para um Cartão de credito que correspondem a especificação*/
void Teste_Cartao_de_Credito::testa_cartao_ok()
{
  Cartao_de_Credito cc = Cartao_de_Credito("5162205644883793", "04/19");
}

/*\brief Validação da inserção de valores para um Cartão de Credito que não correspondem a especificação*/
void Teste_Cartao_de_Credito::testa_cartao_erro() throw(invalid_argument)
{
  bool resultado = false;

  try
  {
    Cartao_de_Credito cc = Cartao_de_Credito("218928231893218", "12/99/32");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {
    throw invalid_argument("teste testa_cartao_erro() falhou");
  }
}

/*\brief Validação da inserção de valores para uma Conta Corrente que correspondem a especificação*/
void Teste_Conta_Corrente::testa_conta_corrente_ok()
{
  Conta_corrente cc = Conta_corrente(260, 980, "123456");
}

/*\brief Validação da inserção de valores para uma Conta Corrente que não correspondem a especificação*/
void Teste_Conta_Corrente::testa_conta_corrente_erro() throw(invalid_argument)
{
  bool resultado = false;

  try
  {
    Conta_corrente cc = Conta_corrente(777, 27312793173, "93847029102888h");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {
    throw invalid_argument("teste testa_conta_corrente() falhou");
  }
}

/******** TESTES DE SUCESSO E ERRO UNITARIOS - DOMINIOS *********/

/**\brief Validacao de insercao de um valor que confere com a especificação da classe Agencia*/
void Teste_Agencia::testa_agencia_ok()
{
  Agencia agencia;

  agencia.set_agencia(45678);
}

/**\brief Validação da inserção de um valor que nao corresponde a especificação da classe Agencia
*/
void Teste_Agencia::testa_agencia_erro() throw(invalid_argument)
{
  bool resultado = false;
  Agencia agencia;

  try
  {

    agencia.set_agencia(19293847);
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {
    throw invalid_argument("Teste testa_agencia() falhou");
  }
}

/**\brief Validacao da insercao de um valor que corresponde com a especificacao do valor de um banco*/
void Teste_Banco::testa_banco_ok()
{
  Banco banco;

  banco.set_banco(260);
}

/**\brief Validacao da insercao de um valor que nao corresponde com a especificacao do valor de um banco*/
void Teste_Banco::testa_banco_erro() throw(invalid_argument)
{
  bool resultado = false;
  Banco banco;

  try
  {

    banco.set_banco(-25948);
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_banco() falhou");
  }
}

/**\brief Validacao da insercao de um valor que corresponde com a especificacao de um valor para a capacidade de acomodacao*/
void Teste_Capacidade_Acomodacao::testa_capacidade_acomodacao_ok()
{
  Capacidade_Acomodacao capacidade_acomodacao;

  capacidade_acomodacao.set_capacidade_acomodacao(7);
}

/**\brief Validacao da insercao de um valor nao corresponde com a especificacao de um valor para a capacidade de acomocacao*/
void Teste_Capacidade_Acomodacao::testa_capacidade_acomodacao_erro() throw(invalid_argument)
{
  bool resultado = false;
  Capacidade_Acomodacao capacidade_acomodacao;

  try
  {

    capacidade_acomodacao.set_capacidade_acomodacao(2323);
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_capacidade_acomodacao() falhou");
  }
}

/**\brief Validacao de insercao de um valor que confere com a especificação de um valor para a diaria */
void Teste_Diaria::testa_diaria_ok()
{
  Diaria diaria;

  diaria.set_diaria(723.44);
}

/**\brief Validacao da insercao de um valor nao corresponde com a especificacao de um valor para a diaria*/
void Teste_Diaria::testa_diaria_erro() throw(invalid_argument)
{
  bool resultado = false;
  Diaria diaria;

  try
  {

    diaria.set_diaria(10000.001);
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_diaria() falhou");
  }
}

/**\brief Validacao de insercao de um valor que confere com a especificação de um valor para a data*/
void Teste_Data::testa_data_ok()
{
  Data data;

  data.set_data("11/set/2001");
}

/**\brief Validacao da insercao de um valor nao corresponde com a especificacao de um valor para a data*/
void Teste_Data::testa_data_erro() throw(invalid_argument)
{
  bool resultado = false;
  Data data;

  try
  {

    data.set_data("34/Jul/32");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_data() falhou");
  }
}

/**\brief Validacao de insercao de um valor que confere com a especificação de um valor para a data de validade */
void Teste_Data_Validade::testa_data_validade_ok()
{
  Data_Validade data_validade;

  data_validade.set_data_validade("12/44");
}

/**\brief Validacao da insercao de um valor nao corresponde com a especificacao de um valor para a data de validade*/
void Teste_Data_Validade::testa_data_validade_erro() throw(invalid_argument)
{
  bool resultado = false;
  Data_Validade data_validade;

  try
  {

    data_validade.set_data_validade("44/84748");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_data_validade() falhou");
  }
}

/**\brief Validacao de insercao de um valor que confere com a especificação de um valor para o estado */
void Teste_Estado::testa_estado_ok()
{
  Estado estado;

  estado.set_estado("DF");
}

/**\brief Validacao da insercao de um valor nao corresponde com a especificacao de um valor para o estado*/
void Teste_Estado::testa_estado_erro() throw(invalid_argument)
{
  bool resultado = false;
  Estado estado;

  try
  {

    estado.set_estado("DH");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_estado() falhou");
  }
}

/**\brief Validacao de insercao de um valor que confere com a especificação de um valor para o identificador*/
void Teste_Identificador::testa_identificador_ok()
{
  Identificador identificador;

  identificador.set_identificador("ideok");
}

/**\brief Validacao da insercao de um valor nao corresponde com a especificacao de um valor para o identificador*/
void Teste_Identificador::testa_identificador_erro() throw(invalid_argument)
{
  bool resultado = false;
  Identificador identificador;

  try
  {

    identificador.set_identificador("ADUAHDUAHDUAH");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_identificador() falhou");
  }
}

/**\brief Validacao de insercao de um valor que confere com a especificação de um valor para o nome */
void Teste_Nome::testa_nome_ok()
{
  Nome nome;
  nome.set_nome("João Pedro");
}

/**\brief Validacao da insercao de um valor nao corresponde com a especificacao de um valor para o nome*/
void Teste_Nome::testa_nome_erro() throw(invalid_argument)
{
  bool resultado = false;
  Nome nome;

  try
  {

    nome.set_nome("JIAIDSJ  eodmemde");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_diaria() falhou");
  }
}

/**\brief Validacao de insercao de um valor que confere com a especificação de um valor para o numero do cartao*/
void Teste_Numero_Cartao::testa_numero_cartao_ok()
{
  Numero_Cartao numero_cartao;

  numero_cartao.set_numero_cartao("5162205644883793");
}

/**\brief Validacao da insercao de um valor nao corresponde com a especificacao de um valor para o numero cartao*/
void Teste_Numero_Cartao::testa_numero_cartao_erro() throw(invalid_argument)
{
  bool resultado = false;
  Numero_Cartao numero_cartao;

  try
  {

    numero_cartao.set_numero_cartao("77876554");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_numero_cartao() falhou");
  }
}

/**\brief Validacao de insercao de um valor que confere com a especificação de um valor para o numero da conta corrente*/
void Teste_Numero_Conta_Corrente::testa_numero_conta_corrente_ok()
{
  Numero_Conta_Corrente numero_conta_corrente;

  numero_conta_corrente.set_numero_conta_corrente("003962");
}

/**\brief Validacao da insercao de um valor nao corresponde com a especificacao de um valor para o numero da conta corrente*/
void Teste_Numero_Conta_Corrente::testa_numero_conta_corrente_erro() throw(invalid_argument)
{
  bool resultado = false;
  Numero_Conta_Corrente numero_conta_corrente;

  try
  {

    numero_conta_corrente.set_numero_conta_corrente("103183812031");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_numero_conta_corrente() falhou");
  }
}

/**\brief Validacao de insercao de um valor que confere com a especificação de um valor para a senha */
void Teste_Senha::testa_senha_ok()
{
  Senha senha;

  senha.set_senha("Senha123!");
}

/**\brief Validacao da insercao de um valor nao corresponde com a especificacao de um valor para a senha*/
void Teste_Senha::testa_senha_erro() throw(invalid_argument)
{
  bool resultado = false;
  Senha senha;

  try
  {

    senha.set_senha("sndjsndheuhdue");
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_senha() falhou");
  }
}

/**\brief Validacao de insercao de um valor que confere com a especificação de um valor para o tipo de acomodacao */
void Teste_Tipo_Acomodacao::testa_tipo_acomodacao_ok()
{
  Tipo_Acomodacao tipo_acomodacao;

  tipo_acomodacao.set_tipo_acomodacao(1);
}

/**\brief Validacao da insercao de um valor nao corresponde com a especificacao de um valor para o tipo de acomodacao*/
void Teste_Tipo_Acomodacao::testa_tipo_acomodacao_erro() throw(invalid_argument)
{
  bool resultado = false;
  Tipo_Acomodacao tipo_acomodacao;

  try
  {

    tipo_acomodacao.set_tipo_acomodacao(5);
  }
  catch (const invalid_argument &ia)
  {
    resultado = true;
  }

  if (!resultado)
  {

    throw invalid_argument("Teste set_tipo_acomodacao() falhou");
  }
}
