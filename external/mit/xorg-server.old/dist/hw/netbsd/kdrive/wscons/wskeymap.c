/*-
 * Copyright (c) 2014 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by TOYOKURA Atsushi.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <time.h>
#include <sys/ioctl.h>
#include <dev/wscons/wsconsio.h>
#include <dev/wscons/wsksymdef.h>
#include <input.h>
#include <kdrive.h>
#include <X11/keysym.h>
#include <X11/Sunkeysym.h>
#include <X11/XF86keysym.h>

struct keysym_table {
	keysym_t ks;		/* wscons */
	KeySym xk;		/* X11 */
};

static void generateKeySymRec(int fd, const struct keysym_table *table[],
			      KeySymsRec *key);
static KeySym getXkSymFromTable(const struct keysym_table *table[],
				keysym_t ks);

static const struct keysym_table standardKeySym[] = {
	/*
	 * Group Ascii (ISO Latin1) character in low byte
	 */

	{ KS_BackSpace,          XK_BackSpace                   },
	{ KS_Tab,                XK_Tab                         },
	{ KS_Linefeed,           XK_Linefeed                    },
	{ KS_Clear,              XK_Clear                       },
	{ KS_Return,             XK_Return                      },
	{ KS_Escape,             XK_Escape                      },

	{ KS_space,              XK_space                       },
	{ KS_exclam,             XK_exclam                      },
	{ KS_quotedbl,           XK_quotedbl                    },
	{ KS_numbersign,         XK_numbersign                  },
	{ KS_dollar,             XK_dollar                      },
	{ KS_percent,            XK_percent                     },
	{ KS_ampersand,          XK_ampersand                   },
	{ KS_apostrophe,         XK_apostrophe                  },
	{ KS_parenleft,          XK_parenleft                   },
	{ KS_parenright,         XK_parenright                  },
	{ KS_asterisk,           XK_asterisk                    },
	{ KS_plus,               XK_plus                        },
	{ KS_comma,              XK_comma                       },
	{ KS_minus,              XK_minus                       },
	{ KS_period,             XK_period                      },
	{ KS_slash,              XK_slash                       },
	{ KS_0,                  XK_0                           },
	{ KS_1,                  XK_1                           },
	{ KS_2,                  XK_2                           },
	{ KS_3,                  XK_3                           },
	{ KS_4,                  XK_4                           },
	{ KS_5,                  XK_5                           },
	{ KS_6,                  XK_6                           },
	{ KS_7,                  XK_7                           },
	{ KS_8,                  XK_8                           },
	{ KS_9,                  XK_9                           },
	{ KS_colon,              XK_colon                       },
	{ KS_semicolon,          XK_semicolon                   },
	{ KS_less,               XK_less                        },
	{ KS_equal,              XK_equal                       },
	{ KS_greater,            XK_greater                     },
	{ KS_question,           XK_question                    },
	{ KS_at,                 XK_at                          },
	{ KS_A,                  XK_A                           },
	{ KS_B,                  XK_B                           },
	{ KS_C,                  XK_C                           },
	{ KS_D,                  XK_D                           },
	{ KS_E,                  XK_E                           },
	{ KS_F,                  XK_F                           },
	{ KS_G,                  XK_G                           },
	{ KS_H,                  XK_H                           },
	{ KS_I,                  XK_I                           },
	{ KS_J,                  XK_J                           },
	{ KS_K,                  XK_K                           },
	{ KS_L,                  XK_L                           },
	{ KS_M,                  XK_M                           },
	{ KS_N,                  XK_N                           },
	{ KS_O,                  XK_O                           },
	{ KS_P,                  XK_P                           },
	{ KS_Q,                  XK_Q                           },
	{ KS_R,                  XK_R                           },
	{ KS_S,                  XK_S                           },
	{ KS_T,                  XK_T                           },
	{ KS_U,                  XK_U                           },
	{ KS_V,                  XK_V                           },
	{ KS_W,                  XK_W                           },
	{ KS_X,                  XK_X                           },
	{ KS_Y,                  XK_Y                           },
	{ KS_Z,                  XK_Z                           },
	{ KS_bracketleft,        XK_bracketleft                 },
	{ KS_backslash,          XK_backslash                   },
	{ KS_bracketright,       XK_bracketright                },
	{ KS_asciicircum,        XK_asciicircum                 },
	{ KS_underscore,         XK_underscore                  },
	{ KS_grave,              XK_grave                       },
	{ KS_a,                  XK_a                           },
	{ KS_b,                  XK_b                           },
	{ KS_c,                  XK_c                           },
	{ KS_d,                  XK_d                           },
	{ KS_e,                  XK_e                           },
	{ KS_f,                  XK_f                           },
	{ KS_g,                  XK_g                           },
	{ KS_h,                  XK_h                           },
	{ KS_i,                  XK_i                           },
	{ KS_j,                  XK_j                           },
	{ KS_k,                  XK_k                           },
	{ KS_l,                  XK_l                           },
	{ KS_m,                  XK_m                           },
	{ KS_n,                  XK_n                           },
	{ KS_o,                  XK_o                           },
	{ KS_p,                  XK_p                           },
	{ KS_q,                  XK_q                           },
	{ KS_r,                  XK_r                           },
	{ KS_s,                  XK_s                           },
	{ KS_t,                  XK_t                           },
	{ KS_u,                  XK_u                           },
	{ KS_v,                  XK_v                           },
	{ KS_w,                  XK_w                           },
	{ KS_x,                  XK_x                           },
	{ KS_y,                  XK_y                           },
	{ KS_z,                  XK_z                           },
	{ KS_braceleft,          XK_braceleft                   },
	{ KS_bar,                XK_bar                         },
	{ KS_braceright,         XK_braceright                  },
	{ KS_asciitilde,         XK_asciitilde                  },
	{ KS_Delete,             XK_Delete                      },

	{ KS_nobreakspace,       XK_nobreakspace                },
	{ KS_exclamdown,         XK_exclamdown                  },
	{ KS_cent,               XK_cent                        },
	{ KS_sterling,           XK_sterling                    },
	{ KS_currency,           XK_currency                    },
	{ KS_yen,                XK_yen                         },
	{ KS_brokenbar,          XK_brokenbar                   },
	{ KS_section,            XK_section                     },
	{ KS_diaeresis,          XK_diaeresis                   },
	{ KS_copyright,          XK_copyright                   },
	{ KS_ordfeminine,        XK_ordfeminine                 },
	{ KS_guillemotleft,      XK_guillemotleft               },
	{ KS_notsign,            XK_notsign                     },
	{ KS_hyphen,             XK_hyphen                      },
	{ KS_registered,         XK_registered                  },
	{ KS_macron,             XK_macron                      },
	{ KS_degree,             XK_degree                      },
	{ KS_plusminus,          XK_plusminus                   },
	{ KS_twosuperior,        XK_twosuperior                 },
	{ KS_threesuperior,      XK_threesuperior               },
	{ KS_acute,              XK_acute                       },
	{ KS_mu,                 XK_mu                          },
	{ KS_paragraph,          XK_paragraph                   },
	{ KS_periodcentered,     XK_periodcentered              },
	{ KS_cedilla,            XK_cedilla                     },
	{ KS_onesuperior,        XK_onesuperior                 },
	{ KS_masculine,          XK_masculine                   },
	{ KS_guillemotright,     XK_guillemotright              },
	{ KS_onequarter,         XK_onequarter                  },
	{ KS_onehalf,            XK_onehalf                     },
	{ KS_threequarters,      XK_threequarters               },
	{ KS_questiondown,       XK_questiondown                },
	{ KS_Agrave,             XK_Agrave                      },
	{ KS_Aacute,             XK_Aacute                      },
	{ KS_Acircumflex,        XK_Acircumflex                 },
	{ KS_Atilde,             XK_Atilde                      },
	{ KS_Adiaeresis,         XK_Adiaeresis                  },
	{ KS_Aring,              XK_Aring                       },
	{ KS_AE,                 XK_AE                          },
	{ KS_Ccedilla,           XK_Ccedilla                    },
	{ KS_Egrave,             XK_Egrave                      },
	{ KS_Eacute,             XK_Eacute                      },
	{ KS_Ecircumflex,        XK_Ecircumflex                 },
	{ KS_Ediaeresis,         XK_Ediaeresis                  },
	{ KS_Igrave,             XK_Igrave                      },
	{ KS_Iacute,             XK_Iacute                      },
	{ KS_Icircumflex,        XK_Icircumflex                 },
	{ KS_Idiaeresis,         XK_Idiaeresis                  },
	{ KS_ETH,                XK_ETH                         },
	{ KS_Ntilde,             XK_Ntilde                      },
	{ KS_Ograve,             XK_Ograve                      },
	{ KS_Oacute,             XK_Oacute                      },
	{ KS_Ocircumflex,        XK_Ocircumflex                 },
	{ KS_Otilde,             XK_Otilde                      },
	{ KS_Odiaeresis,         XK_Odiaeresis                  },
	{ KS_multiply,           XK_multiply                    },
	{ KS_Ooblique,           XK_Oslash                      },
	{ KS_Ugrave,             XK_Ooblique                    },
	{ KS_Uacute,             XK_Ugrave                      },
	{ KS_Ucircumflex,        XK_Ucircumflex                 },
	{ KS_Udiaeresis,         XK_Udiaeresis                  },
	{ KS_Yacute,             XK_Yacute                      },
	{ KS_THORN,              XK_THORN                       },
	{ KS_ssharp,             XK_ssharp                      },
	{ KS_agrave,             XK_agrave                      },
	{ KS_aacute,             XK_aacute                      },
	{ KS_acircumflex,        XK_acircumflex                 },
	{ KS_atilde,             XK_atilde                      },
	{ KS_adiaeresis,         XK_adiaeresis                  },
	{ KS_aring,              XK_aring                       },
	{ KS_ae,                 XK_ae                          },
	{ KS_ccedilla,           XK_ccedilla                    },
	{ KS_egrave,             XK_egrave                      },
	{ KS_eacute,             XK_eacute                      },
	{ KS_ecircumflex,        XK_ecircumflex                 },
	{ KS_ediaeresis,         XK_ediaeresis                  },
	{ KS_igrave,             XK_igrave                      },
	{ KS_iacute,             XK_iacute                      },
	{ KS_icircumflex,        XK_icircumflex                 },
	{ KS_idiaeresis,         XK_idiaeresis                  },
	{ KS_eth,                XK_eth                         },
	{ KS_ntilde,             XK_ntilde                      },
	{ KS_ograve,             XK_ograve                      },
	{ KS_oacute,             XK_oacute                      },
	{ KS_ocircumflex,        XK_ocircumflex                 },
	{ KS_otilde,             XK_otilde                      },
	{ KS_odiaeresis,         XK_odiaeresis                  },
	{ KS_division,           XK_division                    },
	{ KS_oslash,             XK_oslash                      },
	{ KS_ugrave,             XK_ugrave                      },
	{ KS_uacute,             XK_uacute                      },
	{ KS_ucircumflex,        XK_ucircumflex                 },
	{ KS_udiaeresis,         XK_udiaeresis                  },
	{ KS_yacute,             XK_yacute                      },
	{ KS_thorn,              XK_thorn                       },
	{ KS_ydiaeresis,         XK_ydiaeresis                  },
	{ KS_Abreve,             XK_Abreve                      },
	{ KS_abreve,             XK_abreve                      },
	{ KS_Aogonek,            XK_Aogonek                     },
	{ KS_aogonek,            XK_aogonek                     },
	{ KS_Cacute,             XK_Cacute                      },
	{ KS_cacute,             XK_cacute                      },
	{ KS_Ccaron,             XK_Ccaron                      },
	{ KS_ccaron,             XK_ccaron                      },
	{ KS_Dcaron,             XK_Dcaron                      },
	{ KS_dcaron,             XK_dcaron                      },
	{ KS_Dstroke,            XK_Dstroke                     },
	{ KS_dstroke,            XK_dstroke                     },
	{ KS_Eogonek,            XK_Eogonek                     },
	{ KS_eogonek,            XK_eogonek                     },
	{ KS_Ecaron,             XK_Ecaron                      },
	{ KS_ecaron,             XK_ecaron                      },
	{ KS_Lacute,             XK_Lacute                      },
	{ KS_lacute,             XK_lacute                      },
	{ KS_Lcaron,             XK_Lcaron                      },
	{ KS_lcaron,             XK_lcaron                      },
	{ KS_Lstroke,            XK_Lstroke                     },
	{ KS_lstroke,            XK_lstroke                     },
	{ KS_Nacute,             XK_Nacute                      },
	{ KS_nacute,             XK_nacute                      },
	{ KS_Ncaron,             XK_Ncaron                      },
	{ KS_ncaron,             XK_ncaron                      },
	{ KS_Odoubleacute,       XK_Odoubleacute                },
	{ KS_odoubleacute,       XK_odoubleacute                },
	{ KS_Racute,             XK_Racute                      },
	{ KS_racute,             XK_racute                      },
	{ KS_Rcaron,             XK_Rcaron                      },
	{ KS_rcaron,             XK_rcaron                      },
	{ KS_Sacute,             XK_Sacute                      },
	{ KS_sacute,             XK_sacute                      },
	{ KS_Scedilla,           XK_Scedilla                    },
	{ KS_scedilla,           XK_scedilla                    },
	{ KS_Scaron,             XK_Scaron                      },
	{ KS_scaron,             XK_scaron                      },
	{ KS_Tcedilla,           XK_Tcedilla                    },
	{ KS_tcedilla,           XK_tcedilla                    },
	{ KS_Tcaron,             XK_Tcaron                      },
	{ KS_tcaron,             XK_tcaron                      },
	{ KS_Uabovering,         XK_Uring                       },
	{ KS_uabovering,         XK_uring                       },
	{ KS_Udoubleacute,       XK_Udoubleacute                },
	{ KS_udoubleacute,       XK_udoubleacute                },
	{ KS_Zacute,             XK_Zacute                      },
	{ KS_zacute,             XK_zacute                      },
	{ KS_Zabovedot,          XK_Zabovedot                   },
	{ KS_zabovedot,          XK_zabovedot                   },
	{ KS_Zcaron,             XK_Zcaron                      },
	{ KS_zcaron,             XK_zcaron                      },

	{ KS_caron,              XK_caron                       },
	{ KS_breve,              XK_breve                       },
	{ KS_abovedot,           XK_abovedot                    },
	{ KS_ogonek,             XK_ogonek                      },
	{ KS_doubleacute,        XK_doubleacute                 },

	/*
	 * Group Dead (dead accents)
	 * http://www.unicode.org/charts/PDF/U0300.pdf
	 * dotaccent	= "dot above"
	 * hungarumlaut	= "double acute"
	 * slash	= "short solidus"
	 */

	{ KS_dead_grave,         XK_dead_grave                  },
	{ KS_dead_acute,         XK_dead_acute                  },
	{ KS_dead_circumflex,    XK_dead_circumflex             },
	{ KS_dead_tilde,         XK_dead_tilde                  },
	{ KS_dead_breve,         XK_dead_breve                  },
	{ KS_dead_diaeresis,     XK_dead_diaeresis              },
	{ KS_dead_abovering,     XK_dead_abovering              },
	{ KS_dead_caron,         XK_dead_caron                  },
	{ KS_dead_dotaccent,     XK_dead_abovedot               },
	{ KS_dead_hungarumlaut,  XK_dead_doubleacute            },
	{ KS_dead_ogonek,        XK_dead_ogonek                 },
	{ KS_dead_slash,         XK_dead_stroke                 },
	{ KS_dead_cedilla,       XK_dead_cedilla                },
	{ KS_dead_semi,          XK_dead_ogonek                 },
	{ KS_dead_colon,         XK_dead_iota                   },

	/*
	 * Group Greek
	 */

	{ KS_gr_At,              XK_Greek_ALPHAaccent           },
	{ KS_gr_Et,              XK_Greek_EPSILONaccent         },
	{ KS_gr_Ht,              XK_Greek_ETAaccent             },
	{ KS_gr_It,              XK_Greek_IOTAaccent            },
	{ KS_gr_Ot,              XK_Greek_OMICRONaccent         },
	{ KS_gr_Yt,              XK_Greek_UPSILONaccent         },
	{ KS_gr_Vt,              XK_Greek_OMEGAaccent           },
	{ KS_gr_itd,             XK_Greek_iotaaccentdieresis    },
	{ KS_gr_A,               XK_Greek_ALPHA                 },
	{ KS_gr_B,               XK_Greek_BETA                  },
	{ KS_gr_G,               XK_Greek_GAMMA                 },
	{ KS_gr_D,               XK_Greek_DELTA                 },
	{ KS_gr_E,               XK_Greek_EPSILON               },
	{ KS_gr_Z,               XK_Greek_ZETA                  },
	{ KS_gr_H,               XK_Greek_ETA                   },
	{ KS_gr_U,               XK_Greek_THETA                 },
	{ KS_gr_I,               XK_Greek_IOTA                  },
	{ KS_gr_K,               XK_Greek_KAPPA                 },
	{ KS_gr_L,               XK_Greek_LAMDA                 },
	{ KS_gr_M,               XK_Greek_MU                    },
	{ KS_gr_N,               XK_Greek_NU                    },
	{ KS_gr_J,               XK_Greek_XI                    },
	{ KS_gr_O,               XK_Greek_OMICRON               },
	{ KS_gr_P,               XK_Greek_PI                    },
	{ KS_gr_R,               XK_Greek_RHO                   },
	{ KS_gr_S,               XK_Greek_SIGMA                 },
	{ KS_gr_T,               XK_Greek_TAU                   },
	{ KS_gr_Y,               XK_Greek_UPSILON               },
	{ KS_gr_F,               XK_Greek_PHI                   },
	{ KS_gr_X,               XK_Greek_CHI                   },
	{ KS_gr_C,               XK_Greek_PSI                   },
	{ KS_gr_V,               XK_Greek_OMEGA                 },
	{ KS_gr_Id,              XK_Greek_IOTAdieresis          },
	{ KS_gr_Yd,              XK_Greek_UPSILONdieresis       },
	{ KS_gr_at,              XK_Greek_alphaaccent           },
	{ KS_gr_et,              XK_Greek_epsilonaccent         },
	{ KS_gr_ht,              XK_Greek_etaaccent             },
	{ KS_gr_it,              XK_Greek_iotaaccent            },
	{ KS_gr_ytd,             XK_Greek_upsilonaccentdieresis },
	{ KS_gr_a,               XK_Greek_alpha                 },
	{ KS_gr_b,               XK_Greek_beta                  },
	{ KS_gr_g,               XK_Greek_gamma                 },
	{ KS_gr_d,               XK_Greek_delta                 },
	{ KS_gr_e,               XK_Greek_epsilon               },
	{ KS_gr_z,               XK_Greek_zeta                  },
	{ KS_gr_h,               XK_Greek_eta                   },
	{ KS_gr_u,               XK_Greek_theta                 },
	{ KS_gr_i,               XK_Greek_iota                  },
	{ KS_gr_k,               XK_Greek_kappa                 },
	{ KS_gr_l,               XK_Greek_lamda                 },
	{ KS_gr_m,               XK_Greek_mu                    },
	{ KS_gr_n,               XK_Greek_nu                    },
	{ KS_gr_j,               XK_Greek_xi                    },
	{ KS_gr_o,               XK_Greek_omicron               },
	{ KS_gr_p,               XK_Greek_pi                    },
	{ KS_gr_r,               XK_Greek_rho                   },
	{ KS_gr_teliko_s,        XK_Greek_finalsmallsigma       },
	{ KS_gr_s,               XK_Greek_sigma                 },
	{ KS_gr_t,               XK_Greek_tau                   },
	{ KS_gr_y,               XK_Greek_upsilon               },
	{ KS_gr_f,               XK_Greek_phi                   },
	{ KS_gr_x,               XK_Greek_chi                   },
	{ KS_gr_c,               XK_Greek_psi                   },
	{ KS_gr_v,               XK_Greek_omega                 },
	{ KS_gr_id,              XK_Greek_iotadieresis          },
	{ KS_gr_yd,              XK_Greek_upsilondieresis       },
	{ KS_gr_ot,              XK_Greek_omicronaccent         },
	{ KS_gr_yt,              XK_Greek_upsilonaccent         },
	{ KS_gr_vt,              XK_Greek_omegaaccent           },

	/*
	 * Group 1 (modifiers)
	 */

	{ KS_Shift_L,            XK_Shift_L                     },
	{ KS_Shift_R,            XK_Shift_R                     },
	{ KS_Control_L,          XK_Control_L                   },
	{ KS_Control_R,          XK_Control_R                   },
	{ KS_Caps_Lock,          XK_Caps_Lock                   },
	{ KS_Shift_Lock,         XK_Shift_Lock                  },
	{ KS_Alt_L,              XK_Alt_L                       },
	{ KS_Alt_R,              XK_Alt_R                       },
	{ KS_Multi_key,          XK_Multi_key                   },
	{ KS_Mode_switch,        XK_Mode_switch                 },
	{ KS_Num_Lock,           XK_Num_Lock                    },
	{ KS_Hold_Screen,        XK_Pause                       },

	/* KS_Cmd  */
	/* KS_Cmd1 */
	/* KS_Cmd2 */

	{ KS_Meta_L,             XK_Meta_L                      },
	{ KS_Meta_R,             XK_Meta_R                      },
	{ KS_Zenkaku_Hankaku,    XK_Zenkaku_Hankaku             },
	{ KS_Hiragana_Katakana,  XK_Hiragana_Katakana           },
	{ KS_Henkan_Mode,        XK_Henkan_Mode                 },
	{ KS_Henkan,             XK_Henkan                      },
	{ KS_Muhenkan,           XK_Muhenkan                    },

	/*
	 * Group 2 (keypad) character in low byte
	 */

	{ KS_KP_F1,              XK_KP_F1                       },
	{ KS_KP_F2,              XK_KP_F2                       },
	{ KS_KP_F3,              XK_KP_F3                       },
	{ KS_KP_F4,              XK_KP_F4                       },
	{ KS_KP_Home,            XK_KP_Home                     },
	{ KS_KP_Left,            XK_KP_Left                     },
	{ KS_KP_Up,              XK_KP_Up                       },
	{ KS_KP_Right,           XK_KP_Right                    },
	{ KS_KP_Down,            XK_KP_Down                     },
	{ KS_KP_Prior,           XK_KP_Prior                    },
	{ KS_KP_Next,            XK_KP_Next                     },
	{ KS_KP_End,             XK_KP_End                      },
	{ KS_KP_Begin,           XK_KP_Begin                    },
	{ KS_KP_Insert,          XK_KP_Insert                   },
	{ KS_KP_Delete,          XK_KP_Delete                   },

	{ KS_KP_Space,           XK_KP_Space                    },
	{ KS_KP_Tab,             XK_KP_Tab                      },
	{ KS_KP_Enter,           XK_KP_Enter                    },
	{ KS_KP_Equal,           XK_KP_Equal                    },
	{ KS_KP_Numbersign,      XK_numbersign                  },
	{ KS_KP_Multiply,        XK_KP_Multiply                 },
	{ KS_KP_Add,             XK_KP_Add                      },
	{ KS_KP_Separator,       XK_KP_Separator                },
	{ KS_KP_Subtract,        XK_KP_Subtract                 },
	{ KS_KP_Decimal,         XK_KP_Decimal                  },
	{ KS_KP_Divide,          XK_KP_Divide                   },
	{ KS_KP_0,               XK_KP_0                        },
	{ KS_KP_1,               XK_KP_1                        },
	{ KS_KP_2,               XK_KP_2                        },
	{ KS_KP_3,               XK_KP_3                        },
	{ KS_KP_4,               XK_KP_4                        },
	{ KS_KP_5,               XK_KP_5                        },
	{ KS_KP_6,               XK_KP_6                        },
	{ KS_KP_7,               XK_KP_7                        },
	{ KS_KP_8,               XK_KP_8                        },
	{ KS_KP_9,               XK_KP_9                        },

	/*
	 * Group 3 (function)
	 */

	{ KS_f1,                 XK_F1                          },
	{ KS_f2,                 XK_F2                          },
	{ KS_f3,                 XK_F3                          },
	{ KS_f4,                 XK_F4                          },
	{ KS_f5,                 XK_F5                          },
	{ KS_f6,                 XK_F6                          },
	{ KS_f7,                 XK_F7                          },
	{ KS_f8,                 XK_F8                          },
	{ KS_f9,                 XK_F9                          },
	{ KS_f10,                XK_F10                         },
	{ KS_f11,                XK_F11                         },
	{ KS_f12,                XK_F12                         },
	{ KS_f13,                XK_F13                         },
	{ KS_f14,                XK_F14                         },
	{ KS_f15,                XK_F15                         },
	{ KS_f16,                XK_F16                         },
	{ KS_f17,                XK_F17                         },
	{ KS_f18,                XK_F18                         },
	{ KS_f19,                XK_F19                         },
	{ KS_f20,                XK_F20                         },

	{ KS_F1,                 XK_F1                          },
	{ KS_F2,                 XK_F2                          },
	{ KS_F3,                 XK_F3                          },
	{ KS_F4,                 XK_F4                          },
	{ KS_F5,                 XK_F5                          },
	{ KS_F6,                 XK_F6                          },
	{ KS_F7,                 XK_F7                          },
	{ KS_F8,                 XK_F8                          },
	{ KS_F9,                 XK_F9                          },
	{ KS_F10,                XK_F10                         },
	{ KS_F11,                XK_F11                         },
	{ KS_F12,                XK_F12                         },
	{ KS_F13,                XK_F13                         },
	{ KS_F14,                XK_F14                         },
	{ KS_F15,                XK_F15                         },
	{ KS_F16,                XK_F16                         },
	{ KS_F17,                XK_F17                         },
	{ KS_F18,                XK_F18                         },
	{ KS_F19,                XK_F19                         },
	{ KS_F20,                XK_F20                         },

	{ KS_Home,               XK_Home                        },
	{ KS_Prior,              XK_Prior                       },
	{ KS_Next,               XK_Next                        },
	{ KS_Up,                 XK_Up                          },
	{ KS_Down,               XK_Down                        },
	{ KS_Left,               XK_Left                        },
	{ KS_Right,              XK_Right                       },
	{ KS_End,                XK_End                         },
	{ KS_Insert,             XK_Insert                      },
	{ KS_Help,               XK_Help                        },
	{ KS_Execute,            XK_Execute                     },
	{ KS_Find,               XK_Find                        },
	{ KS_Select,             XK_Select                      },
	{ KS_Again,              XK_Redo                        },
	{ KS_Undo,               XK_Undo                        },

	{ KS_Menu,               XK_Menu                        },
	{ KS_Pause,              XK_Pause                       },
	{ KS_Print_Screen,       XK_Print                       },

	{ 0xFFFF,                NoSymbol                       }
};

static const struct keysym_table *standardKeyTable[] = {
	standardKeySym, NULL
};

#if 0
static const struct keysym_table sunKeySym[] = {
	{ KS_Cmd,                XK_Help                        },

	{ KS_Power,              SunXK_PowerSwitch              },

	{ KS_Again,              SunXK_Again                    },
	{ KS_Props,              SunXK_Props                    },
	{ KS_Undo,               SunXK_Undo                     },
	{ KS_Front,              SunXK_Front                    },
	{ KS_Copy,               SunXK_Copy                     },
	{ KS_Open,               SunXK_Open                     },
	{ KS_Paste,              SunXK_Paste                    },
	{ KS_Cut,                SunXK_Cut                      },

	{ KS_Cmd_BrightnessUp,   SunXK_VideoRaiseBrightness     },
	{ KS_Cmd_BrightnessDown, SunXK_VideoLowerBrightness     },
	{ KS_Cmd_VolumeUp,       SunXK_AudioRaiseVolume         },
	{ KS_Cmd_VolumeDown,     SunXK_AudioLowerVolume         },
	{ KS_Cmd_VolumeToggle,   SunXK_AudioMute                }

	{ 0xFFFF,                NoSymbol                       }
};

static const struct keysym_table *sunKeyTable[] = {
	sunKeySym, NULL
};

static const struct keysym_table xf86KeySym[] = {
	{ KS_Power,              XF86XK_Standby                 },

	{ KS_Copy,               XF86XK_Copy                    },
	{ KS_Open,               XF86XK_Open                    },
	{ KS_Paste,              XF86XK_Paste                   },
	{ KS_Cut,                XF86XK_Cut                     },

	{ 0xFFFF,                NoSymbol                       }
};
#endif

Bool
wskeymapInitKeySymRec(int fd, KeySymsRec *key)
{
	u_int type = 0;

	if (ioctl(fd, WSKBDIO_GTYPE, &type) == -1) {
		ErrorF("wskeymap: cannot read \n");
		return FALSE;
	}

	switch (type) {
#if 0
	case WSKBD_TYPE_SUN:
	case WSKBD_TYPE_SUN5:
		generateKeySymRec(fd, sunKeyTable, key);
		break;
#endif
	default:
		generateKeySymRec(fd, standardKeyTable, key);
		break;
	}

	return TRUE;
}


Bool
wskeymapInitModMap(KeySymsRec *keySyms, CARD8 *modMap)
{
	int i, n;

	for (i = 0; i < KD_MAX_LENGTH; i++)
		modMap[i] = NoSymbol;

	if (keySyms->minKeyCode < KD_MIN_KEYCODE) {
		keySyms->minKeyCode += KD_MIN_KEYCODE;
		keySyms->maxKeyCode += KD_MIN_KEYCODE;
	}

	for (i = keySyms->minKeyCode; i <= keySyms->maxKeyCode; i++) {
		n = (i - keySyms->minKeyCode) * KD_MAX_WIDTH;
		switch (keySyms->map[n]) {
		case XK_Shift_L:
		case XK_Shift_R:
			modMap[i] = ShiftMask;
			break;
		case XK_Control_L:
		case XK_Control_R:
			modMap[i] = ControlMask;
			break;
		case XK_Alt_L:
		case XK_Alt_R:
			modMap[i] = Mod1Mask;
			break;
		case XK_Meta_L:
		case XK_Meta_R:
			modMap[i] = Mod2Mask;
			break;
		case XK_Caps_Lock:
			modMap[i] = LockMask;
			break;
		default:
			break;
		}
	}

	return TRUE;
}

static void
generateKeySymRec(int fd,  const struct keysym_table *table[], KeySymsRec *key)
{
	struct wscons_keymap wscons_keymap[WSKBDIO_MAXMAPLEN];
	struct wscons_keymap *keymap;
	struct wskbd_map_data map_data;
	KeySym *x_key;
	int i, length;

	memset(wscons_keymap, 0, sizeof(wscons_keymap));
	map_data.maplen = WSKBDIO_MAXMAPLEN;
	map_data.map    = wscons_keymap;

	/* Get keymap */
	if (ioctl(fd, WSKBDIO_GETMAP, &map_data) == -1) {
		ErrorF("wskeymap: cannot get WSKBDIO_GETMAP");
		return;
	}

	length = (key->maxKeyCode - key->minKeyCode + 1) * key->mapWidth;
	for (i = 0; i < length; i++)
		key->map[i] = NoSymbol;

	x_key = key->map;
	for (i = 0; i < map_data.maplen && i <= key->maxKeyCode; i++) {
		keymap = &wscons_keymap[i];
		*x_key++ = getXkSymFromTable(table, keymap->group1[0]);
		*x_key++ = getXkSymFromTable(table, keymap->group1[1]);
		*x_key++ = getXkSymFromTable(table, keymap->group2[0]);
		*x_key++ = getXkSymFromTable(table, keymap->group2[1]);
	}

	key->maxKeyCode = i - 1;

	for (i = 0; i <= key->maxKeyCode; i ++) {
		ErrorF("wskeymap: (%3d) [%04x] %04x %04x %04x %04x, %04x %04x %04x %04x\n",
		       i, wscons_keymap[i].command,
		       wscons_keymap[i].group1[0], wscons_keymap[i].group1[1],
		       wscons_keymap[i].group2[0], wscons_keymap[i].group2[1],
		       key->map[i * 4 + 0], key->map[i * 4 + 1],
		       key->map[i * 4 + 2], key->map[i * 4 + 3]);
	}
}

static KeySym
getXkSymFromTable(const struct keysym_table *table[], keysym_t ks)
{
	int i, j;
	const struct keysym_table *keysym;

	for (i = 0; table[i] != NULL; i++) {
		keysym = (const struct keysym_table *)table[i];
		for (j = 0; keysym[j].ks != 0xFFFF; j++) {
			if (ks == keysym[j].ks)
				return keysym[j].xk;
		}
	}

	return NoSymbol;
}

#define KEY_Escape       /* Escape                0x01  */    1  
#define KEY_1            /* 1           !         0x02  */    2 
#define KEY_2            /* 2           @         0x03  */    3 
#define KEY_3            /* 3           #         0x04  */    4 
#define KEY_4            /* 4           $         0x05  */    5 
#define KEY_5            /* 5           %         0x06  */    6 
#define KEY_6            /* 6           ^         0x07  */    7 
#define KEY_7            /* 7           &         0x08  */    8 
#define KEY_8            /* 8           *         0x09  */    9 
#define KEY_9            /* 9           (         0x0a  */   10 
#define KEY_0            /* 0           )         0x0b  */   11 
#define KEY_Minus        /* - (Minus)   _ (Under) 0x0c  */   12
#define KEY_Equal        /* = (Equal)   +         0x0d  */   13 
#define KEY_BackSpace    /* Back Space            0x0e  */   14 
#define KEY_Tab          /* Tab                   0x0f  */   15
#define KEY_Q            /* Q                     0x10  */   16
#define KEY_W            /* W                     0x11  */   17
#define KEY_E            /* E                     0x12  */   18
#define KEY_R            /* R                     0x13  */   19
#define KEY_T            /* T                     0x14  */   20
#define KEY_Y            /* Y                     0x15  */   21
#define KEY_U            /* U                     0x16  */   22
#define KEY_I            /* I                     0x17  */   23
#define KEY_O            /* O                     0x18  */   24
#define KEY_P            /* P                     0x19  */   25
#define KEY_LBrace       /* [           {         0x1a  */   26
#define KEY_RBrace       /* ]           }         0x1b  */   27 
#define KEY_Enter        /* Enter                 0x1c  */   28
#define KEY_LCtrl        /* Ctrl(left)            0x1d  */   29
#define KEY_A            /* A                     0x1e  */   30
#define KEY_S            /* S                     0x1f  */   31
#define KEY_D            /* D                     0x20  */   32 
#define KEY_F            /* F                     0x21  */   33
#define KEY_G            /* G                     0x22  */   34
#define KEY_H            /* H                     0x23  */   35
#define KEY_J            /* J                     0x24  */   36
#define KEY_K            /* K                     0x25  */   37
#define KEY_L            /* L                     0x26  */   38
#define KEY_SemiColon    /* ;(SemiColon) :(Colon) 0x27  */   39
#define KEY_Quote        /* ' (Apostr)  " (Quote) 0x28  */   40
#define KEY_Tilde        /* ` (Accent)  ~ (Tilde) 0x29  */   41
#define KEY_ShiftL       /* Shift(left)           0x2a  */   42
#define KEY_BSlash       /* \(BckSlash) |(VertBar)0x2b  */   43
#define KEY_Z            /* Z                     0x2c  */   44
#define KEY_X            /* X                     0x2d  */   45
#define KEY_C            /* C                     0x2e  */   46
#define KEY_V            /* V                     0x2f  */   47
#define KEY_B            /* B                     0x30  */   48
#define KEY_N            /* N                     0x31  */   49
#define KEY_M            /* M                     0x32  */   50
#define KEY_Comma        /* , (Comma)   < (Less)  0x33  */   51
#define KEY_Period       /* . (Period)  >(Greater)0x34  */   52
#define KEY_Slash        /* / (Slash)   ?         0x35  */   53
#define KEY_ShiftR       /* Shift(right)          0x36  */   54
#define KEY_KP_Multiply  /* *                     0x37  */   55
#define KEY_Alt          /* Alt(left)             0x38  */   56
#define KEY_Space        /*   (SpaceBar)          0x39  */   57
#define KEY_CapsLock     /* CapsLock              0x3a  */   58
#define KEY_F1           /* F1                    0x3b  */   59
#define KEY_F2           /* F2                    0x3c  */   60
#define KEY_F3           /* F3                    0x3d  */   61
#define KEY_F4           /* F4                    0x3e  */   62
#define KEY_F5           /* F5                    0x3f  */   63
#define KEY_F6           /* F6                    0x40  */   64
#define KEY_F7           /* F7                    0x41  */   65
#define KEY_F8           /* F8                    0x42  */   66
#define KEY_F9           /* F9                    0x43  */   67
#define KEY_F10          /* F10                   0x44  */   68
#define KEY_NumLock      /* NumLock               0x45  */   69
#define KEY_ScrollLock   /* ScrollLock            0x46  */   70
#define KEY_KP_7         /* 7           Home      0x47  */   71 
#define KEY_KP_8         /* 8           Up        0x48  */   72 
#define KEY_KP_9         /* 9           PgUp      0x49  */   73 
#define KEY_KP_Minus     /* - (Minus)             0x4a  */   74
#define KEY_KP_4         /* 4           Left      0x4b  */   75
#define KEY_KP_5         /* 5                     0x4c  */   76
#define KEY_KP_6         /* 6           Right     0x4d  */   77
#define KEY_KP_Plus      /* + (Plus)              0x4e  */   78
#define KEY_KP_1         /* 1           End       0x4f  */   79
#define KEY_KP_2         /* 2           Down      0x50  */   80
#define KEY_KP_3         /* 3           PgDown    0x51  */   81
#define KEY_KP_0         /* 0           Insert    0x52  */   82
#define KEY_KP_Decimal   /* . (Decimal) Delete    0x53  */   83 
#define KEY_SysReqest    /* SysReqest             0x54  */   84
                         /* NOTUSED               0x55  */
#define KEY_Less         /* < (Less)   >(Greater) 0x56  */   86
#define KEY_F11          /* F11                   0x57  */   87
#define KEY_F12          /* F12                   0x58  */   88

#define KEY_Prefix0      /* special               0x60  */   96
#define KEY_Prefix1      /* specail               0x61  */   97

/*
 * The 'scancodes' below are generated by the server, because the MF101/102
 * keyboard sends them as sequence of other scancodes
 */
#define KEY_Home         /* Home                  0x59  */   89
#define KEY_Up           /* Up                    0x5a  */   90
#define KEY_PgUp         /* PgUp                  0x5b  */   91
#define KEY_Left         /* Left                  0x5c  */   92
#define KEY_Begin        /* Begin                 0x5d  */   93
#define KEY_Right        /* Right                 0x5e  */   94
#define KEY_End          /* End                   0x5f  */   95
#define KEY_Down         /* Down                  0x60  */   96
#define KEY_PgDown       /* PgDown                0x61  */   97
#define KEY_Insert       /* Insert                0x62  */   98
#define KEY_Delete       /* Delete                0x63  */   99
#define KEY_KP_Enter     /* Enter                 0x64  */  100
#define KEY_RCtrl        /* Ctrl(right)           0x65  */  101
#define KEY_Pause        /* Pause                 0x66  */  102
#define KEY_Print        /* Print                 0x67  */  103
#define KEY_KP_Divide    /* Divide                0x68  */  104
#define KEY_AltLang      /* AtlLang(right)        0x69  */  105
#define KEY_Break        /* Break                 0x6a  */  106
#define KEY_LMeta        /* Left Meta             0x6b  */  107
#define KEY_RMeta        /* Right Meta            0x6c  */  108
#define KEY_Menu         /* Menu                  0x6d  */  109
#define KEY_F13          /* F13                   0x6e  */  110
#define KEY_F14          /* F14                   0x6f  */  111
#define KEY_F15          /* F15                   0x70  */  112
#define KEY_HKTG         /* Hirugana/Katakana tog 0x70  */  112
#define KEY_F16          /* F16                   0x71  */  113
#define KEY_F17          /* F17                   0x72  */  114
#define KEY_KP_DEC       /* KP_DEC                0x73  */  115
#define KEY_BSlash2      /* \           _         0x73  */  115
#define KEY_KP_Equal	 /* Equal (Keypad)        0x76  */  118
#define KEY_XFER         /* Kanji Transfer        0x79  */  121
#define KEY_NFER         /* No Kanji Transfer     0x7b  */  123
#define KEY_Yen          /* Yen                   0x7d  */  125

#define KEY_Power        /* Power Key             0x84  */  132
#define KEY_Mute         /* Audio Mute            0x85  */  133
#define KEY_AudioLower   /* Audio Lower           0x86  */  134
#define KEY_AudioRaise   /* Audio Raise           0x87  */  135
#define KEY_Help         /* Help                  0x88  */  136
#define KEY_L1           /* Stop                  0x89  */  137
#define KEY_L2           /* Again                 0x8a  */  138
#define KEY_L3           /* Props                 0x8b  */  139
#define KEY_L4           /* Undo                  0x8c  */  140
#define KEY_L5           /* Front                 0x8d  */  141
#define KEY_L6           /* Copy                  0x8e  */  142
#define KEY_L7           /* Open                  0x8f  */  143
#define KEY_L8           /* Paste                 0x90  */  144
#define KEY_L9           /* Find                  0x91  */  145
#define KEY_L10          /* Cut                   0x92  */  146

/*
 * Fake 'scancodes' in the following ranges are generated for 2-byte
 * codes not handled elsewhere.  These correspond to most extended keys
 * on so-called "Internet" keyboards:
 *
 *	0x79-0x93
 *	0x96-0xa1
 *	0xa3-0xac
 *	0xb1-0xb4
 *	0xba-0xbd
 *	0xc2
 *	0xcc-0xd2
 *	0xd6-0xf7
 */

/*
 * Remapped 'scancodes' are generated for single-byte codes in the range
 * 0x59-0x5f,0x62-0x76.  These are used for some extra keys on some keyboards.
 */

#define KEY_0x59		0x95
#define KEY_0x5A		0xA2
#define KEY_0x5B		0xAD
#define KEY_0x5C		KEY_KP_EQUAL
#define KEY_0x5D		0xAE
#define KEY_0x5E		0xAF
#define KEY_0x5F		0xB0
#define KEY_0x62		0xB5
#define KEY_0x63		0xB6
#define KEY_0x64		0xB7
#define KEY_0x65		0xB8
#define KEY_0x66		0xB9
#define KEY_0x67		0xBE
#define KEY_0x68		0xBF
#define KEY_0x69		0xC0
#define KEY_0x6A		0xC1
#define KEY_0x6B		0xC3
#define KEY_0x6C		0xC4
#define KEY_0x6D		0xC5
#define KEY_0x6E		0xC6
#define KEY_0x6F		0xC7
#define KEY_0x70		0xC8
#define KEY_0x71		0xC9
#define KEY_0x72		0xCA
#define KEY_0x73		0xCB
#define KEY_0x74		0xD3
#define KEY_0x75		0xD4
#define KEY_0x76		0xD5

/* These are for "notused" and "unknown" entries in translation maps. */
#define KEY_NOTUSED	  0
#define KEY_UNKNOWN	255

static unsigned char wsXtMap[] = {
	/* 0 */ KEY_NOTUSED,
	/* 1 */ KEY_Escape,
	/* 2 */ KEY_1,
	/* 3 */ KEY_2,
	/* 4 */ KEY_3,		
	/* 5 */ KEY_4,
	/* 6 */ KEY_5,
	/* 7 */ KEY_6,
	/* 8 */ KEY_7,
	/* 9 */ KEY_8,
	/* 10 */ KEY_9,
	/* 11 */ KEY_0,
	/* 12 */ KEY_Minus,
	/* 13 */ KEY_Equal,
	/* 14 */ KEY_BackSpace,
	/* 15 */ KEY_Tab,
	/* 16 */ KEY_Q,
	/* 17 */ KEY_W,
	/* 18 */ KEY_E,
	/* 19 */ KEY_R,
	/* 20 */ KEY_T,
	/* 21 */ KEY_Y,
	/* 22 */ KEY_U,
	/* 23 */ KEY_I,
	/* 24 */ KEY_O,
	/* 25 */ KEY_P,
	/* 26 */ KEY_LBrace,
	/* 27 */ KEY_RBrace,
	/* 28 */ KEY_Enter,
	/* 29 */ KEY_LCtrl,
	/* 30 */ KEY_A,
	/* 31 */ KEY_S,
	/* 32 */ KEY_D,
	/* 33 */ KEY_F,
	/* 34 */ KEY_G,
	/* 35 */ KEY_H,
	/* 36 */ KEY_J,
	/* 37 */ KEY_K,
	/* 38 */ KEY_L,
	/* 39 */ KEY_SemiColon,
	/* 40 */ KEY_Quote,
	/* 41 */ KEY_Tilde,
	/* 42 */ KEY_ShiftL,
	/* 43 */ KEY_BSlash,
	/* 44 */ KEY_Z,
	/* 45 */ KEY_X,
	/* 46 */ KEY_C,
	/* 47 */ KEY_V,
	/* 48 */ KEY_B,
	/* 49 */ KEY_N,
	/* 50 */ KEY_M,
	/* 51 */ KEY_Comma,
	/* 52 */ KEY_Period,
	/* 53 */ KEY_Slash,
	/* 54 */ KEY_ShiftR,
	/* 55 */ KEY_KP_Multiply,
	/* 56 */ KEY_Alt,
	/* 57 */ KEY_Space,
	/* 58 */ KEY_CapsLock,
	/* 59 */ KEY_F1,
	/* 60 */ KEY_F2,
	/* 61 */ KEY_F3,
	/* 62 */ KEY_F4,
	/* 63 */ KEY_F5,
	/* 64 */ KEY_F6,
	/* 65 */ KEY_F7,
	/* 66 */ KEY_F8,
	/* 67 */ KEY_F9,
	/* 68 */ KEY_F10,
	/* 69 */ KEY_NumLock,
	/* 70 */ KEY_ScrollLock,
	/* 71 */ KEY_KP_7,
	/* 72 */ KEY_KP_8,
	/* 73 */ KEY_KP_9,
	/* 74 */ KEY_KP_Minus,
	/* 75 */ KEY_KP_4,
	/* 76 */ KEY_KP_5,
	/* 77 */ KEY_KP_6,
	/* 78 */ KEY_KP_Plus,
	/* 79 */ KEY_KP_1,
	/* 80 */ KEY_KP_2,
	/* 81 */ KEY_KP_3,
	/* 82 */ KEY_KP_0,
	/* 83 */ KEY_KP_Decimal,
	/* 84 */ KEY_NOTUSED,
	/* 85 */ KEY_NOTUSED,
	/* 86 */ KEY_Less,	/* backslash on uk, < on german */
	/* 87 */ KEY_F11,
	/* 88 */ KEY_F12,
	/* 89 */ KEY_NOTUSED,
	/* 90 */ KEY_NOTUSED,
	/* 91 */ KEY_NOTUSED,
	/* 92 */ KEY_NOTUSED,
	/* 93 */ KEY_NOTUSED,
	/* 94 */ KEY_NOTUSED,
	/* 95 */ KEY_NOTUSED,
	/* 96 */ KEY_NOTUSED,
	/* 97 */ KEY_NOTUSED,
	/* 98 */ KEY_NOTUSED,
	/* 99 */ KEY_NOTUSED,
	/* 100 */ KEY_NOTUSED,
	/* 101 */ KEY_NOTUSED,
	/* 102 */ KEY_NOTUSED,
	/* 103 */ KEY_NOTUSED,
	/* 104 */ KEY_NOTUSED,
	/* 105 */ KEY_NOTUSED,
	/* 106 */ KEY_NOTUSED,
	/* 107 */ KEY_NOTUSED,
	/* 108 */ KEY_NOTUSED,
	/* 109 */ KEY_NOTUSED,
	/* 110 */ KEY_NOTUSED,
	/* 111 */ KEY_NOTUSED,
	/* 112 */ KEY_NOTUSED,
	/* 113 */ KEY_NOTUSED,
	/* 114 */ KEY_NOTUSED,
	/* 115 */ KEY_NOTUSED,
	/* 116 */ KEY_NOTUSED,
	/* 117 */ KEY_NOTUSED,
	/* 118 */ KEY_NOTUSED,
	/* 119 */ KEY_NOTUSED,
	/* 120 */ KEY_NOTUSED,
	/* 121 */ KEY_NOTUSED,
	/* 122 */ KEY_NOTUSED,
	/* 123 */ KEY_NOTUSED,
	/* 124 */ KEY_NOTUSED,
	/* 125 */ KEY_NOTUSED,
	/* 126 */ KEY_NOTUSED,
	/* 127 */ KEY_Pause,
	/* 128 */ KEY_NOTUSED,
	/* 129 */ KEY_NOTUSED,
	/* 130 */ KEY_NOTUSED,
	/* 131 */ KEY_NOTUSED,
	/* 132 */ KEY_NOTUSED,
	/* 133 */ KEY_NOTUSED,
	/* 134 */ KEY_NOTUSED,
	/* 135 */ KEY_NOTUSED,
	/* 136 */ KEY_NOTUSED,
	/* 137 */ KEY_NOTUSED,
	/* 138 */ KEY_NOTUSED,
	/* 139 */ KEY_NOTUSED,
	/* 140 */ KEY_NOTUSED,
	/* 141 */ KEY_NOTUSED,
	/* 142 */ KEY_NOTUSED,
	/* 143 */ KEY_NOTUSED,
	/* 144 */ KEY_NOTUSED,
	/* 145 */ KEY_NOTUSED,
	/* 146 */ KEY_NOTUSED,
	/* 147 */ KEY_NOTUSED,
	/* 148 */ KEY_NOTUSED,
	/* 149 */ KEY_NOTUSED,
	/* 150 */ KEY_NOTUSED,
	/* 151 */ KEY_NOTUSED,
	/* 152 */ KEY_NOTUSED,
	/* 153 */ KEY_NOTUSED,
	/* 154 */ KEY_NOTUSED,
	/* 155 */ KEY_NOTUSED,
	/* 156 */ KEY_KP_Enter,
	/* 157 */ KEY_RCtrl,
	/* 158 */ KEY_NOTUSED,
	/* 159 */ KEY_NOTUSED,
	/* 160 */ KEY_Mute,
	/* 161 */ KEY_NOTUSED,
	/* 162 */ KEY_NOTUSED,
	/* 163 */ KEY_NOTUSED,
	/* 164 */ KEY_NOTUSED,
	/* 165 */ KEY_NOTUSED,
	/* 166 */ KEY_NOTUSED,
	/* 167 */ KEY_NOTUSED,
	/* 168 */ KEY_NOTUSED,
	/* 169 */ KEY_NOTUSED,
	/* 170 */ KEY_Print,
	/* 171 */ KEY_NOTUSED,
	/* 172 */ KEY_NOTUSED,
	/* 173 */ KEY_NOTUSED,
	/* 174 */ KEY_AudioLower,
	/* 175 */ KEY_AudioRaise,
	/* 176 */ KEY_NOTUSED,
	/* 177 */ KEY_NOTUSED,
	/* 178 */ KEY_NOTUSED,
	/* 179 */ KEY_NOTUSED,
	/* 180 */ KEY_NOTUSED,
	/* 181 */ KEY_KP_Divide,
	/* 182 */ KEY_NOTUSED,
	/* 183 */ KEY_Print,
	/* 184 */ KEY_AltLang,
	/* 185 */ KEY_NOTUSED,
	/* 186 */ KEY_NOTUSED,
	/* 187 */ KEY_NOTUSED,
	/* 188 */ KEY_NOTUSED,
	/* 189 */ KEY_NOTUSED,
	/* 190 */ KEY_NOTUSED,
	/* 191 */ KEY_NOTUSED,
	/* 192 */ KEY_NOTUSED,
	/* 193 */ KEY_NOTUSED,
	/* 194 */ KEY_NOTUSED,
	/* 195 */ KEY_NOTUSED,
	/* 196 */ KEY_NOTUSED,
	/* 197 */ KEY_NOTUSED,
	/* 198 */ KEY_NOTUSED,
	/* 199 */ KEY_Home,
	/* 200 */ KEY_Up,
	/* 201 */ KEY_PgUp,
	/* 202 */ KEY_NOTUSED,
	/* 203 */ KEY_Left,
	/* 204 */ KEY_NOTUSED,
	/* 205 */ KEY_Right,
	/* 206 */ KEY_NOTUSED,
	/* 207 */ KEY_End,
	/* 208 */ KEY_Down,
	/* 209 */ KEY_PgDown,
	/* 210 */ KEY_Insert,
	/* 211 */ KEY_Delete,
	/* 212 */ KEY_NOTUSED,
	/* 213 */ KEY_NOTUSED,
	/* 214 */ KEY_NOTUSED,
	/* 215 */ KEY_NOTUSED,
	/* 216 */ KEY_NOTUSED,
	/* 217 */ KEY_NOTUSED,
	/* 218 */ KEY_NOTUSED,
	/* 219 */ KEY_LMeta,
	/* 220 */ KEY_RMeta,
	/* 221 */ KEY_Menu
};

unsigned char wskeymapGetScancode(keysym_t ks)
{
	ErrorF("wskeymapGetScancode: from %d to %d\n", ks, wsXtMap[ks]);
	return wsXtMap[ks];
}
