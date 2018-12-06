/* $XFree86: xc/programs/Xserver/hw/xfree86/vga256/drivers/mga/mga.h,v 3.8.2.8 1998/10/20 20:51:20 hohndel Exp $ */
/*
 * MGA Millennium (MGA2064W) functions
 *
 * Copyright 1996 The XFree86 Project, Inc.
 *
 * Authors
 *		Dirk Hohndel
 *			hohndel@XFree86.Org
 *		David Dawes
 *			dawes@XFree86.Org
 */

#ifndef MGA_H
#define MGA_H

#include "xf86_PCI.h"
#include "mga_reg.h"
#include "misc.h"


#if !defined(EXTRADEBUG)
#define INREG8(addr) *(volatile CARD8 *)(MGAMMIOBase + (addr))
#define INREG16(addr) *(volatile CARD16 *)(MGAMMIOBase + (addr))
#define INREG(addr) *(volatile CARD32 *)(MGAMMIOBase + (addr))
#define OUTREG8(addr, val) *(volatile CARD8 *)(MGAMMIOBase + (addr)) = (val)
#define OUTREG16(addr, val) *(volatile CARD16 *)(MGAMMIOBase + (addr)) = (val)
#define OUTREG(addr, val) *(volatile CARD32 *)(MGAMMIOBase + (addr)) = (val)
#else /* EXTRADEBUG */
CARD8 dbg_inreg8(int,int);
CARD16 dbg_inreg16(int,int);
CARD32 dbg_inreg32(int,int);
void dbg_outreg8(int,int);
void dbg_outreg16(int,int);
void dbg_outreg32(int,int);
#define INREG8(addr) dbg_inreg8(addr,1)
#define INREG16(addr) dbg_inreg16(addr,1)
#define INREG(addr) dbg_inreg32(addr,1)
#define OUTREG8(addr,val) dbg_outreg8(addr,val)
#define OUTREG16(addr,val) dbg_outreg16(addr,val)
#define OUTREG(addr,val) dbg_outreg32(addr,val)
#endif /* EXTRADEBUG */

#define MGAISBUSY() (INREG8(MGAREG_Status + 2) & 0x01)
#define MGAWAITFIFO() while(INREG16(MGAREG_FIFOSTATUS) & 0x100);
#define MGAWAITFREE() while(MGAISBUSY());
#define MGAWAITFIFOSLOTS(slots) while ( ((INREG16(MGAREG_FIFOSTATUS) & 0x3f) - (slots)) < 0 );

#define MGA_IS_2164(chip) (((chip) == PCI_CHIP_MGA2164) || \
			   ((chip) == PCI_CHIP_MGA2164_AGP))
#define MGA_IS_G100(chip) (((chip) == PCI_CHIP_MGAG100) || \
			   ((chip) == PCI_CHIP_MGAG100_PCI))
#define MGA_IS_G200(chip) (((chip) == PCI_CHIP_MGAG200) || \
			   ((chip) == PCI_CHIP_MGAG200_PCI))
#define MGA_IS_G400(chip) ((chip) == PCI_CHIP_MGAG400)
#define MGA_IS_GCLASS(chip) (MGA_IS_G100(chip) || MGA_IS_G200(chip) \
				|| MGA_IS_G400(chip))

typedef struct {
    Bool	isHwCursor;
    int		CursorMaxWidth;
    int 	CursorMaxHeight;
    int		CursorFlags;
    void	(*LoadCursorImage)();
    void	(*ShowCursor)();
    void	(*HideCursor)();
    void	(*SetCursorPosition)();
    void	(*SetCursorColors)();
    long	maxPixelClock;
    long	MemoryClock;
} MGARamdacRec;

extern MGARamdacRec MGAdac;
extern pciTagRec MGAPciTag;
extern int Chipset;
extern int MGArev;
extern int MGAinterleave;
extern int MGABppShift;
extern int MGAusefbitblt;
extern int MGAydstorg;
extern unsigned char *MGAMMIOBase;
extern Bool MGAHasSDRAM;

extern void Mga8AccelInit();
extern void Mga16AccelInit();
extern void Mga24AccelInit();
extern void Mga32AccelInit();
extern void MGAStormAccelInit();
extern void MGAStormSync();
extern void MGAStormEngineInit();
extern void MGA3026RamdacInit();
extern void MGA1064RamdacInit();
extern void MGA1064ShowCursor();
extern void MGA1064HideCursor();
extern void MGA1064LoadCursorImage();
extern void MGA1064SetCursorPosition();
extern void MGA1064SetCursorColors();

void outMGAdac(unsigned char *MGAMMIOBase, unsigned char reg, unsigned char val);
unsigned char inMGAdac(unsigned char *MGAMMIOBase, unsigned char reg);

#if 0

#define BREG(name) \
    (*(volatile unsigned char *)(MGAMMIOBase + MGAREG_OFFSET + (name)))
#define WREG(name) \
    (*(volatile unsigned short *)(MGAMMIOBase + MGAREG_OFFSET + (name)))

#define BREGO(name, value) \
    (*(volatile unsigned char *)(MGAMMIOBase + MGAREG_OFFSET + (name))) = value
#define WREGO(name, value) \
    (*(volatile unsigned short *)(MGAMMIOBase + MGAREG_OFFSET + (name))) = value

#else

#define BREG(name)  inb (name)
#define WREG(name)  inw (name)

#define BREGO(name, value)  outb (name, value)
#define WREGO(name, value)  outw (name, value)

#endif

#ifdef __alpha__
extern unsigned char *MGAMMIOBaseDENSE;
#endif

/*
 * ROPs
 *
 * for some silly reason, the bits in the ROPs are just the other way round
 */

/*
 * definitions for the new acceleration interface
 */
#define WAITUNTILFINISHED()	MGAWAITFREE()
#define SETBACKGROUNDCOLOR(col)	OUTREG(MGAREG_BCOL, (col))
#define SETFOREGROUNDCOLOR(col)	OUTREG(MGAREG_FCOL, (col))
#define SETRASTEROP(rop)	mga_cmd |= MGARop[rop]
#define SETWRITEPLANEMASK(pm)	OUTREG(MGAREG_PLNWT, (pm))
#define SETBLTXYDIR(x,y)	OUTREG(MGAREG_SGN, ((-x+1)>>1)+4*((-y+1)>>1))

#endif
