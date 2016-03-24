#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_edma.h>

extern Int32 buffer_IN_A;

/*
 * Helper function to write aic23 registers
 */
static void set_aic23_register(MCBSP_Handle MCBSP_handle, Uint16 reg, Uint16 val) {
	val &= 0x1ff;
	while(!MCBSP_xrdy(MCBSP_handle));
	MCBSP_write(MCBSP_handle, (reg<<9) | val);
}

/*
 * aic23 register map
 */
enum aic23_register_map {
	LEFT_LINE_INPUT_CHANNEL_VOLUME,
	RIGHT_LINE_INPUT_CHANNEL_VOLUME,
	LEFT_CHANNEL_HEADPHONE_VOLUME,
	RIGHT_CHANNEL_HEADPHONE_VOLUME,
	ANALOG_AUDIO_PATH,
	DIGITAL_AUDIO_PATH,
	POWER_DOWN_CONTROL,
	DIGITAL_AUDIO_INTERFACE_FORMAT,
	SAMPLE_RATE_CONTROL,
	DIGITAL_INTERFACE_ACTIVATION,
	RESET_REGISTER
}

/*
 * McBSP0 config interface for aic23
 */
MCBSP_Config MCBSP0_config = {
	/* Serial Port Control Register (SPCR) */
	MCBSP_SPCR_RMK(
		MCBSP_SPCR_FREE_NO,
		MCBSP_SPCR_SOFT_NO,
		MCBSP_SPCR_FRST_YES,
		MCBSP_SPCR_GRST_YES,
		MCBSP_SPCR_XINTM_XRDY,
		MCBSP_SPCR_XSYNCERR_NO,
		MCBSP_SPCR_XRST_YES,		// Serial port transmitter is enabled
		MCBSP_SPCR_DLB_OFF,
		MCBSP_SPCR_RJUST_RZF,
		MCBSP_SPCR_CLKSTP_NODELAY,
		MCBSP_SPCR_DXENA_OFF,
		MCBSP_SPCR_RINTM_RRDY,
		MCBSP_SPCR_RYSYNCRR_NO,
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
MCBSP_Config MCBSP1_config = {
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
		MCBSP_SPCR_RYSYNCRR_NO,
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
		MCBSP_XCR_XWDLEN1_DEFAULT,
		MCBSP_XCR_XCOMPAND_MSB,
		MCBSP_XCR_XFIG_NO,
		MCBSP_XCR_XDATDLY_0BIT,
		MCBSP_XCR_XFRLEN1_OF(1),		// transmit frame length
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
 * EDMA RX Channel config
 */
EDMA_Config EDMA_RX_config = {
	EDMA_OPT_RMK(
		EDMA_OPT_PRI_LOW,
		EDMA_OPT_ESIZE_32BIT,
		EDMA_OPT_2DS_NO,
		EDMA_OPT_SUM_NONE,			// no source address update
		EDMA_OPT_2DD_NO,
		EDMA_OPT_DUM_INC,			// increment destination address
		EDMA_OPT_TCINT_YES,			// Source address update mode
		EDMA_OPT_TCC_DEFAULT,
		EDMA_OPT_LINK_DEFAULT,
		EDMA_OPT_FS_NO),			// Frame synchronisation
	EDMA_SRC_OF(0x00000000),			// EDMA Channel Source Address
	EDMA_CNT_RMK(0, 108),				// Frame count (FRMCNT), Element count (FRMCNT)
	EDMA_DST_OF(buffer_IN_A),			// EDMA Channel Destination Address
	EDMA_IDX_RMK(0x0000, 0x0004),		// Frame index, Element index
	EDMA_RLD_RMK(0x0000, 0x0000)		// Element count reload, Link address
};

/*
 *
 */
void DSK6713_configure_AIC23() {
	Uint32 receiveAddr;

	/* Configure McBSP0 as control interface for aic23 */
	MCBSP_Handle MCBSP0_handle;
	MCBSP0_handle = MCBSP_open(MCBSP0_handle, MCBSP_OPEN_RESET);
	MCBSP_config(MCBSP0_handle, &MCBSP0_config);
	MCBSP_start(MCBSP0_handle, MCBSP_XMIT_START, 0xffffffff);

	set_aic23_register(MCBSP0_handle, RESET_REGISTER, 0x0000);
	set_aic23_register(MCBSP0_handle, POWER_DOWN_CONTROL, 0x0000);

	set_aic23_register(MCBSP0_handle, LEFT_LINE_INPUT_CHANNEL_VOLUME, 0x0017);
	set_aic23_register(MCBSP0_handle, RIGHT_LINE_INPUT_CHANNEL_VOLUME, 0x0017);
	set_aic23_register(MCBSP0_handle, LEFT_CHANNEL_HEADPHONE_VOLUME, 0x00f9);
	set_aic23_register(MCBSP0_handle, RIGHT_CHANNEL_HEADPHONE_VOLUME, 0x00f9);
	set_aic23_register(MCBSP0_handle, ANALOG_AUDIO_PATH, 0x0012);
	set_aic23_register(MCBSP0_handle, DIGITAL_AUDIO_PATH, 0x0000);
	set_aic23_register(MCBSP0_handle, DIGITAL_AUDIO_INTERFACE_FORMAT, 0x0041);
	set_aic23_register(MCBSP0_handle, SAMPLE_RATE_CONTROL, 0x000d);
	set_aic23_register(MCBSP0_handle, DIGITAL_INTERFACE_ACTIVATION, 0x0001);

	/* Configure McBSP1 as data interface for aic23 */
	MCBSP_Handle MCBSP1_handle;
	MCBSP1_handle MCBSP_open(MCBSP1_handle, MCBSP_OPEN_RESET);
	MCBSP_config(MCBSP1_handle, &MCBSP1_config);
	MCBSP_start(MCBSP1_handle, MCBSP_XMIT_START, 0xffffffff);
	receiveAddr = MCBSP_getRcvAddr(MCBSP1_handle);		// Get address of DRR

	/* Configure EDMA */
	EDMA_Handle EDMA_RX_handle;
	EDMA_handle = EDMA_open(EDMA_RX_handle, EDMA_CHA_REVT1, EDMA_OPEN_RESET);
	EDMA_RX_config.src = receiveAddr;
	EDMA_config(EDMA_RX_handle, &EDMA_RX_config);
	EDMA_enableChannel(EDMA_RX_handle);
	EDMA_intEnable();
}
