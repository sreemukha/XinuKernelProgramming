/* this is the code for the producer thread that is started by the shell command xsh_prodcons */

#include <xinu.h>
#include <future.h>

uint32 future_prod(future *fut)
{
	int32 i, j, status,length;
	
	char msg[31] = "";		
	int32	retval;			/* Return value from call	*/
	uid32	slot;			/* Slot in UDP table		*/
	uint32	serverip;		/* IP address of a 	*/
	uint32	rport = 8888;
	uint32	lport = 64999;
	char	prompt[31] = "This is a test message";	/* Message to VM server*/
	
	/* Convert time server IP address to binary */

	if (dot2ip("192.168.1.100", &serverip) == SYSERR) {
		return SYSERR;
	}
	
	/* Register the server and port */
	slot = udp_register(serverip, rport, lport);
	if (slot == SYSERR) {
		fprintf(stderr,"udptest: cannot register a udp port %d\n",
					rport);
		return SYSERR;
	}

	/* Send arbitrary message VM server */
	
	retval = udp_send(slot,	prompt, 100);
	if (retval == SYSERR) {
		fprintf(stderr,"udptest: cannot send a udp message %d\n",rport);
		udp_release(slot);
		return SYSERR;
	}

	retval = udp_recv(slot, msg, 31, 20000);
	if ( (retval == SYSERR) || (retval == TIMEOUT) ) {
		udp_release(slot);
		return SYSERR;
	}
	
	udp_release(slot);
	length = strlen(msg);
	for (j=0;j<length;j++)
	{
	    if(msg[j] >= '0' && msg[j] <= '9') {
		i = (i*10) + (msg[j] - '0');
		}
	}

	status = future_set(fut, &i);

	if (status < 1) {
		kprintf("future_set failed\n");
    		return -1;
	}
	return OK;
}