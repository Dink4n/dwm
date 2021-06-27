/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 4;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char font[]            = "monospace:size=12";
static const char *fonts[]          = { font, "JoyPixels:pixelsize=14:antialias=true:autohint=true" };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",			NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "St",      		NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      		NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
	{ "Game",    		NULL,     NULL,           0,         1,          0,           1,        -1 },
    { "glslViewer",     NULL,     NULL,           0,         1,          0,           1,        -1 },
	{ "Hdajackretask",  NULL,     NULL,           0,         0,          0,           1,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },       /* Master on left, slaves on right */
    { "TTT",      bstack },     /* Master on top, slaves on bottom */

 	{ "[M]",	  monocle },    /* All windows on top of eachother */
    { "H[]",      deck },       /* Master on left, monocle mode on right */

	{ "[@]",	  spiral },	    /* Fibonacci spiral */
	{ "[\\]",	  dwindle },    /* Decreasing in size right and leftward */

    { "|M|",      centeredmaster }, /* Master in middle, slaves on sides */
    { ">M>",      centeredfloatingmaster }, /* Same as centeredmaster, but master floats */

	{ "><>",      NULL },       /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* constants */
#define TERM "st"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERM, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
    /* wm independent hotkeys */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_w,      spawn,          SHCMD("$BROWSER") },
    { MODKEY,                       XK_r,      spawn,          SHCMD("randwal") },
    { MODKEY|ShiftMask,             XK_l,      spawn,          SHCMD("dmenubooks") },
    { MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("loginctl suspend") },

    /* Audio */
    { MODKEY,                       XK_a,      spawn,          SHCMD(TERM " -e pulsemixer") },
    { MODKEY,                       XK_equal,  spawn,          SHCMD("amixer -c 1 set Master +5") },
    { MODKEY,                       XK_minus,  spawn,          SHCMD("amixer -c 1 set Master -5") },
    { MODKEY,                       XK_s,      spawn,          SHCMD("amixer -c 1 set Headphone toggle; amixer -c 1 set Front toggle") },

    /* dwm hotkeys */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

    /* Gaps */
    { MODKEY,                       XK_g,      defaultgaps,    {0} },
    { MODKEY|ShiftMask,             XK_g,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_equal,  incrgaps,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_minus,  incrgaps,       {.i = -1 } },

    /* Layouts */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[1]} }, /* bstack */
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} }, /* monocle */
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[3]} }, /* deck */
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[4]} }, /* spiral */
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[5]} }, /* dwindle */
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[7]} }, /* centerdfloatingmaster */
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefloating, {0} },

    /* Tags */
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_bracketright,  shiftviewclients, { .i = +1 } },
	{ MODKEY,                       XK_bracketleft,   shiftviewclients, { .i = -1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

