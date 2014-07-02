/* Duke University sql monitor. Saves a query line to a remote monitor server.
 * Author: Brett Walenz (bwalenz///at///cs.duke.edu)
 */ 
#ifndef DSQL_H 
#define DSQL_H 

#include "postgres_fe.h"

int			save_query_to_remote(char *line);
char *      get_mac_address(void);

#endif   /* DSQL_H */
