/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 1;        /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",      dwindle },
};

/* key definitions */
#define Alt Mod1Mask
#define Alt(KEY,TAG) \
	{ Alt,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ Alt|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ Alt + Shift,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ Alt|ControlMask + Shift, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ Alt,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ Alt + Shift,             XK_Return, spawn,          {.v = termcmd } },
	{ Alt,                       XK_b,      togglebar,      {0} },
	{ Alt,                       XK_j,      focusstack,     {.i = +1 } },
	{ Alt,                       XK_k,      focusstack,     {.i = -1 } },
	{ Alt,                       XK_i,      incnmaster,     {.i = +1 } },
	{ Alt,                       XK_d,      incnmaster,     {.i = -1 } },
	{ Alt,                       XK_h,      setmfact,       {.f = -0.05} },
	{ Alt,                       XK_l,      setmfact,       {.f = +0.05} },
	{ Alt,                       XK_Return, zoom,           {0} },
	{ Alt,                       XK_Tab,    view,           {0} },
	{ Alt + Shift,             XK_c,      killclient,     {0} },
	{ Alt,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ Alt,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ Alt,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ Alt,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ Alt + Shift,             XK_r,      setlayout,      {.v = &layouts[4]} },
	{ Alt,                       XK_space,  setlayout,      {0} },
	{ Alt + Shift,             XK_space,  togglefloating, {0} },
	{ Alt,                       XK_0,      view,           {.ui = ~0 } },
	{ Alt + Shift,             XK_0,      tag,            {.ui = ~0 } },
	{ Alt,                       XK_comma,  focusmon,       {.i = -1 } },
	{ Alt,                       XK_period, focusmon,       {.i = +1 } },
	{ Alt + Shift,             XK_comma,  tagmon,         {.i = -1 } },
	{ Alt + Shift,             XK_period, tagmon,         {.i = +1 } },
	Alt(                        XK_1,                      0)
	Alt(                        XK_2,                      1)
	Alt(                        XK_3,                      2)
	Alt(                        XK_4,                      3)
	Alt(                        XK_5,                      4)
	Alt(                        XK_6,                      5)
	Alt(                        XK_7,                      6)
	Alt(                        XK_8,                      7)
	Alt(                        XK_9,                      8)
	{ Alt + Shift,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         Alt,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         Alt,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         Alt,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            Alt,         Button1,        tag,            {0} },
	{ ClkTagBar,            Alt,         Button3,        toggletag,      {0} },
};

