#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>
#include <mdp.h>

/*
 * @brief usage print usage routines
 */
void usage()
{
	fprintf(stderr, "Usage: null\n");
	return;
}

/* 
 * @brief mdp parser engineering test
 * @return error:1 success:0
 */
int main(int argc, char *argv[])
{
	static int options;
 	extern char *optarg;
	//char buffer[MAX_MDP_BUFFER] = "select event_name from event where event_name <= power and meter_uid=TN123 or alarm_id> 12;";
	//char buffer[MAX_MDP_BUFFER] = "select event_name, alarm_id from event where meter_uid=A and meter_uid = B and (event_name < Primary Power Up or event_name >'Meter Cover Opened') or alarm_id<= Meter Cover Closed and alarm_id>> 'Hardware Failure' rindex<=123"; 
	//char buffer[MAX_MDP_BUFFER] = "select event_name from event where meter_uid=TUNG1234567890123456;";
	char buffer[MAX_MDP_BUFFER] = "select event_names from event_usage where event_names = xxx and meter_uid=A and meter_uid = B and (event_name < Primary Power Up or event_name >'Meter Cover Opened') or alarm_id<= Meter Cover Closed and alarm_id>> 'Hardware Failure' rindex<=123;";
	//char buffer[MAX_MDP_BUFFER] = "select * from event_usage where event_names = xxx and meter_uid='abc';";
	//char buffer[MAX_MDP_BUFFER] = "select * from load_profile_usage;";
	char *pbuffer;
	int n = 0, s = 0;

	mdp_t *mdp;

	/* token analysis */
	while((options=getopt(argc, argv, "h")) != -1) {
		   	switch(options) {
			case 'h':
				usage(); 
				return 1;
				break; 
			default:
				printf("Incorrect paramter: %c\n", options);
				usage() ;
				return 1 ; /* print help without process others opts */
		}
	}


	/* initial mdp */
	mdp = mx_mdp_init();
	if (!mdp) {
		fprintf(stderr, "mdp object init error\n");
		exit(1);
	}

	/* prepare buffer */
	mx_mdp_setbuffer(mdp, buffer, strlen(buffer));

	/* preparing the regular expression rules */
	/* desire max result for matching and sub-matching limit to 10 record */
	//if (mx_mdp_prepare(mdp, "([a-z]+[_]*[a-z]+)([ ]*[<=>]+[ ]*)([a-z0-9]+)", 10, 10)) {
	//if (mx_mdp_prepare(mdp, "(meter_uid)([ ]*[<=>]+[ ]*)([']*)([a-z0-9]+)([']*)", 10, 10)) {
	//if (mx_mdp_prepare(mdp, "meter_uid=([0-9a-zA-Z]+)", 10, 10)) {
	//if (mx_mdp_prepare(mdp, "SELECT[[:print:]]+FROM[ ]+([[:printf:]]+)_usage[]+(WHERE)?([[:printf:]]+)", 10, 10)) {
	//if (mx_mdp_prepare(mdp, "SELECT([[:print:]]+)FROM[ ]+([[:print:]]+)_usage[ ]*(WHERE)?([[:print:]]*)", 10, 10)) {
	if (mx_mdp_prepare(mdp, "a", 15, 10)) {
		fprintf(stderr, "prepare error\n");
		exit(1);
	}

	/* engine process */
	if (mx_mdp_processor(mdp))
		fprintf(stderr, "processor error or result not match\n");

	/* get number matching */
	if (mx_mdp_getnmatch(mdp, &n))
		fprintf(stderr, "processor error or result not match\n");
	else
		fprintf(stdout, "total number of matching %d\n", n);

	while (n-- != 0) {

		/* get the result */
		if (!mx_mdp_getnresult(mdp, (void *)&pbuffer, n)) {
			fprintf(stdout, "============\n");
			fprintf(stdout, "result of index[%d] is :%s:\n", n, pbuffer);
		}
		else
			fprintf(stderr, "no such index!!\n");

		/* get number sub-matching */
		if (mx_mdp_getsmatch(mdp, &s, n))
			fprintf(stderr, "processor error or result not match\n");
		else
			fprintf(stdout, "total number of sub matching of index[%d] is %d\n", n, s);

		while (s-- != 0) {
			/* get the result */
			if (!mx_mdp_getsresult(mdp, (void *)&pbuffer, n, s)) {
				fprintf(stdout, "result of index[%d][%d] is :%s:\n", n, s, pbuffer);
			}
			else
				fprintf(stderr, "no such index!!\n");
		}

	}

	/* release mdp */
	mx_mdp_release(mdp);

	return 0;
}
