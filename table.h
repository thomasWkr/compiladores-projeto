typedef enum
{
    Inteiro,
    Flutuante,
} tipo_t;

typedef enum
{
    Literal,
    Identificador,
    Funcao,
} natureza_t;
typedef struct
{
    int quantidade_de_parametros;
    tipo_t *tipo_dos_argumentos;
} parametros_t;

// 0 - literal
// 1 - token id
typedef struct valor
{
    char *lexema;
    int no_linha;
    tipo_t tipo;
} valor_t;
typedef struct
{
    natureza_t natureza;
    parametros_t argumentos;
    valor_t dados;
} conteudo_t;
typedef struct
{
    int chave;
    conteudo_t conteudo;
} entrada_t;