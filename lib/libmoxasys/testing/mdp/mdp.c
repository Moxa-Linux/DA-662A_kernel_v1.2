#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>
#include <unistd.h>
#include <mdp.h>

int main(int argc, char *argv[])
{
	char buffer[4][MAX_MDP_BUFFER] = {"123bcxsfdas89y", "qwertyuiopasdfghjklcvbn", "1234567890", "!@#$%^&*(12345678@#$%"};
	char *pbuffer;
	char result[MAX_MDP_BUFFER];
	int i = 0;
	int ret = 0; 
	int error = 0;

	mdp_t	*mdp;

	mdp = mx_mdp_init();
	if (!mdp) {
		fprintf(stderr, "mdp object init error\n");
		exit(1);
	}

	/* test buffer */
	fprintf(stdout, "test...........1\n");
       	mx_mdp_setbuffer(mdp, buffer[0], strlen(buffer[0])); 
	mx_mdp_prepare(mdp, "([1-9]+)([a-z]+)", 10);
	mx_mdp_processor(mdp);
	if (mx_mdp_getnmatch(mdp, &i)) {
		fprintf(stderr, "processor error or result not match\n");
		error = 1;
	}

	while (i-- != 0) {
		/* get the result */
		if (!mx_mdp_getnresult(mdp, (void *)&pbuffer, i)) {
		       	fprintf(stdout, "result of index[%d] is %s.\n", i, pbuffer);
		}
		else
		       	error = 1;
	}

	/* test buffer */
	fprintf(stdout, "test...........2\n");
       	mx_mdp_setbuffer(mdp, buffer[1], strlen(buffer[1])); 
	mx_mdp_prepare(mdp, "([1-9]*)", 10);
	mx_mdp_processor(mdp);
	mx_mdp_getnmatch(mdp, &i);

	while (i-- != 0) {
		/* get the result */
		if (mx_mdp_getnresult(mdp, (void *)&pbuffer, i)) {
		       	fprintf(stdout, "result of index[%d] is %s.\n", i, pbuffer);
		       	error = 1;
		}
	}

	/* test buffer */
	fprintf(stdout, "test...........3\n");
       	mx_mdp_setbuffer(mdp, buffer[2], strlen(buffer[2])); 
	mx_mdp_prepare(mdp, "([a-z]*)", 10);
	mx_mdp_processor(mdp);
	mx_mdp_getnmatch(mdp, &i);

	while (i-- != 0) {
		/* get the result */
		if (mx_mdp_getnresult(mdp, (void *)&pbuffer, i)) {
		       	fprintf(stdout, "result of index[%d] is %s.\n", i, pbuffer);
		       	error = 1;
		}
	}

	/* test buffer */
	fprintf(stdout, "test...........4\n");
       	mx_mdp_setbuffer(mdp, buffer[3], strlen(buffer[3])); 
	mx_mdp_prepare(mdp, "([1-9]+)", 10);
	mx_mdp_processor(mdp);
	mx_mdp_getnmatch(mdp, &i);

	while (i-- != 0) {
		/* get the result */
		if (mx_mdp_getnresult(mdp, (void *)&pbuffer, i))
		       	error = 1;
	}

	printf("test result %s\n", (error)?"failure":"success");

	/* release mdp */
	mx_mdp_release(mdp);

	return 0;
}
