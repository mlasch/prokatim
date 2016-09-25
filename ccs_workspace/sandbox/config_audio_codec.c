
#include <csl.h>
#include <csl_irq.h>
#include <csl_mcbsp.h>
#include <csl_edma.h>

#include <dsk6713.h>
#include <dsk6713_aic23.h>

#include <sandboxcfg.h>		// includes all the necessary stuff for dsp bios

#include "process_data.h"
#include "config_audio_codec.h"
#include "globals.h"


/*
 * Helper function to write aic23 registers
 */
static void set_aic23_register(MCBSP_Handle hMcbsp, Uint16 reg, Uint16 val) {
	val &= 0x1ff;
	//LOG_printf(&LOG0, "Transmit data: %d", (reg<<9) | val);
	while(!MCBSP_xrdy(hMcbsp));
	MCBSP_write(hMcbsp, (reg<<9) | val);
	while (MCBSP_xrdy(hMcbsp));
}

/*
 * McBSP0 config interface for aic23
 */
static MCBSP_Config MCBSP0_config = {
	/* Serial Port Control Register (SPCR) */
	MCBSP_SPCR_RMK(
		MCBSP_SPCR_FREE_NO,
		MCBSP_SPCR_SOFT_NO,
		MCBSP_SPCR_FRST_YES,
		MCBSP_SPCR_GRST_YES,
		MCBSP_SPCR_XINTM_XRDY,
		MCBSP_SPCR_XSYNCERR_NO,
		MCBSP_SPCR_XRST_YES,		// Serial port reset state
		MCBSP_SPCR_DLB_OFF,
		MCBSP_SPCR_RJUST_RZF,
		MCBSP_SPCR_CLKSTP_NODELAY,
		MCBSP_SPCR_DXENA_OFF,
		MCBSP_SPCR_RINTM_RRDY,
		MCBSP_SPCR_RSYNCERR_NO,
		MCBSP_SPCR_RRST_YES),

	/* Receive Control Register */
	MCBSP_RCR_DEFAULT,			// Leave RCR in default since we just transmit

	/* Transmit Control Register (TCR) */
	MCBSP_XCR_RMK(
		MCBSP_XCR_XPHASE_SINGLE,
		MCBSP_XCR_XFRLEN2_DEFAULT,
		MCBSP_XCR_XWDLEN1_DEFAULT,
		MCBSP_XCR_XCOMPAND_MSB,
		MCBSP_XCR_XFIG_NO,
		MCBSP_XCR_XDATDLY_1BIT,
		MCBSP_XCR_XFRLEN1_OF(0),
		MCBSP_XCR_XWDLEN1_16BIT,
		MCBSP_XCR_XWDREVRS_DISABLE),

	/* Sample Rate Generator Register */
	MCBSP_SRGR_RMK(
		MCBSP_SRGR_GSYNC_FREE,
		MCBSP_SRGR_CLKSP_RISING,
		MCBSP_SRGR_CLKSM_INTERNAL,
		MCBSP_SRGR_FSGM_DXR2XSR,
		MCBSP_SRGR_FPER_OF(0),
		MCBSP_SRGR_FWID_OF(19),
		MCBSP_SRGR_CLKGDV_OF(99)),

	/* Multichannel Control Register (MCR) */
	MCBSP_MCR_DEFAULT,
	/* Receive Channel Enable Register (RCER) */
	MCBSP_RCER_DEFAULT,
	/* Transmit Channel Enable Register (XCER) */
	MCBSP_XCER_DEFAULT,

	/* Pinout Control Register (PCR) */
	MCBSP_PCR_RMK(
		MCBSP_PCR_XIOEN_SP,
		MCBSP_PCR_RIOEN_SP,
		MCBSP_PCR_FSXM_INTERNAL,
		MCBSP_PCR_FSRM_DEFAULT,
		MCBSP_PCR_CLKXM_OUTPUT,
		MCBSP_PCR_CLKRM_DEFAULT,
		MCBSP_PCR_CLKSSTAT_DEFAULT,
		MCBSP_PCR_DXSTAT_DEFAULT,
		MCBSP_PCR_FSXP_ACTIVELOW,
		MCBSP_PCR_FSRP_DEFAULT,
		MCBSP_PCR_CLKXP_FALLING,
		MCBSP_PCR_CLKRP_DEFAULT)
};

/*
 * McBSP1 data interface for aic23
 */
static MCBSP_Config MCBSP1_config = {
	/* Serial Port Control Register (SPCR) */
	MCBSP_SPCR_RMK(
		MCBSP_SPCR_FREE_NO,
		MCBSP_SPCR_SOFT_NO,
		MCBSP_SPCR_FRST_YES,
		MCBSP_SPCR_GRST_YES,
		MCBSP_SPCR_XINTM_XRDY,
		MCBSP_SPCR_XSYNCERR_NO,
		MCBSP_SPCR_XRST_YES,
		MCBSP_SPCR_DLB_OFF,
		MCBSP_SPCR_RJUST_RZF,
		MCBSP_SPCR_CLKSTP_DISABLE,
		MCBSP_SPCR_DXENA_OFF,
		MCBSP_SPCR_RINTM_RRDY,
		MCBSP_SPCR_RSYNCERR_NO,
		MCBSP_SPCR_RRST_YES),

	/* Receive Control Register (RCR) */
	MCBSP_RCR_RMK(
		MCBSP_RCR_RPHASE_SINGLE,
		MCBSP_RCR_RFRLEN2_DEFAULT,
		MCBSP_RCR_RWDLEN2_DEFAULT,
		MCBSP_RCR_RCOMPAND_MSB,
		MCBSP_RCR_RFIG_NO,
		MCBSP_RCR_RDATDLY_0BIT,
		MCBSP_RCR_RFRLEN1_OF(0),		// receive frame length
		MCBSP_RCR_RWDLEN1_32BIT,		// receive word length
		MCBSP_RCR_RWDREVRS_DISABLE),

	/* Transmit Control Register (TCR) */
	MCBSP_XCR_RMK(
		MCBSP_XCR_XPHASE_SINGLE,
		MCBSP_XCR_XFRLEN2_DEFAULT,
		MCBSP_XCR_XWDLEN2_DEFAULT,
		MCBSP_XCR_XCOMPAND_MSB,
		MCBSP_XCR_XFIG_NO,
		MCBSP_XCR_XDATDLY_0BIT,
		MCBSP_XCR_XFRLEN1_OF(0),		// transmit frame length
		MCBSP_XCR_XWDLEN1_32BIT,		// transmit word length
		MCBSP_XCR_XWDREVRS_DISABLE),

	/* Sample Rate Generator Register */
	MCBSP_SRGR_DEFAULT,
	/* Multichannel Control Register (MCR) */
	MCBSP_MCR_DEFAULT,
	/* Receive Channel Enable Register (RCER) */
	MCBSP_RCER_DEFAULT,
	/* Transmit Channel Enable Register (XCER) */
	MCBSP_XCER_DEFAULT,

	/* Pinout Control Register (PCR) */
	MCBSP_PCR_RMK(
		MCBSP_PCR_XIOEN_SP,
		MCBSP_PCR_RIOEN_SP,
		MCBSP_PCR_FSXM_EXTERNAL,
		MCBSP_PCR_FSRM_EXTERNAL,
		MCBSP_PCR_CLKXM_INPUT,
		MCBSP_PCR_CLKRM_INPUT,
		MCBSP_PCR_CLKSSTAT_DEFAULT,
		MCBSP_PCR_DXSTAT_DEFAULT,
		MCBSP_PCR_FSXP_ACTIVEHIGH,
		MCBSP_PCR_FSRP_ACTIVEHIGH,
		MCBSP_PCR_CLKXP_FALLING,
		MCBSP_PCR_CLKRP_RISING)
};

/*
 * EDMA receive channel config
 */
EDMA_Config gEdmaRcvConfig = {
	EDMA_OPT_RMK(
		EDMA_OPT_PRI_LOW,
		EDMA_OPT_ESIZE_32BIT,
		EDMA_OPT_2DS_NO,
		EDMA_OPT_SUM_NONE,				// no source address update
		EDMA_OPT_2DD_NO,
		EDMA_OPT_DUM_INC,				// increment destination address
		EDMA_OPT_TCINT_YES,				// Source address update mode
		EDMA_OPT_TCC_DEFAULT,
		EDMA_OPT_LINK_YES,
		EDMA_OPT_FS_NO),				// Frame synchronisation, we copy element by element
	EDMA_SRC_OF(0x00000000),			// EDMA Channel Source Address
	EDMA_CNT_RMK(0, 108),				// Frame count (FRMCNT), Element count (FRMCNT)
	EDMA_DST_OF((Uint32) &gRcvBufferA),	// EDMA Channel Destination Address
	EDMA_IDX_RMK(EDMA_IDX_FRMIDX_DEFAULT, EDMA_IDX_ELEIDX_DEFAULT),		// Frame index, Element index
	EDMA_RLD_RMK(0x0000, 0x0000)		// Element count reload, Link address
};

/*
 * EDMA transmit channel config
 */
EDMA_Config gEdmaXmtConfig = {
	EDMA_OPT_RMK(
		EDMA_OPT_PRI_LOW,
		EDMA_OPT_ESIZE_32BIT,
		EDMA_OPT_2DS_NO,
		EDMA_OPT_SUM_INC,				// increment source address
		EDMA_OPT_2DD_NO,
		EDMA_OPT_DUM_NONE,				// don't update destination (TDR)
		EDMA_OPT_TCINT_YES,				// Source address update mode
		EDMA_OPT_TCC_DEFAULT,
		EDMA_OPT_LINK_YES,
		EDMA_OPT_FS_NO),				// Frame synchronisation, we copy element by element
	(Uint32) &gXmtBufferA,			// EDMA Channel Source Address
	EDMA_CNT_RMK(0, 108),				// Frame count (FRMCNT), Element count (FRMCNT)
	EDMA_DST_OF(0x00000000),			// EDMA Channel Destination Address
	EDMA_IDX_RMK(EDMA_IDX_FRMIDX_DEFAULT, EDMA_IDX_ELEIDX_DEFAULT),		// Frame index, Element index
	EDMA_RLD_RMK(0x0000, 0x0000)		// Element count reload, Link address
};

/*
 *
 */
void DSK6713_configure_AIC23() {
	/* Configure McBSP0 as control interface for aic23 */
	MCBSP_Handle MCBSP0_handle;
	MCBSP0_handle = MCBSP_open(MCBSP_DEV0, MCBSP_OPEN_RESET);

	MCBSP_config(MCBSP0_handle, &MCBSP0_config);
	MCBSP_start(MCBSP0_handle, MCBSP_XMIT_START | MCBSP_SRGR_START | MCBSP_SRGR_START, 220);

	set_aic23_register(MCBSP0_handle, RESET_REGISTER, 0x0000);
	set_aic23_register(MCBSP0_handle, POWER_DOWN_CONTROL, 0x0000);
	set_aic23_register(MCBSP0_handle, LEFT_LINE_INPUT_CHANNEL_VOLUME, 0x0017);
	set_aic23_register(MCBSP0_handle, RIGHT_LINE_INPUT_CHANNEL_VOLUME, 0x0017);
	set_aic23_register(MCBSP0_handle, LEFT_CHANNEL_HEADPHONE_VOLUME, 0x00f9);
	set_aic23_register(MCBSP0_handle, RIGHT_CHANNEL_HEADPHONE_VOLUME, 0x00f9);
	set_aic23_register(MCBSP0_handle, ANALOG_AUDIO_PATH, 0x0011);			// 00001 0010
	set_aic23_register(MCBSP0_handle, DIGITAL_AUDIO_PATH, 0x0000);			// 000000000
	set_aic23_register(MCBSP0_handle, DIGITAL_AUDIO_INTERFACE_FORMAT, 0x0043);	//0 0100 0001
	set_aic23_register(MCBSP0_handle, SAMPLE_RATE_CONTROL, 0x000d);		// 01100
	set_aic23_register(MCBSP0_handle, DIGITAL_INTERFACE_ACTIVATION, 0x0001);

	/* Configure McBSP1 as data interface for aic23 */
	MCBSP_Handle MCBSP1_handle;
	MCBSP1_handle = MCBSP_open(MCBSP_DEV1, MCBSP_OPEN_RESET);
	MCBSP_config(MCBSP1_handle, &MCBSP1_config);
	MCBSP_start(MCBSP1_handle, MCBSP_XMIT_START|MCBSP_RCV_START|MCBSP_SRGR_FRAMESYNC|MCBSP_SRGR_START, 220);

	/* Configure receive EDMA */
	EDMA_Handle hEdmaRcv;
	EDMA_Handle hEdmaRcvA;
	EDMA_Handle hEdmaRcvB;

	hEdmaRcv = EDMA_open(EDMA_CHA_REVT1, EDMA_OPEN_RESET);
	hEdmaRcvA = EDMA_allocTable(-1);
	hEdmaRcvB = EDMA_allocTable(-1);

	gEdmaRcvConfig.src = MCBSP_getRcvAddr(MCBSP1_handle);		// Get address of DRR
	gTccRcvChan = EDMA_intAlloc(-1);							// get next free transfer complete code
	gEdmaRcvConfig.opt |= EDMA_FMK(OPT, TCC, gTccRcvChan);
	EDMA_config(hEdmaRcv, &gEdmaRcvConfig);
	EDMA_config(hEdmaRcvA, &gEdmaRcvConfig);
	gEdmaRcvConfig.dst = EDMA_DST_OF(gRcvBufferB);
	EDMA_config(hEdmaRcvB, &gEdmaRcvConfig);

	EDMA_link(hEdmaRcv, hEdmaRcvB);
	EDMA_link(hEdmaRcvB, hEdmaRcvA);
	EDMA_link(hEdmaRcvA, hEdmaRcvB);

	/* Configure transmit EDMA */
	EDMA_Handle hEdmaXmt;
	EDMA_Handle hEdmaXmtA;
	EDMA_Handle hEdmaXmtB;

	hEdmaXmt = EDMA_open(EDMA_CHA_XEVT1, EDMA_OPEN_RESET);
	hEdmaXmtA = EDMA_allocTable(-1);
	hEdmaXmtB = EDMA_allocTable(-1);

	gEdmaXmtConfig.dst = MCBSP_getXmtAddr(MCBSP1_handle);		// Get address of DXR
	gTccXmtChan = EDMA_intAlloc(-1);							// get next free transfer complete code
	gEdmaXmtConfig.opt |= EDMA_FMK(OPT, TCC, gTccXmtChan);
	EDMA_config(hEdmaXmt, &gEdmaXmtConfig);
	EDMA_config(hEdmaXmtA, &gEdmaXmtConfig);
	gEdmaXmtConfig.src = EDMA_DST_OF(gXmtBufferB);				// set source to buffer B
	EDMA_config(hEdmaXmtB, &gEdmaXmtConfig);

	EDMA_link(hEdmaXmt, hEdmaXmtB);
	EDMA_link(hEdmaXmtB, hEdmaXmtA);
	EDMA_link(hEdmaXmtA, hEdmaXmtB);


	EDMA_intClear(gTccRcvChan);
	EDMA_intClear(gTccXmtChan);
	EDMA_intEnable(gTccRcvChan);
	EDMA_intEnable(gTccXmtChan);

	gBufferState.cpuBufferState = StateB;		// inittial cpu buffer state

	EDMA_enableChannel(hEdmaRcv);
	EDMA_enableChannel(hEdmaXmt);

	IRQ_clear(IRQ_EVT_EDMAINT);
	IRQ_enable(IRQ_EVT_EDMAINT);

	MCBSP_write(MCBSP1_handle, 0x00);
}
