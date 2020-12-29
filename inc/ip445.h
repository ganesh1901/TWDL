typedef int BOOL;
typedef unsigned char byte;

#ifdef __cplusplus
extern "C"
{
#ifdef SELECT_BIG_ENDIAN
struct map445			/* Memory map of the I/O board */
{
	byte    unused1;        /* undefined */    
	byte    cntl_reg;       /* IP445 control register */
	struct io445
	{
		byte unused2;   /* undefined */
		byte io_port;   /* IO registers */
	} io_map[4];
};
#else				/* Little ENDIAN structure */
struct map445			/* Memory map of the I/O board */
{
	byte    cntl_reg;       /* IP445 control register */
	byte    unused1;        /* undefined */    
	struct io445
	{
		byte io_port;   /* IO registers */
		byte unused2;   /* undefined */
	} io_map[4];
};
#endif /* SELECT_BIG_ENDIAN */


/*
	Defined below is the structure for the Status Command function.
*/

struct sblk445
{
	struct map445 *brd_ptr;     /* pointer to base address of board. */
	byte id_prom[32];           /* id prom contents. */
	char slotLetter;	    /* IP slot letter */
	int nHandle;		    /* handle to an open carrier board */
	BOOL bCarrier;		    /* flag indicating a carrier is open */
	BOOL bInitialized;	    /* flag indicating ready to talk to board */	
};


/*
	DECLARE MODULES CALLED:
*/

void psts445(struct sblk445 *);	/* routine which calls the Read I.D. Command */
void rsts445(struct sblk445 *);	/* routine to read I.D. information */
long rpnt445(struct sblk445 *,
			unsigned port,			/* the I/O port number */
			unsigned point);		/* routine to read a point */
long rprt445(struct sblk445 *,
			unsigned port);			/* routine to read a port */
long wpnt445(struct sblk445 *,
			unsigned port,			/* the I/O port number */
			unsigned point,			/* the I/O point of a port */
			byte value);			/* routine to write a point */
long wprt445(struct sblk445 *,
			unsigned port,			/* the I/O port number */
			byte value);			/* routine to write a port */
long wcntl445(struct sblk445 *,
			byte value);			/* routine to write control register */    
long get_param(void);				/* input a parameter */
void access_EEPROM(struct sblk445 *c_blk);  /* EEPROM read/write */
}
#endif
