// macros and whatnot for translator

// dimensions
#define IMS  -4
#define IME  430
#define JMS  -4
#define JME  305
#define KMS  1
#define KME  35
#define IX  435
#define JX  310
#define KX  35

/*#define D1 (*ime - *ims + 1)
#define D2 (*kme - *kms + 1)
#define D3 (*jme - *jms + 1)*/
#define D1 (IME - IMS + 1)
#define D2 (KME - KMS + 1)
#define D3 (JME - JMS + 1)

// rgualan - modified macros
#define P2(i,j)    (i + D1*j )
#define P3(i,k,j)  (i + k*D1 + j*D1*D2)


//#define MAX(x,y) max(x,y)
//#define MIN(x,y) min(x,y)
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))



//Convert indexes
/**
 * Convert fortran index to c index
 */
#define indexI(fi) (fi+4)
#define indexJ(fj) (fj+4)
#define indexK(fk) (fk-1)





