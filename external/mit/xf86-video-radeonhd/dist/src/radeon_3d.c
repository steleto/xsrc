/* BROKEN DISCLAIMER */

/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef IS_QUICK_AND_DIRTY /* make this IS_RADEON_DRIVER */
/*
 * Radeon driver specifics.
 */
#if defined(ACCEL_MMIO) && defined(ACCEL_CP)
#error Cannot define both MMIO and CP acceleration!
#endif

#if !defined(UNIXCPP) || defined(ANSICPP)
#define FUNC_NAME_CAT(prefix,suffix) prefix##suffix
#else
#define FUNC_NAME_CAT(prefix,suffix) prefix/**/suffix
#endif

#ifdef ACCEL_MMIO
#define FUNC_NAME(prefix) FUNC_NAME_CAT(prefix,MMIO)
#else
#ifdef ACCEL_CP
#define FUNC_NAME(prefix) FUNC_NAME_CAT(prefix,CP)
#else
#error No accel type defined!
#endif
#endif

#ifdef ACCEL_MMIO
#define ONLY_ONCE
#else
#undef ONLY_ONCE
#endif

#ifdef ONLY_ONCE

# define VAR_PREAMBLE() RHDPtr info = RHDPTR(pScrn)
# define THREEDSTATE_PREAMBLE() struct rhdAccel *accel_state = info->accel_state

# define HAS_TCL info->has_tcl

# define R5XXPowerPipes(p) {}

/* Map the number of GB Pipes the hardware has. */
static int
R5xxGBPipesCount(ScrnInfoPtr pScrn)
{
    return ((RHDRegRead(pScrn, R400_GB_PIPE_SELECT) >> 12) & 0x03) + 1;
}
#define NUM_GB_PIPES R5xxGBPipesCount(pScrn)

/* Map the number of FPUs the VPS has. */
static int
R5xxPVSFPUCount(ScrnInfoPtr pScrn)
{
     switch (RHDPTR(pScrn)->ChipSet) {
     case RHD_RV515:
     case RHD_RV516:
     case RHD_RV550:
     case RHD_M52:
     case RHD_M54:
     case RHD_M62:
     case RHD_M64:
	 return 2;
     case RHD_RV530:
     case RHD_RV560:
     case RHD_RV570:
     case RHD_M56:
     case RHD_M58:
     case RHD_M66:
	 return 5;
     case RHD_R520:
     case RHD_R580:
     case RHD_M68:
	 return 8;
     default:
	 return 4;
     }
}
#define NUM_PVS_FPUS R5xxPVSFPUCount(pScrn)

# define END_ACCEL()

#endif /* ONLY_ONCE */

#else /* IS_RADEON_DRIVER */
/*
 * RadeonHD driver specifics.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include "rhd.h"
#include "rhd_cs.h"

#include "r5xx_accel.h"
#include "r5xx_3dregs.h"

#ifdef USE_DRI
/* for claiming the context */
#include "rhd_dri.h"
#endif

#define VAR_PREAMBLE() RHDPtr rhdPtr = RHDPTR(pScrn)
#define THREEDSTATE_PREAMBLE() struct R5xx3D *accel_state = rhdPtr->ThreeDPrivate

/*
 * Map the macros.
 */
#define ACCEL_PREAMBLE() struct RhdCS *CS = rhdPtr->CS

#define BEGIN_ACCEL(Count) RHDCSGrab(CS, 2 * (Count))
#define OUT_ACCEL_REG(Reg, Value) RHDCSRegWrite(CS, (Reg), (Value))
#define FINISH_ACCEL()

#ifdef USE_DRI
#define END_ACCEL() RHDCSAdvance(CS); \
                    if (pScrn->pScreen) \
                        RHDDRIContextClaim(pScrn)
#else
#define END_ACCEL() RHDCSAdvance(CS)
#endif

#define uint32_t CARD32

#define IS_R300_3D \
    ((rhdPtr->ChipSet == RHD_RS690) || \
     (rhdPtr->ChipSet == RHD_RS600) || \
     (rhdPtr->ChipSet == RHD_RS740))

#define IS_R500_3D \
    ((rhdPtr->ChipSet != RHD_RS690) && \
     (rhdPtr->ChipSet != RHD_RS600) && \
     (rhdPtr->ChipSet != RHD_RS740))

#define HAS_TCL IS_R500_3D

/* Map the number of GB Pipes the hardware has. */
static int
R5xxGBPipesCount(ScrnInfoPtr pScrn)
{
    return ((RHDRegRead(pScrn, R400_GB_PIPE_SELECT) >> 12) & 0x03) + 1;
}
#define NUM_GB_PIPES R5xxGBPipesCount(pScrn)

/* Map the number of FPUs the VPS has. */
static int
R5xxPVSFPUCount(ScrnInfoPtr pScrn)
{
     switch (RHDPTR(pScrn)->ChipSet) {
     case RHD_RV515:
     case RHD_RV516:
     case RHD_RV550:
     case RHD_M52:
     case RHD_M54:
     case RHD_M62:
     case RHD_M64:
	 return 2;
     case RHD_RV530:
     case RHD_RV560:
     case RHD_RV570:
     case RHD_M56:
     case RHD_M58:
     case RHD_M66:
	 return 5;
     case RHD_R520:
     case RHD_R580:
     case RHD_M68:
	 return 8;
     default:
	 return 4;
     }
}
#define NUM_PVS_FPUS R5xxPVSFPUCount(pScrn)

/*
 *
 */
static void
R5XXPowerPipes(ScrnInfoPtr pScrn)
{
    CARD32 tmp = RHDRegRead(pScrn, R400_GB_PIPE_SELECT);
    RHDWritePLL(pScrn, R500_DYN_SCLK_PWMEM_PIPE, (1 | ((tmp >> 8) & 0xf) << 4));
}
/* for radeon, this is done elsewhere, so use:
 * #define R5XXPowerPipes(x)
 */

#endif /* IS_RADEON_DRIVER */

#if defined(IS_RADEON_DRIVER) || defined(IS_QUICK_AND_DIRTY)
static void
FUNC_NAME(RADEONInit3DEngine)(int scrnIndex)
#else
void
R5xx3DSetup(int scrnIndex)
#endif
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    VAR_PREAMBLE();
    THREEDSTATE_PREAMBLE();
    uint32_t gb_tile_config, su_reg_dest, vap_cntl;
    int num_gb_pipes = NUM_GB_PIPES;
    int num_pvs_fpus = NUM_PVS_FPUS;
    Bool HasTCL = HAS_TCL;
    ACCEL_PREAMBLE();

    accel_state->texW[0] = 1;
    accel_state->texH[0] = 1;
    accel_state->texW[1] = 1;
    accel_state->texH[1] = 1;

#ifdef IS_RADEON_DRIVER
    if (IS_R300_3D || IS_R500_3D) {
#endif
	BEGIN_ACCEL(3);
	OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D | R300_DC_FREE_3D);
	OUT_ACCEL_REG(R300_RB3D_ZCACHE_CTLSTAT, R300_ZC_FLUSH | R300_ZC_FREE);
	OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_2D_IDLECLEAN | RADEON_WAIT_3D_IDLECLEAN);
	FINISH_ACCEL();

	if (IS_R500_3D)
	    R5XXPowerPipes(pScrn);

	gb_tile_config = (R300_ENABLE_TILING | R300_TILE_SIZE_16 | R300_SUBPIXEL_1_16);

	switch(num_gb_pipes) {
	case 2: gb_tile_config |= R300_PIPE_COUNT_R300; break;
	case 3: gb_tile_config |= R300_PIPE_COUNT_R420_3P; break;
	case 4: gb_tile_config |= R300_PIPE_COUNT_R420; break;
	default:
	case 1: gb_tile_config |= R300_PIPE_COUNT_RV350; break;
	}

	BEGIN_ACCEL(5);
	OUT_ACCEL_REG(R300_GB_TILE_CONFIG, gb_tile_config);
	OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_2D_IDLECLEAN | RADEON_WAIT_3D_IDLECLEAN);
	OUT_ACCEL_REG(R300_DST_PIPE_CONFIG, R300_PIPE_AUTO_CONFIG);
	OUT_ACCEL_REG(R300_GB_SELECT, 0);
	OUT_ACCEL_REG(R300_GB_ENABLE, 0);
	FINISH_ACCEL();

	if (IS_R500_3D) {
	    su_reg_dest = ((1 << num_gb_pipes) - 1);
	    BEGIN_ACCEL(2);
	    OUT_ACCEL_REG(R500_SU_REG_DEST, su_reg_dest);
	    OUT_ACCEL_REG(R500_VAP_INDEX_OFFSET, 0);
	    FINISH_ACCEL();
	}

	BEGIN_ACCEL(3);
	OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D | R300_DC_FREE_3D);
	OUT_ACCEL_REG(R300_RB3D_ZCACHE_CTLSTAT, R300_ZC_FLUSH | R300_ZC_FREE);
	OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_2D_IDLECLEAN | RADEON_WAIT_3D_IDLECLEAN);
	FINISH_ACCEL();

	BEGIN_ACCEL(5);
	OUT_ACCEL_REG(R300_GB_AA_CONFIG, 0);
	OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D | R300_DC_FREE_3D);
	OUT_ACCEL_REG(R300_RB3D_ZCACHE_CTLSTAT, R300_ZC_FLUSH | R300_ZC_FREE);
	OUT_ACCEL_REG(R300_GB_MSPOS0, ((8 << R300_MS_X0_SHIFT) |
				       (8 << R300_MS_Y0_SHIFT) |
				       (8 << R300_MS_X1_SHIFT) |
				       (8 << R300_MS_Y1_SHIFT) |
				       (8 << R300_MS_X2_SHIFT) |
				       (8 << R300_MS_Y2_SHIFT) |
				       (8 << R300_MSBD0_Y_SHIFT) |
				       (7 << R300_MSBD0_X_SHIFT)));
	OUT_ACCEL_REG(R300_GB_MSPOS1, ((8 << R300_MS_X3_SHIFT) |
				       (8 << R300_MS_Y3_SHIFT) |
				       (8 << R300_MS_X4_SHIFT) |
				       (8 << R300_MS_Y4_SHIFT) |
				       (8 << R300_MS_X5_SHIFT) |
				       (8 << R300_MS_Y5_SHIFT) |
				       (8 << R300_MSBD1_SHIFT)));
	FINISH_ACCEL();

	BEGIN_ACCEL(5);
	OUT_ACCEL_REG(R300_GA_ENHANCE, R300_GA_DEADLOCK_CNTL | R300_GA_FASTSYNC_CNTL);
	OUT_ACCEL_REG(R300_GA_POLY_MODE, R300_FRONT_PTYPE_TRIANGE | R300_BACK_PTYPE_TRIANGE);
	OUT_ACCEL_REG(R300_GA_ROUND_MODE, (R300_GEOMETRY_ROUND_NEAREST |
					   R300_COLOR_ROUND_NEAREST));
	OUT_ACCEL_REG(R300_GA_COLOR_CONTROL, (R300_RGB0_SHADING_GOURAUD |
					      R300_ALPHA0_SHADING_GOURAUD |
					      R300_RGB1_SHADING_GOURAUD |
					      R300_ALPHA1_SHADING_GOURAUD |
					      R300_RGB2_SHADING_GOURAUD |
					      R300_ALPHA2_SHADING_GOURAUD |
					      R300_RGB3_SHADING_GOURAUD |
					      R300_ALPHA3_SHADING_GOURAUD));
	OUT_ACCEL_REG(R300_GA_OFFSET, 0);
	FINISH_ACCEL();

	BEGIN_ACCEL(5);
	OUT_ACCEL_REG(R300_SU_TEX_WRAP, 0);
	OUT_ACCEL_REG(R300_SU_POLY_OFFSET_ENABLE, 0);
	OUT_ACCEL_REG(R300_SU_CULL_MODE, R300_FACE_NEG);
	OUT_ACCEL_REG(R300_SU_DEPTH_SCALE, 0x4b7fffff);
	OUT_ACCEL_REG(R300_SU_DEPTH_OFFSET, 0);
	FINISH_ACCEL();

	/* setup the VAP */
	if (HasTCL)
	    vap_cntl = ((5 << R300_PVS_NUM_SLOTS_SHIFT) |
			(5 << R300_PVS_NUM_CNTLRS_SHIFT) |
			(9 << R300_VF_MAX_VTX_NUM_SHIFT) |
			(num_pvs_fpus << R300_PVS_NUM_FPUS_SHIFT));
	else
	    vap_cntl = ((10 << R300_PVS_NUM_SLOTS_SHIFT) |
			(5 << R300_PVS_NUM_CNTLRS_SHIFT) |
			(5 << R300_VF_MAX_VTX_NUM_SHIFT) |
			(num_pvs_fpus << R300_PVS_NUM_FPUS_SHIFT));

	if (HasTCL)
	    BEGIN_ACCEL(15);
	else
	    BEGIN_ACCEL(9);
	OUT_ACCEL_REG(R300_VAP_VTX_STATE_CNTL, 0);
	OUT_ACCEL_REG(R300_VAP_PVS_STATE_FLUSH_REG, 0);

	if (HasTCL)
	    OUT_ACCEL_REG(R300_VAP_CNTL_STATUS, 0);
	else
	    OUT_ACCEL_REG(R300_VAP_CNTL_STATUS, R300_PVS_BYPASS);
	OUT_ACCEL_REG(R300_VAP_CNTL, vap_cntl);
	OUT_ACCEL_REG(R300_VAP_PVS_STATE_FLUSH_REG, 0);
	OUT_ACCEL_REG(R300_VAP_VTE_CNTL, R300_VTX_XY_FMT | R300_VTX_Z_FMT);
	OUT_ACCEL_REG(R300_VAP_PSC_SGN_NORM_CNTL, 0);

	OUT_ACCEL_REG(R300_VAP_PROG_STREAM_CNTL_EXT_0,
		      ((R300_SWIZZLE_SELECT_X << R300_SWIZZLE_SELECT_X_0_SHIFT) |
		       (R300_SWIZZLE_SELECT_Y << R300_SWIZZLE_SELECT_Y_0_SHIFT) |
		       (R300_SWIZZLE_SELECT_Z << R300_SWIZZLE_SELECT_Z_0_SHIFT) |
		       (R300_SWIZZLE_SELECT_W << R300_SWIZZLE_SELECT_W_0_SHIFT) |
		       ((R300_WRITE_ENA_X | R300_WRITE_ENA_Y | R300_WRITE_ENA_Z | R300_WRITE_ENA_W)
			<< R300_WRITE_ENA_0_SHIFT) |
		       (R300_SWIZZLE_SELECT_X << R300_SWIZZLE_SELECT_X_1_SHIFT) |
		       (R300_SWIZZLE_SELECT_Y << R300_SWIZZLE_SELECT_Y_1_SHIFT) |
		       (R300_SWIZZLE_SELECT_Z << R300_SWIZZLE_SELECT_Z_1_SHIFT) |
		       (R300_SWIZZLE_SELECT_W << R300_SWIZZLE_SELECT_W_1_SHIFT) |
		       ((R300_WRITE_ENA_X | R300_WRITE_ENA_Y | R300_WRITE_ENA_Z | R300_WRITE_ENA_W)
			<< R300_WRITE_ENA_1_SHIFT)));
	OUT_ACCEL_REG(R300_VAP_PROG_STREAM_CNTL_EXT_1,
		      ((R300_SWIZZLE_SELECT_X << R300_SWIZZLE_SELECT_X_2_SHIFT) |
		       (R300_SWIZZLE_SELECT_Y << R300_SWIZZLE_SELECT_Y_2_SHIFT) |
		       (R300_SWIZZLE_SELECT_Z << R300_SWIZZLE_SELECT_Z_2_SHIFT) |
		       (R300_SWIZZLE_SELECT_W << R300_SWIZZLE_SELECT_W_2_SHIFT) |
		       ((R300_WRITE_ENA_X | R300_WRITE_ENA_Y | R300_WRITE_ENA_Z | R300_WRITE_ENA_W)
			<< R300_WRITE_ENA_2_SHIFT)));

	if (HasTCL) {
	    OUT_ACCEL_REG(R300_VAP_PVS_FLOW_CNTL_OPC, 0);
	    OUT_ACCEL_REG(R300_VAP_GB_VERT_CLIP_ADJ, 0x3f800000);
	    OUT_ACCEL_REG(R300_VAP_GB_VERT_DISC_ADJ, 0x3f800000);
	    OUT_ACCEL_REG(R300_VAP_GB_HORZ_CLIP_ADJ, 0x3f800000);
	    OUT_ACCEL_REG(R300_VAP_GB_HORZ_DISC_ADJ, 0x3f800000);
	    OUT_ACCEL_REG(R300_VAP_CLIP_CNTL, R300_CLIP_DISABLE);
	}
	FINISH_ACCEL();

	/* pre-load the vertex shaders */
	if (HasTCL) {
	    /* exa mask shader program */
	    BEGIN_ACCEL(13);
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_INDX_REG, 0);
	    /* PVS inst 0 */
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_ADD) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(0) |
			   R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
			   R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_W)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    /* PVS inst 1 */
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_ADD) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(1) |
			   R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
			   R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_W)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    /* PVS inst 2 */
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_ADD) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(2) |
			   R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
			   R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(7) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_W)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(7) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(7) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    FINISH_ACCEL();

	    BEGIN_ACCEL(9);
	    /* exa no mask instruction */
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_INDX_REG, 3);
	    /* PVS inst 0 */
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_ADD) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(0) |
			   R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
			   R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_W)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    /* PVS inst 1 */
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_ADD) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(1) |
			   R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
			   R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_W)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    FINISH_ACCEL();

	    /* Xv shader program */
	    BEGIN_ACCEL(9);
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_INDX_REG, 5);

	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_ADD) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(0) |
			   R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
			   R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_W)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_ADD) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(1) |
			   R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
			   R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_W)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_ACCEL_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    FINISH_ACCEL();
	}

	/* pre-load the RS instructions */
	BEGIN_ACCEL(4);
	if (IS_R300_3D) {
	    /* rasterizer source table
	     * R300_RS_TEX_PTR is the offset into the input RS stream
	     * 0,1 are tex0
	     * 2,3 are tex1
	     */
	    OUT_ACCEL_REG(R300_RS_IP_0,
			  (R300_RS_TEX_PTR(0) |
			   R300_RS_SEL_S(R300_RS_SEL_C0) |
			   R300_RS_SEL_T(R300_RS_SEL_C1) |
			   R300_RS_SEL_R(R300_RS_SEL_K0) |
			   R300_RS_SEL_Q(R300_RS_SEL_K1)));
	    OUT_ACCEL_REG(R300_RS_IP_1,
			  (R300_RS_TEX_PTR(2) |
			   R300_RS_SEL_S(R300_RS_SEL_C0) |
			   R300_RS_SEL_T(R300_RS_SEL_C1) |
			   R300_RS_SEL_R(R300_RS_SEL_K0) |
			   R300_RS_SEL_Q(R300_RS_SEL_K1)));
	    /* src tex */
	    /* R300_INST_TEX_ID - select the RS source table entry
	     * R300_INST_TEX_ADDR - the FS temp register for the texture data
	     */
	    OUT_ACCEL_REG(R300_RS_INST_0, (R300_INST_TEX_ID(0) |
					   R300_RS_INST_TEX_CN_WRITE |
					   R300_INST_TEX_ADDR(0)));
	    /* mask tex */
	    OUT_ACCEL_REG(R300_RS_INST_1, (R300_INST_TEX_ID(1) |
					   R300_RS_INST_TEX_CN_WRITE |
					   R300_INST_TEX_ADDR(1)));

	} else {
	    /* rasterizer source table
	     * R300_RS_TEX_PTR is the offset into the input RS stream
	     * 0,1 are tex0
	     * 2,3 are tex1
	     */
	    OUT_ACCEL_REG(R500_RS_IP_0, ((0 << R500_RS_IP_TEX_PTR_S_SHIFT) |
					 (1 << R500_RS_IP_TEX_PTR_T_SHIFT) |
					 (R500_RS_IP_PTR_K0 << R500_RS_IP_TEX_PTR_R_SHIFT) |
					 (R500_RS_IP_PTR_K1 << R500_RS_IP_TEX_PTR_Q_SHIFT)));

	    OUT_ACCEL_REG(R500_RS_IP_1, ((2 << R500_RS_IP_TEX_PTR_S_SHIFT) |
					 (3 << R500_RS_IP_TEX_PTR_T_SHIFT) |
					 (R500_RS_IP_PTR_K0 << R500_RS_IP_TEX_PTR_R_SHIFT) |
					 (R500_RS_IP_PTR_K1 << R500_RS_IP_TEX_PTR_Q_SHIFT)));
	    /* src tex */
	    /* R500_RS_INST_TEX_ID_SHIFT - select the RS source table entry
	     * R500_RS_INST_TEX_ADDR_SHIFT - the FS temp register for the texture data
	     */
	    OUT_ACCEL_REG(R500_RS_INST_0, ((0 << R500_RS_INST_TEX_ID_SHIFT) |
					   R500_RS_INST_TEX_CN_WRITE |
					   (0 << R500_RS_INST_TEX_ADDR_SHIFT)));
	    /* mask tex */
	    OUT_ACCEL_REG(R500_RS_INST_1, ((1 << R500_RS_INST_TEX_ID_SHIFT) |
					   R500_RS_INST_TEX_CN_WRITE |
					   (1 << R500_RS_INST_TEX_ADDR_SHIFT)));
	}
	FINISH_ACCEL();

	/* pre-load FS tex instructions */
	if (IS_R300_3D) {
	    BEGIN_ACCEL(2);
	    /* tex inst for src texture */
	    OUT_ACCEL_REG(R300_US_TEX_INST_0,
			  (R300_TEX_SRC_ADDR(0) |
			   R300_TEX_DST_ADDR(0) |
			   R300_TEX_ID(0) |
			   R300_TEX_INST(R300_TEX_INST_LD)));

	    /* tex inst for mask texture */
	    OUT_ACCEL_REG(R300_US_TEX_INST_1,
			  (R300_TEX_SRC_ADDR(1) |
			   R300_TEX_DST_ADDR(1) |
			   R300_TEX_ID(1) |
			   R300_TEX_INST(R300_TEX_INST_LD)));
	    FINISH_ACCEL();
	}

	if (IS_R300_3D) {
	    BEGIN_ACCEL(9);
	    OUT_ACCEL_REG(R300_US_CONFIG, (0 << R300_NLEVEL_SHIFT) | R300_FIRST_TEX);
	    OUT_ACCEL_REG(R300_US_PIXSIZE, 1); /* highest temp used */
	    OUT_ACCEL_REG(R300_US_CODE_ADDR_0,
			  (R300_ALU_START(0) |
			   R300_ALU_SIZE(0) |
			   R300_TEX_START(0) |
			   R300_TEX_SIZE(0)));
	    OUT_ACCEL_REG(R300_US_CODE_ADDR_1,
			  (R300_ALU_START(0) |
			   R300_ALU_SIZE(0) |
			   R300_TEX_START(0) |
			   R300_TEX_SIZE(0)));
	    OUT_ACCEL_REG(R300_US_CODE_ADDR_2,
			  (R300_ALU_START(0) |
			   R300_ALU_SIZE(0) |
			   R300_TEX_START(0) |
			   R300_TEX_SIZE(0)));
	} else {
	    BEGIN_ACCEL(7);
	    OUT_ACCEL_REG(R300_US_CONFIG, R500_ZERO_TIMES_ANYTHING_EQUALS_ZERO);
	    OUT_ACCEL_REG(R300_US_PIXSIZE, 1); /* highest temp used */
	    OUT_ACCEL_REG(R500_US_FC_CTRL, 0);
	}
	OUT_ACCEL_REG(R300_US_W_FMT, 0);
	OUT_ACCEL_REG(R300_US_OUT_FMT_1, (R300_OUT_FMT_UNUSED |
					  R300_OUT_FMT_C0_SEL_BLUE |
					  R300_OUT_FMT_C1_SEL_GREEN |
					  R300_OUT_FMT_C2_SEL_RED |
					  R300_OUT_FMT_C3_SEL_ALPHA));
	OUT_ACCEL_REG(R300_US_OUT_FMT_2, (R300_OUT_FMT_UNUSED |
					  R300_OUT_FMT_C0_SEL_BLUE |
					  R300_OUT_FMT_C1_SEL_GREEN |
					  R300_OUT_FMT_C2_SEL_RED |
					  R300_OUT_FMT_C3_SEL_ALPHA));
	OUT_ACCEL_REG(R300_US_OUT_FMT_3, (R300_OUT_FMT_UNUSED |
					  R300_OUT_FMT_C0_SEL_BLUE |
					  R300_OUT_FMT_C1_SEL_GREEN |
					  R300_OUT_FMT_C2_SEL_RED |
					  R300_OUT_FMT_C3_SEL_ALPHA));
	FINISH_ACCEL();


	BEGIN_ACCEL(3);
	OUT_ACCEL_REG(R300_FG_DEPTH_SRC, 0);
	OUT_ACCEL_REG(R300_FG_FOG_BLEND, 0);
	OUT_ACCEL_REG(R300_FG_ALPHA_FUNC, 0);
	FINISH_ACCEL();

	BEGIN_ACCEL(13);
	OUT_ACCEL_REG(R300_RB3D_ABLENDCNTL, 0);
	OUT_ACCEL_REG(R300_RB3D_ZSTENCILCNTL, 0);
	OUT_ACCEL_REG(R300_RB3D_ZCACHE_CTLSTAT, R300_ZC_FLUSH | R300_ZC_FREE);
	OUT_ACCEL_REG(R300_RB3D_BW_CNTL, 0);
	OUT_ACCEL_REG(R300_RB3D_ZCNTL, 0);
	OUT_ACCEL_REG(R300_RB3D_ZTOP, 0);
	OUT_ACCEL_REG(R300_RB3D_ROPCNTL, 0);

	OUT_ACCEL_REG(R300_RB3D_AARESOLVE_CTL, 0);
	OUT_ACCEL_REG(R300_RB3D_COLOR_CHANNEL_MASK, (R300_BLUE_MASK_EN |
						     R300_GREEN_MASK_EN |
						     R300_RED_MASK_EN |
						     R300_ALPHA_MASK_EN));
	OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D | R300_DC_FREE_3D);
	OUT_ACCEL_REG(R300_RB3D_CCTL, 0);
	OUT_ACCEL_REG(R300_RB3D_DITHER_CTL, 0);
	OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D | R300_DC_FREE_3D);
	FINISH_ACCEL();

	BEGIN_ACCEL(7);
	OUT_ACCEL_REG(R300_SC_EDGERULE, 0xA5294A5);
	OUT_ACCEL_REG(R300_SC_SCISSOR0, ((0 << R300_SCISSOR_X_SHIFT) |
					 (0 << R300_SCISSOR_Y_SHIFT)));
	OUT_ACCEL_REG(R300_SC_SCISSOR1, ((8191 << R300_SCISSOR_X_SHIFT) |
					 (8191 << R300_SCISSOR_Y_SHIFT)));

	if (IS_R300_3D) {
	    /* clip has offset 1440 */
	    OUT_ACCEL_REG(R300_SC_CLIP_0_A, ((1088 << R300_CLIP_X_SHIFT) |
					     (1088 << R300_CLIP_Y_SHIFT)));
	    OUT_ACCEL_REG(R300_SC_CLIP_0_B, (((1080 + 2920) << R300_CLIP_X_SHIFT) |
					     ((1080 + 2920) << R300_CLIP_Y_SHIFT)));
	} else {
	    OUT_ACCEL_REG(R300_SC_CLIP_0_A, ((0 << R300_CLIP_X_SHIFT) |
					     (0 << R300_CLIP_Y_SHIFT)));
	    OUT_ACCEL_REG(R300_SC_CLIP_0_B, ((4080 << R300_CLIP_X_SHIFT) |
					     (4080 << R300_CLIP_Y_SHIFT)));
	}
	OUT_ACCEL_REG(R300_SC_CLIP_RULE, 0xAAAA);
	OUT_ACCEL_REG(R300_SC_SCREENDOOR, 0xffffff);
	FINISH_ACCEL();

#ifdef IS_RADEON_DRIVER
    } else if ((info->ChipFamily == CHIP_FAMILY_RV250) ||
	       (info->ChipFamily == CHIP_FAMILY_RV280) ||
	       (info->ChipFamily == CHIP_FAMILY_RS300) ||
	       (info->ChipFamily == CHIP_FAMILY_R200)) {

	BEGIN_ACCEL(7);
	if (info->ChipFamily == CHIP_FAMILY_RS300) {
	    OUT_ACCEL_REG(R200_SE_VAP_CNTL_STATUS, RADEON_TCL_BYPASS);
	} else {
	    OUT_ACCEL_REG(R200_SE_VAP_CNTL_STATUS, 0);
	}
	OUT_ACCEL_REG(R200_PP_CNTL_X, 0);
	OUT_ACCEL_REG(R200_PP_TXMULTI_CTL_0, 0);
	OUT_ACCEL_REG(R200_SE_VTX_STATE_CNTL, 0);
	OUT_ACCEL_REG(R200_RE_CNTL, 0x0);
	OUT_ACCEL_REG(R200_SE_VTE_CNTL, 0);
	OUT_ACCEL_REG(R200_SE_VAP_CNTL, R200_VAP_FORCE_W_TO_ONE |
	    R200_VAP_VF_MAX_VTX_NUM);
	FINISH_ACCEL();

	BEGIN_ACCEL(5);
	OUT_ACCEL_REG(RADEON_RE_TOP_LEFT, 0);
	OUT_ACCEL_REG(RADEON_RE_WIDTH_HEIGHT, 0x07ff07ff);
	OUT_ACCEL_REG(RADEON_AUX_SC_CNTL, 0);
	OUT_ACCEL_REG(RADEON_RB3D_PLANEMASK, 0xffffffff);
	OUT_ACCEL_REG(RADEON_SE_CNTL, (RADEON_DIFFUSE_SHADE_GOURAUD |
				       RADEON_BFACE_SOLID |
				       RADEON_FFACE_SOLID |
				       RADEON_VTX_PIX_CENTER_OGL |
				       RADEON_ROUND_MODE_ROUND |
				       RADEON_ROUND_PREC_4TH_PIX));
	FINISH_ACCEL();
    } else {
	BEGIN_ACCEL(2);
	if ((info->ChipFamily == CHIP_FAMILY_RADEON) ||
	    (info->ChipFamily == CHIP_FAMILY_RV200))
	    OUT_ACCEL_REG(RADEON_SE_CNTL_STATUS, 0);
	else
	    OUT_ACCEL_REG(RADEON_SE_CNTL_STATUS, RADEON_TCL_BYPASS);
	OUT_ACCEL_REG(RADEON_SE_COORD_FMT,
	    RADEON_VTX_XY_PRE_MULT_1_OVER_W0 |
	    RADEON_VTX_ST0_NONPARAMETRIC |
	    RADEON_VTX_ST1_NONPARAMETRIC |
	    RADEON_TEX1_W_ROUTING_USE_W0);
	FINISH_ACCEL();

	BEGIN_ACCEL(5);
	OUT_ACCEL_REG(RADEON_RE_TOP_LEFT, 0);
	OUT_ACCEL_REG(RADEON_RE_WIDTH_HEIGHT, 0x07ff07ff);
	OUT_ACCEL_REG(RADEON_AUX_SC_CNTL, 0);
	OUT_ACCEL_REG(RADEON_RB3D_PLANEMASK, 0xffffffff);
	OUT_ACCEL_REG(RADEON_SE_CNTL, (RADEON_DIFFUSE_SHADE_GOURAUD |
				       RADEON_BFACE_SOLID |
				       RADEON_FFACE_SOLID |
				       RADEON_VTX_PIX_CENTER_OGL |
				       RADEON_ROUND_MODE_ROUND |
				       RADEON_ROUND_PREC_4TH_PIX));
	FINISH_ACCEL();
    }
#endif

    END_ACCEL();

    accel_state->XHas3DEngineState = TRUE;
}

#undef FUNC_NAME
