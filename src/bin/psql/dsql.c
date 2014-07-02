#include "postgres_fe.h"
#include "dsql.h"
#include "common.h"

int	save_query_to_remote(char *line) {
    PGconn *conn = NULL;
    int NUM_VALUES = 8;
   
    const char **keywords = pg_malloc(NUM_VALUES * sizeof(*keywords));
    const char **values = pg_malloc(NUM_VALUES * sizeof(*values));
    const char *command = "INSERT INTO queries(user_id, query, time) VALUES ($1, $2, now())";
    const char **params = pg_malloc(2 * sizeof(*params));
    char* mac = get_mac_address();
    params[0] = mac;
    params[1] = line;

    keywords[0] = "host";
    values[0] = "localhost";
    keywords[1] = "port";
    values[1] = "5432";
    keywords[2] = "dbname";
    values[2] = "monitor";
    keywords[3] = "user";
    values[3] = "duke_cs"; 
    keywords[4] = "password";
    values[4] = "duke";
    keywords[5] = "fallback_application_name";
    values[5] = "monitor";
    keywords[6] = "client_encoding";
    values[6] = "auto"; 
    keywords[7] = NULL;
    values[7] = NULL;

    conn = PQconnectdbParams(keywords, values, true);
    free(keywords);
    free(values);

    if (PQstatus(conn) == CONNECTION_BAD) {
        printf("Bad connection: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return -1;
    }
   
    PQexecParams(conn, command, 2, NULL, params, NULL, NULL, 0);

    free(params);
    return 1;

}

char *get_mac_address() {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    //this might only work on Ubuntu, untested elsewhere
    char *def_mac = "unknown_mac_address";
    char *file_name = "/sys/class/net/eth0/address";
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Could not open file for mac address: %s", file_name);
        //exit or give a generic mac address?
        line = def_mac;
    }

    // read one line, that's all it contains
    read = getline(&line, &len, fp); 
    if (read == -1) {
        //we didn't read anything, error?
        printf("Nothing read for mac address.");
        line = def_mac;
    }   
    return line;

}
