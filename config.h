/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {
/*	"IBM CGA 8x8:style=Regular:size=7",*/
	"JetBrains Mono:size=14"
};

/* solarized colors http://ethanschoonover.com/solarized */
static const char s_base03[]        = "#002b36";
static const char s_base02[]        = "#202020";
static const char s_base01[]        = "#586e75";
static const char s_base00[]        = "#657b83";
static const char s_base0[]         = "#ffffff";
static const char s_base1[]         = "#93a1a1";
static const char s_base2[]         = "#eee8d5";
static const char s_base3[]         = "#fdf6e3";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
	{ s_base0, s_base02, s_base2 },      /* SchemeNorm dark */
	{ s_base0, s_base03, s_base2 },      /* SchemeSel dark */
	{ s_base00, s_base3, s_base02 },     /* SchemeNorm light */
	{ s_base00, s_base2, s_base02},      /* SchemeSel light */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title                       tags mask     isfloating   isterminal noswallow monitor */
	{ "Gimp",     NULL,       NULL,                       0,            1,           0,         0,        -1 },
	{ "St",       NULL,       NULL,                       0,            0,           1,         1,        -1 },
	{ "StFloat",  NULL,       NULL,                       0,            1,           1,         1,        -1 },
	{ "mgba",     NULL,       NULL,                       0,            1,           0,         1,        -1 },
	{ NULL,       NULL,       "Picture-in-Picture",       0,            1,           1,         1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "gaplessgrid.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "###",      gaplessgrid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTMOD Mod1Mask|Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ ALTMOD,                       KEY,      focusnthmon,    {.i = TAG} }, \
	{ ALTMOD|ShiftMask,             KEY,      tagnthmon,      {.i = TAG} },

/* helpers for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define STCMD(cmd) { .v = (const char*[]){ "st", "-e", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon };
static const char *termcmd[]  = { "st", NULL };

/* scratchpad */
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "70x20", NULL };

/* misc */
static const char *floatstcmd[] = { "st", "-c", "StFloat", NULL };

/* autocwd settings */
static int autocwd = 1; /* 0 means no automatic cwd */
static const Arg cwdon = SHCMD("dunstify -t 700 -r 111 \"AUTOCWD: ON\"");
static const Arg cwdoff = SHCMD("dunstify -t 700 -r 111 \"AUTOCWD: OFF\"");

static Key keys[] = {
	/* modifier               key                        function        argument */
	{ MODKEY,                 XK_c,                      togglecwd,      {0} },
	{ MODKEY,                 XK_grave,                  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                 XK_s,                      spawn,          {.v = floatstcmd } },
	{ MODKEY,                 XK_n,                      spawn,          STCMD("nnn") },
	{ MODKEY,                 XK_w,                      spawn,          STCMD("newsboat") },
	{ MODKEY|ShiftMask,       XK_m,                      spawn,          STCMD("neomutt") },
	{ 0,                      XF86XK_Mail,               spawn,          STCMD("neomutt") },
	{ MODKEY,                 XK_y,                      spawn,          SHCMD("clipmenu") },
	{ MODKEY|ShiftMask,       XK_p,                      spawn,          SHCMD("xdg-open $(dbrowse)") },
	{ MODKEY,                 XK_v,                      spawn,          SHCMD("dmenu | mpv") },
	{ MODKEY|ShiftMask,       XK_s,                      spawn,          SHCMD("prmpt shutdown sudo -A shutdown -h now") },
	{ MODKEY|ShiftMask,       XK_r,                      spawn,          SHCMD("prmpt reboot sudo -A reboot") },
	{ MODKEY|ShiftMask,       XK_F6,                     quit,           {0} },
	{ 0,                      XK_Print,                  spawn,          SHCMD("scrshot f") },
	{ MODKEY,                 XK_F1,                     spawn,          SHCMD("scrshot f") },
	{ MODKEY,                 XK_F2,                     spawn,          SHCMD("scrshot w") },
	{ MODKEY,                 XK_F3,                     spawn,          SHCMD("scrshot s") },
	{ MODKEY,                 XK_Home,                   spawn,          SHCMD("$BROWSER") },
	{ 0,                      XF86XK_HomePage,           spawn,          SHCMD("$BROWSER") },
	{ 0,                      XF86XK_AudioRaiseVolume,   spawn,          SHCMD("vol up") },
	{ 0,                      XF86XK_AudioLowerVolume,   spawn,          SHCMD("vol down") },
	{ 0,                      XF86XK_AudioMute,          spawn,          SHCMD("vol toggle") },
	{ 0,                      XF86XK_AudioPlay,          spawn,          SHCMD("mpc toggle") },
	{ 0,                      XF86XK_AudioStop,          spawn,          SHCMD("mpc stop") },
	{ 0,                      XF86XK_AudioNext,          spawn,          SHCMD("mpc next") },
	{ 0,                      XF86XK_AudioPrev,          spawn,          SHCMD("mpc prev") },
	{ MODKEY,                 XK_g,                      setlayout,      {.v = &layouts[3] }},
	{ MODKEY,                 XK_o,                      winview,        {0} },
	{ MODKEY|ShiftMask,       XK_t,                      schemeToggle,   {0} },
	{ MODKEY|ShiftMask,       XK_z,                      schemeCycle,    {0} },
	/*                                                                            */
	{ MODKEY,                 XK_space,                  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,       XK_Return,                 spawn,          {.v = termcmd } },
	{ MODKEY,                 XK_b,                      togglebar,      {0} },
	{ MODKEY,                 XK_j,                      focusstack,     {.i = +1 } },
	{ MODKEY,                 XK_k,                      focusstack,     {.i = -1 } },
	{ MODKEY,                 XK_i,                      incnmaster,     {.i = +1 } },
	{ MODKEY,                 XK_d,                      incnmaster,     {.i = -1 } },
	{ MODKEY,                 XK_h,                      setmfact,       {.f = -0.05} },
	{ MODKEY,                 XK_l,                      setmfact,       {.f = +0.05} },
	{ MODKEY,                 XK_Return,                 zoom,           {0} },
	{ MODKEY,                 XK_q,                      view,           {0} },
	{ MODKEY|ShiftMask,       XK_q,                      killclient,     {0} },
	{ MODKEY|ShiftMask,       XK_c,                      killclient,     {0} },
	{ MODKEY,                 XK_t,                      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                 XK_f,                      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                 XK_m,                      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,       XK_space,                  togglefloating, {0} },
	{ MODKEY,                 XK_0,                      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,       XK_0,                      tag,            {.ui = ~0 } },
	{ MODKEY,                 XK_comma,                  focusmon,       {.i = -1 } },
	{ MODKEY,                 XK_period,                 focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,       XK_comma,                  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,       XK_period,                 tagmon,         {.i = +1 } },
	TAGKEYS(                  XK_1,                      0)
	TAGKEYS(                  XK_2,                      1)
	TAGKEYS(                  XK_3,                      2)
	TAGKEYS(                  XK_4,                      3)
	TAGKEYS(                  XK_5,                      4)
	TAGKEYS(                  XK_6,                      5)
	TAGKEYS(                  XK_7,                      6)
	TAGKEYS(                  XK_8,                      7)
	TAGKEYS(                  XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
