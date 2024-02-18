#include "mm.h"
#define NALLOC 1024 /*minimum #units to request*/

typedef long Align;

union header
{
	struct
	{
		union header *nextPtr;
		size_t size;
	}s;
	Align x;
};

typedef union header Header;

static Header base; /*empty list to get started*/
static Header *freep=NULL; /*start of free list*/

static Header *morecore(size_t nu)
{
	char *cp,*sbrk(int);
	Header *up;

	if(nu<NALLOC)
		nu=NALLOC;
	cp=sbrk(nu*sizeof(Header));
	if(cp==(char *)-1)	/*no space at all*/
		return NULL;
	up=(Header *)cp;
	up->s.size=nu;
	myfree((void *)(up+1));

	return freep;
}

void *mymalloc(size_t size)
{
	Header *p,*prevp;
	Header *morecore(size_t);
	size_t nunits;
	
	nunits=(size+sizeof(Header)-1)/sizeof(Header)+1;
	if((prevp=freep)==NULL)				/*No free list yet*/
	{
		base.s.nextPtr=freep=prevp=&base;
		base.s.size=0;
	}
	for(p=prevp->s.nextPtr;;prevp=p,p=p->s.nextPtr)
	{
		if(p->s.size>=nunits)			/*big enough*/
		{
			if(p->s.size==nunits)		/*exactly*/
				prevp->s.nextPtr=p->s.nextPtr;
			else						/*allocate tail end*/
			{
				p->s.size-=nunits;
				p+=p->s.size;
				p->s.size=nunits;
			}
			freep=prevp;
			return (void*)(p+1);
		}
		if(p==freep)					/*wrapped around free list*/
		{
			if((p=morecore(nunits))==NULL)
				return NULL;			/*none left*/
		}
	}
}

void myfree(void *ptr)
{
	Header *bp,*p;
	
	bp=(Header *)ptr-1;		/*point to block header*/
	for(p=freep;!(bp>p&&bp<p->s.nextPtr);p=p->s.nextPtr)
	{
		if(p>=p->s.nextPtr && (bp>p||bp<p->s.nextPtr) )
		{
			break;			/*freed block at start or end of arena*/
		}
	}
	if(bp+bp->s.size==p->s.nextPtr)	/*join to upper nbr*/
	{
		bp->s.size+=p->s.nextPtr->s.size;
		bp->s.nextPtr=p->s.nextPtr->s.nextPtr;
	}
	else
	{
		bp->s.nextPtr=p->s.nextPtr;
	}
	if(p+p->s.size==bp)
	{
		p->s.size+=bp->s.size;
		p->s.nextPtr=bp->s.nextPtr;
	}
	else
	{
		p->s.nextPtr=bp;
	}

	freep=p;
}


void *myrealloc(void *ptr, size_t size)
{
	Header *bp;
	bp=(Header *)ptr-1;		/*point to block header*/
	size_t nunits;
	nunits=(size+sizeof(Header)-1)/sizeof(Header)+1;
	
	if(bp->s.size>=nunits)
	{
		return ptr;
	}

	void *newP=mymalloc(size);
	if(newP==NULL)
	{
		return NULL;
	}
	memcpy(newP,ptr,(bp->s.size-1)*sizeof(Header));
	myfree(ptr);

	return newP;
}

void *mycalloc(size_t nmemb, size_t size)
{
	void* newP=mymalloc(nmemb*size);
	
	if(newP==NULL)
	{
		return NULL;
	}

	memset(newP,0,nmemb*size);
	
	return newP;
}
