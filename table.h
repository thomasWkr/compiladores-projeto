typedef enum
{
    Inteiro,
    Flutuante,
} tipo_t;

typedef struct
{
    int quantidade_de_parametros;
    tipo_t *tipo_dos_argumentos;
} parametros_t;