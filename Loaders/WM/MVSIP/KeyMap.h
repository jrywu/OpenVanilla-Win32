// Keyboard information
#define NUM_KEYBDS          4
#define MAX_NUM_ROWS        5

#define MAX_KEYS_IN_ROW     11

#define NUM_MODEKEYS        2 // Eng. Chn. 
#define NUM_FUNKEYS         4 // shift shaped sym lock

//#define NUM_COMPLEXRECTS    2

#define KEYBD_HEIGHT        250
#define KEYBD_WIDTH         480

// Index of Mode
#define MODE_CHINESE        0x01
#define MODE_ENGLISH        0x02
//#define MODE_SYMBOL         0x03
#define NULLIMC_KBD         MODE_ENGLISH


// Index of Function Key
#define FN_SHAPE            0x01
#define FN_SHIFT            0x02
//#define FN_CONTROL          0x03
#define FN_SYMBOL			0x04
#define FN_LOCK				0x05

// Y coordinates of rows in the keyboards.  

const int g_5RowCoord[MAX_NUM_ROWS + 1] = {4, 56, 103, 152, 204, 246};
const int g_4RowCoord[MAX_NUM_ROWS + 1] = {11, 75, 140, 205, 246};


// Key states.
BYTE g_KeyStates[NUM_KEYBDS][MAX_NUM_ROWS][MAX_KEYS_IN_ROW + 1] = {0};

// Mode Key states
BYTE g_ModeKeyStates[NUM_MODEKEYS + 1] = {0};

// Function Key states
BYTE g_FnKeyStates[NUM_FUNKEYS + 1] = {0};

// keyboard
const int   *g_pRowCoord = g_5RowCoord;
int   g_nCurKB = 0;
int   g_nRows = MAX_NUM_ROWS;

// Flags for fdwFlags member of KEYENTRY.
#define F_VK                0x00010000      // VK key
#define F_VVK               0x00020000      // vitual VK key
#define F_STK               0x00040000      // sticky Key
#define F_TOG               0x00080000      // toggle key
#define F_MODE              0x00100000      // mode changing key
#define F_NOSHIFT           0x00200000      // VK key don't want SHIFT key down
#define F_HOLD	            0x00400000      // The key wo't effect under CONTROL mode
#define F_NOLOCK            0x00800000      // This key can not be locked
//#define F_POSTSK            0x01000000      // post send key
#define F_REPEAT            0x10000000      // repeat key
#define F_REDRAW            0x20000000      // redraw window when state changes
#define F_LOCK		        0x40000000      // The complex key
#define F_DISABLE           0x80000000      // disabled key

#define KEYINDEX_MASK       0x000000FF      // key index (last byte)


// Flags for KEYSTATE
#define STATE_DOWN          0x01
#define STATE_SET           0x02
#define STATE_LOCKED        0x04
#define STATE_DISABLED      0x08
#define STATE_PREVSET       0x10

// Private Virtual VK key definition
#define VVK_CHINESE         VK_KANJI
#define VVK_ENGLISH         VK_DBE_ALPHANUMERIC
#define VVK_SYMBOL          VK_MBUTTON
#define VVK_SHAPE           VK_CONVERT
#define VVK_SHIFT           VK_SHIFT
//#define VVK_CAPITAL         VK_CAPITAL
//#define VVK_CONTROL         VK_CONTROL
//#define VVK_ALPHANUM        VK_DBE_ALPHANUMERIC
//#define VVK_LOCK			VK_NUMLOCK
#define NO_CTL              0xFFFF
#define SENTINEL            VK_END

#define VK_PACKET		0xE7

typedef struct {
    BYTE    bVk;
    UINT    uChar;
    UINT    uFullChar;
	BYTE	bHoldVk;
    UINT    uHoldChar;
	UINT	uHoldFullCHar;
    DWORD   fdwFlags;
    int     nLeft;
} KEYENTRY;

const KEYENTRY g_keys[NUM_KEYBDS][MAX_NUM_ROWS][MAX_KEYS_IN_ROW + 1] =
{
    { // 0- main keyboard
        {   // Row 1
        	{ 0,                 0,      0,   0,    0,    0, F_DISABLE,                          0   },
			{ '1',             '1',		 0,   0,    0,    0, 0,                                  7   },
			{ '2',             '2',		 0,   0,    0,    0, 0,                                  53  },
			{ '3',             '3',		 0,   0,    0,    0, 0,                                  99  },
            { '4',             '4',		 0,   0,    0,    0, 0,                                  146 },
            { '5',             '5',		 0,   0,    0,    0, 0,                                  193 },
            { '6',             '6',		 0,   0,    0,    0, 0,                                  240 },
            { '7',             '7',		 0,   0,    0,    0, 0,                                  286 },
            { '8',             '8',		 0,   0,    0,    0, 0,                                  334 },
            { '9',             '9',		 0,   0,    0,    0, 0,                                  380 },
            { '0',             '0',		 0,   0,    0,    0, 0,                                  426 },
            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        },
        {   // Row 2
        	{ 0,                 0,      0,   0,    0,    0, F_DISABLE,                          0   },
            { 'Q',               0,      0,   0,    0,    0, F_VK,                               5   },
            { 'W',               0,      0,   0,    0,    0, F_VK,                               50  },
            { 'E',               0,      0,   0,    0,    0, F_VK,                               95  },
            { 'R',               0,      0,   0,    0,    0, F_VK,                               142 },
            { 'T',               0,      0,   0,    0,    0, F_VK,                               189 },
            { 'Y',               0,      0,   0,    0,    0, F_VK,                               236 },
            { 'U',               0,      0,   0,    0,    0, F_VK,                               283 },
            { 'I',               0,      0,   0,    0,    0, F_VK,                               330 },
            { 'O',               0,      0,   0,    0,    0, F_VK,                               377 },
            { 'P',               0,      0,   0,    0,    0, F_VK,                               423 },
            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        },
        {   // Row 3
            { 0,                 0,      0,   0,    0,    0, F_DISABLE,                          0   },
            { 'A',               0,      0,   0,    0,    0, F_VK,                               6   },
            { 'S',               0,      0,   0,    0,    0, F_VK,                               52  },
            { 'D',               0,      0,   0,    0,    0, F_VK,                               97  },
            { 'F',               0,      0,   0,    0,    0, F_VK,                               144 },
            { 'G',               0,      0,   0,    0,    0, F_VK,                               192 },
            { 'H',               0,      0,   0,    0,    0, F_VK,                               237 },
            { 'J',               0,      0,   0,    0,    0, F_VK,                               286 },
            { 'K',               0,      0,   0,    0,    0, F_VK,                               332 },
            { 'L',               0,      0,   0,    0,    0, F_VK,                               377 },
            { VK_BACK,      0x0008, 0x0008,   0,    0,    0, F_VK|F_REPEAT, 					 424 },
            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        },                                          
        {   // Row 4
            { 0,                 0,      0,   0,    0,    0, F_DISABLE,                          0   },
			{ 'Z',               0,      0,   0,    0,    0, F_VK,                               6   },
            { 'X',               0,      0,   0,    0,    0, F_VK,                               53  },
            { 'C',               0,      0,   0,    0,    0, F_VK,                               97  },
            { 'V',               0,      0,   0,    0,    0, F_VK,                               144 },
            { 'B',               0,      0,   0,    0,    0, F_VK,                               192 },
            { 'N',               0,      0,   0,    0,    0, F_VK,                               238 },
            { 'M',               0,      0,   0,    0,    0, F_VK,                               286 },
	        { VK_SEMICOLON,    ';', 0xff1b,   0,    0,    0, 0,                                  334 },
			{ VK_APOSTROPHE,  '\'', 0xff07,   0,    0,    0, 0,                                  378 },
            { VK_RETURN,    0x000d, 0x000d,   0,    0,    0, 0,				                     424 },
            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        },
        {   // Row 5
            { VVK_CHINESE,       0,      0,   0,    0,    0, F_DISABLE,                          0,  },
			{ VVK_SYMBOL,        0,      0,   0,    0,    0, F_VVK|F_TOG|F_REDRAW|FN_SYMBOL,     10  },
            { VVK_ENGLISH,       0,      0,   0,    0,    0, F_VVK|F_MODE|F_REDRAW|MODE_ENGLISH, 53  },
		    
            { VK_SPACE,        ' ',      0,   0,    0,    0, F_VK,                               107 },

			{ VK_COMMA,        ',',      0,   0,    0,    0, 0,                                  294 },
            { VK_PERIOD,       '.',      0,   0,    0,    0, 0,                                  340 },
            { VK_SLASH,        '/',      0,   0,    0,    0, 0,                                  385 },

			{ VK_EQUAL,        '=',		 0, '?',  '?',    0, F_HOLD,                             430 },

            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        }
    },
	
	{ // 1- English keyboard
        {   // Row 1
        	{ 0,                 0,      0,   0,    0,      0, F_DISABLE,                          0   },
            { 'Q',             'q', 0xff51, '1',  '1', 0xff11, F_HOLD,                             5   },
            { 'W',             'w', 0xff57, '2',  '2', 0xff12, F_HOLD,                             50  },
            { 'E',             'e', 0xff45, '3',  '3', 0xff13, F_HOLD,                             95  },
            { 'R',             'r', 0xff52, '4',  '4', 0xff14, F_HOLD,                             142 },
            { 'T',             't', 0xff54, '5',  '5', 0xff15, F_HOLD,                             189 },
            { 'Y',             'y', 0xff59, '6',  '6', 0xff16, F_HOLD,                             236 },
            { 'U',             'u', 0xff55, '7',  '7', 0xff17, F_HOLD,                             283 },
            { 'I',             'i', 0xff49, '8',  '8', 0xff18, F_HOLD,                             330 },
            { 'O',             'o', 0xff4f, '9',  '9', 0xff19, F_HOLD,                             377 },
            { 'P',             'p', 0xff50, '0',  '0', 0xff10, F_HOLD,                             423 },
            { SENTINEL,          0,      0,   0,    0,      0, F_HOLD,                             478 }
        },
        {   // Row 2
	
            { 0,                 0,      0,   0,    0,      0, F_DISABLE,                           0  },
			{ 'A',             'a', 0xff41,  '1', '!', 0xff01, F_HOLD,                              6  },
            { 'S',             's', 0xff53,  '2', '@', 0xff20, F_HOLD,                             52  },
            { 'D',             'd', 0xff44,  '3', '#', 0xff03, F_HOLD,                             97  },
            { 'F',             'f', 0xff46,  '4', '$', 0xff04, F_HOLD,                             144 },
            { 'G',             'g', 0xff47,  '5', '%', 0xff05, F_HOLD,                             192 },
            { 'H',             'h', 0xff48,  '7', '&', 0xff06, F_HOLD,                             237 },
            { 'J',             'j', 0xff4a,  '8', '*', 0xff0a, F_HOLD,                             286 },
            { 'K',             'k', 0xff4b,  '?', '?', 0xff1f, F_HOLD,                             332 },
            { 'L',             'l', 0xff4c,VK_SLASH, '/', 0xff0f, F_HOLD,                          377 },
            { VK_BACK,      0x0008, 0x0008,   0,    0,    0,   F_VK|F_REPEAT,                      424 },
            { SENTINEL,          0,      0,   0,    0,    0,   0,                                  478 }
        },                                          
        {   // Row 3
            { 0,                 0,      0,   0,    0,    0, F_DISABLE,                          0   },
			{ VK_SHIFT,          0,      0,   0,    0,    0, F_STK|F_REDRAW|FN_SHIFT,			 5   },

			{ 'Z',             'z', 0xff5a, VK_BACKQUOTE,  '~', 0xff5e, F_HOLD,                  60  },
            { 'X',             'x', 0xff58, VK_APOSTROPHE, '\"', 0xff02,F_HOLD,                  105 },
            { 'C',             'c', 0xff43, VK_APOSTROPHE, '\'', 0xff07,F_HOLD,                  150 },
            { 'V',             'v', 0xff56, '9',           '(', 0xff08, F_HOLD,                  196 },
            { 'B',             'b', 0xff42, '0',           ')', 0xff09, F_HOLD,                  243 },
            { 'N',             'n', 0xff4e, VK_HYPHEN,     '-', 0xff0d, F_HOLD,                  290 },
            { 'M',             'm', 0xff4d, VK_HYPHEN,     '_', 0xff3f, F_HOLD,                  336 },
			{ VK_COMMA,        ',', 0xff0c, VK_SEMICOLON,  ';', 0xff1b, F_HOLD,                  384 },
            { VK_RETURN,    0x000d, 0x000d,   0,            0,       0,  0,		                 429 },
            { SENTINEL,          0,      0,   0,            0,       0,  0,                      478 }
        },
        {   // Row 4
			{ 0,                 0,      0,   0,    0,    0, F_DISABLE,                           0  },
			{ VK_TAB,       0x0009, 0x0009,   0,    0,    0, 0,                                   8	 },
			{ VVK_CHINESE,       0,      0,   0,    0,    0, F_VVK|F_MODE|F_REDRAW|MODE_CHINESE,  67 },
			{ VVK_SHAPE,         0,      0,   0,    0,    0, F_VVK|F_TOG|FN_SHAPE,               126 },
            
			            
            { VK_SPACE,        ' ', 0x3000,   0,    0,    0, F_VK,                               172 },
			{ VK_PERIOD,       '.', 0xff0e,   VK_SEMICOLON, ':', 0xff1a, F_HOLD,                 376 },

			{ VVK_SYMBOL,        0,      0,   0,    0,    0, F_VVK|F_STK|F_REDRAW|FN_SYMBOL,     424 },

            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        }
    },
	{ // 2- shitfted English keyboard
        {   // Row 1
        	{ 0,                 0,      0,   0,    0,    0,   F_DISABLE,                          0   },
			{ 'Q',             'Q', 0xff31, '1',  '1', 0xff11, F_HOLD,                             5   },
            { 'W',             'W', 0xff37, '2',  '2', 0xff12, F_HOLD,                             50  },
            { 'E',             'E', 0xff25, '3',  '3', 0xff13, F_HOLD,                             95  },
            { 'R',             'R', 0xff32, '4',  '4', 0xff14, F_HOLD,                             142 },
            { 'T',             'T', 0xff34, '5',  '5', 0xff15, F_HOLD,                             189 },
            { 'Y',             'Y', 0xff39, '6',  '6', 0xff16, F_HOLD,                             236 },
            { 'U',             'U', 0xff35, '7',  '7', 0xff17, F_HOLD,                             283 },
            { 'I',             'I', 0xff29, '8',  '8', 0xff18, F_HOLD,                             330 },
            { 'O',             'O', 0xff2f, '9',  '9', 0xff19, F_HOLD,                             377 },
            { 'P',             'P', 0xff30, '0',  '0', 0xff10, F_HOLD,                             423 },
            { SENTINEL,          0,      0,   0,    0,    0,   0,                                  478 }
        },
        {   // Row 2
            { 0,                 0,      0,   0,    0,    0,   F_DISABLE,                           0  },
			{ 'A',             'A', 0xff21,  '1', '!', 0xff01, F_HOLD,                              6  },
            { 'S',             'S', 0xff33,  '2', '@', 0xff20, F_HOLD,                             52  },
            { 'D',             'D', 0xff24,  '3', '#', 0xff03, F_HOLD,                             97  },
            { 'F',             'F', 0xff26,  '4', '$', 0xff04, F_HOLD,                             144 },
            { 'G',             'G', 0xff27,  '5', '%', 0xff05, F_HOLD,                             192 },
            { 'H',             'H', 0xff28,  '7', '&', 0xff06, F_HOLD,                             237 },
            { 'J',             'J', 0xff2a,  '8', '*', 0xff0a, F_HOLD,                             286 },
            { 'K',             'K', 0xff2b,  '?', '?', 0xff1f, F_HOLD,                             332 },
            { 'L',             'L', 0xff2c,VK_SLASH, '/', 0xff0f, F_HOLD,                          377 },
            { VK_BACK,      0x0008, 0x0008,   0,    0,    0,   F_VK|F_REPEAT,                      424 },
            { SENTINEL,          0,      0,   0,    0,    0,   0,                                  478 }
        },                                          
        {   // Row 3
            { 0,                 0,      0,   0,    0,    0, F_DISABLE,                          0   },
			{ VK_SHIFT,          0,      0,   0,    0,    0, F_STK|F_REDRAW|FN_SHIFT,			 8   },
			{ 'Z',             'Z', 0xff3a, VK_BACKQUOTE,  '~', 0xff5e, F_HOLD,                  60  },
            { 'X',             'X', 0xff38, VK_APOSTROPHE, '\"', 0xff02,F_HOLD,                  105 },
            { 'C',             'C', 0xff23, VK_APOSTROPHE, '\'', 0xff07,F_HOLD,                  150 },
            { 'V',             'V', 0xff36, '9',           '(', 0xff08, F_HOLD,                  196 },
            { 'B',             'B', 0xff22, '0',           ')', 0xff09, F_HOLD,                  243 },
            { 'N',             'N', 0xff2e, VK_HYPHEN,     '-', 0xff0d, F_HOLD,                  290 },
            { 'M',             'M', 0xff2d, VK_HYPHEN,     '_', 0xff3f, F_HOLD,                  336 },
			{ VK_COMMA,        ',', 0xff0c, VK_SEMICOLON,  ';', 0xff1b, F_HOLD,                  384 },
            { VK_RETURN,    0x000d, 0x000d,   0,            0,    0, 0,			                 429 },
            { SENTINEL,          0,      0,   0,            0,    0, 0,                          478 }
        },
        {   // Row 4
            { 0,                 0,      0,   0,    0,    0, F_DISABLE,                          0   },
			{ FN_SHIFT,			 0,      0,   0,    0,    0, F_VVK|F_LOCK|F_TOG|FN_LOCK,		 8   },
			{ VVK_CHINESE,       0,      0,   0,    0,    0, F_VVK|F_MODE|F_REDRAW|MODE_CHINESE, 67  },
			{ VVK_SHAPE,         0,      0,   0,    0,    0, F_VVK|F_TOG|FN_SHAPE,               126 },
            
			            
            { VK_SPACE,        ' ', 0x3000,   0,    0,    0, F_VK,                               172 },
			{ VK_PERIOD,       '.', 0xff0e,  VK_SEMICOLON, ':', 0xff1a, F_HOLD,                  376 },

			{ VVK_SYMBOL,        0,      0,   0,    0,    0, F_VVK|F_STK|F_REDRAW|FN_SYMBOL,	 424 },

            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        }
    },

	{ // 3- symbol keyboard 
        {   // Row 1
        	{   0,               0,      0,   0,    0,    0, F_DISABLE,                          0   },
			
			{ '1',             '1', 0xff11,   0,    0,    0, 0,                                  7   },
			{ '2',             '2', 0xff12,   0,    0,    0, 0,                                  53  },
			{ '3',             '3', 0xff13,   0,    0,    0, 0,                                  99  },
            { '4',             '4', 0xff14,   0,    0,    0, 0,                                  146 },
            { '5',             '5', 0xff15,   0,    0,    0, 0,                                  193 },
            { '6',             '6', 0xff16,   0,    0,    0, 0,                                  240 },
            { '7',             '7', 0xff17,   0,    0,    0, 0,                                  286 },
            { '8',             '8', 0xff18,   0,    0,    0, 0,                                  334 },
            { '9',             '9', 0xff19,   0,    0,    0, 0,                                  380 },
            { '0',             '0', 0xff10,   0,    0,    0, 0,                                  426 },
			

            
            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        },
        {   // Row 2
        	{ 0,                 0,      0,   0,    0,    0, F_DISABLE,                          0   },
			{ '1',             '!', 0xff01,   0,    0,    0, 0,                                  5   },
            { '2',             '@', 0xff20,   0,    0,    0, 0,                                  50  },
            { '3',             '#', 0xff03,   0,    0,    0, 0,                                  95  },
            { '4',             '$', 0xff04,   0,    0,    0, 0,                                  142 },
            { '5',             '%', 0xff05,   0,    0,    0, 0,                                  189 },
            { '6',             '^', 0xff3e,   0,    0,    0, 0,                                  236 },
            { '7',             '&', 0xff06,   0,    0,    0, 0,                                  283 },
            { '8',             '*', 0xff0a,   0,    0,    0, 0,                                  330 },
            { '9',             '(', 0xff08,   0,    0,    0, 0,                                  377 },
            { '0',             ')', 0xff09,   0,    0,    0, 0,                                  423 },
            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        },
        {   // Row 3
            { 0,                 0,      0,   0,    0,    0, F_DISABLE,                          0   },
			{ VK_BACKQUOTE,    '`', 0xff40,   0,    0,    0, 0,									 6   },
            { VK_BACKQUOTE,    '~', 0xff5e,   0,    0,    0, 0,									 52  },
            
	        { VK_HYPHEN,       '-', 0xff0d,   0,    0,    0, 0,                                  97  },
            { VK_HYPHEN,       '_', 0xff3f,   0,    0,    0, 0,                                  144 },
			{ VK_EQUAL,        '=', 0xff1d,   0,    0,    0, 0,                                  192 },
            { VK_EQUAL,        '+', 0xff0b,   0,    0,    0, 0,                                  237 },
            { VK_LBRACKET,     '{', 0xff5b,   0,    0,    0, 0,                                  286 },
            { VK_RBRACKET,     '}', 0xff5d,   0,    0,    0, 0,                                  332 },
			{ VK_BACKSLASH,     '|', 0xff5c,  0,    0,    0, 0,                                  377 },
            { VK_BACK,      0x0008, 0x0008,   0,    0,    0, F_VK|F_REPEAT,                      424 },
            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        },                                          
        {   // Row 4
            { 0,                 0,      0,   0,    0,    0, F_DISABLE,                           0  },
			{ VK_LBRACKET,     '{', 0xff5b,   0,    0,    0, 0,                                   6  },
            { VK_RBRACKET,     '}', 0xff5d,   0,    0,    0, 0,                                   52 },
			
			{ VK_SEMICOLON,    ';', 0xff1b,   0,    0,    0, 0,                                   97 },
			{ VK_SEMICOLON,    ':', 0xff1a,   0,    0,    0, 0,                                  144 },            
			
			{ VK_APOSTROPHE,  '\"', 0xff02,   0,    0,    0, 0,                                  192 },
			{ VK_APOSTROPHE,  '\'', 0xff07,   0,    0,    0, 0,                                  237 },
	        
			{ VK_COMMA,        ',', 0xff0c,   0,    0,    0, 0,                                  286 },
            { VK_PERIOD,       '.', 0xff0e,   0,    0,    0, 0,                                  332 },
            { VK_SLASH,        '/', 0xff0f,   0,    0,    0, 0,                                  377 },
            { VK_RETURN,    0x000d, 0x000d,   0,    0,    0, 0,				                     424 },

            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        },
        {   // Row 5
			{ VVK_SYMBOL,		 0,      0,   0,    0,    0, F_VVK|F_STK|FN_SYMBOL|F_REDRAW|F_DISABLE,0},

			{ VVK_CHINESE,       0,      0,   0,    0,    0, F_VVK|F_MODE|F_REDRAW|MODE_CHINESE,  6, },
			{ VVK_SHAPE,         0,      0,   0,    0,    0, F_VVK|F_TOG|FN_SHAPE,                65 },
                        
            { VK_SPACE,        ' ', 0x3000,   0,    0,    0, F_VK,                               116 },

			{ VK_COMMA,        '<', 0xff1c,   0,    0,    0, 0,                                  237 },
            { VK_PERIOD,       '>', 0xff1e,   0,    0,    0, 0,                                  286 },
            { '?',             '?', 0xff1f,   0,    0,    0, 0,                                  332 },
			{ VK_BACKSLASH,    '\\',0xff3c,   0,    0,    0, 0,                                  377 },
			{ FN_SYMBOL,		 0,      0,	  0,    0,    0, F_VVK|F_LOCK|F_TOG|FN_LOCK,		 424 },
			

            { SENTINEL,          0,      0,   0,    0,    0, 0,                                  478 }
        }
    }
};




