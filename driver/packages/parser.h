#ifndef ZORKPARSE_H
#define ZORKPARSE_H

/* Token convention:
 * <0 is a token (OBJ, etc).
 * >=0 is a literal.
 */
#define ERROR_TOKEN          -1
#define STR_TOKEN	     -2

#define LIV_MODIFIER         8
#define VIS_ONLY_MODIFIER    16

#define OBJ_A_TOKEN          -4
#define LIV_A_TOKEN          -((-OBJ_A_TOKEN) | LIV_MODIFIER)
#define OBJ_TOKEN	     -((-OBJ_A_TOKEN) | VIS_ONLY_MODIFIER)
#define LIV_TOKEN	     -((-LIV_A_TOKEN) | VIS_ONLY_MODIFIER)

#define MAX_NUM_OBJECTS      256
/* must be powers of 2 */
#define HASH_SIZE            32
#define VERB_HASH_SIZE       128

/* This is used to hash shared string pointers for various lookup tables */
#define DO_HASH(x, n)         ((((int)x) & (n - 1)) ^ \
			      (((int)x >> 8) & (n - 1)) ^ \
			      (((int)x >> 16) & (n - 1)))

/*
 * bitvec stuff.  Basically, at the start of parsing, we determine what
 * objects are involved in the parse, and assign each a number.  Then
 * we use the following bitvectors to keep track of sets of objects,
 * so we can intersect sets fast etc.
 */
/* bits per int */
#define BPI ((sizeof(int)) * 8)
#define NUM_BITVEC_INTS (MAX_NUM_OBJECTS / BPI)
#define BV_WHICH(x) ((x) / BPI)
#define BV_BIT(x) (1 << ((x) % BPI))

/* A parse value.  This keeps track of which objects respond to a given
 * word and how.  For example:
 *
 * ob1: "wolf sword"   adj = wolf  noun = sword
 * ob2: "gray wolf"    adj = gray  noun = wolf
 * ob3: "white wolf"   adj = white noun = wolf
 *
 * "wolf":
 *   noun: (ob2 and ob3)
 *   adj:  (ob1)
 */
typedef struct {
    unsigned int noun[NUM_BITVEC_INTS];
    unsigned int plural[NUM_BITVEC_INTS];
    unsigned int adj[NUM_BITVEC_INTS];
} parse_val_t;

#define WORD_ALLOCATED 1

typedef struct {
    int type;
    char *string;
    char *end_prev;
    char *start_next;
} word_t;

/* Flags for parse_info structures.  parse_info information is cached inside
 * objects so we don't have to call the relevant LPC functions over and over
 * and over.  parse_refresh() is provided to dump the cached info, for cases
 * where it changes.
 *
 * parse_init() allocates the pinfo structure and sets PI_SETUP to zero.
 * The actual info will be filled in when needed, and PI_SETUP will then
 * be set to 1.  parse_refresh() actually just zeros PI_SETUP.
 */
#define PI_SETUP		1
#define PI_LIVING		2
#define PI_VERB_HANDLER		4
#define PI_REMOTE_LIVINGS	8
#define PI_INV_ACCESSIBLE	16
#define PI_INV_VISIBLE		32

typedef struct parse_info_s {
    int flags;
    struct object_s *ob;
    int num_ids, num_adjs, num_plurals;
    char **ids, **adjs, **plurals;
} parse_info_t;

/* HV_PERM indicates that an entry shouldn't be removed from the hash
 * table.  Currently, unused.  Good for global objects, etc
 */
#define HV_PERM   1
#define HV_NOUN   2
#define HV_PLURAL 4
#define HV_ADJ    8

/* An entry in the hash table that hashes words->interpretations;
 * Hmm ... maybe flags here should be removed.
 */
typedef struct hash_entry_s {
    struct hash_entry_s *next;
    char *name;
    int flags;
    parse_val_t pv;
} hash_entry_t;

/* Each node holds informations about a given rule.  The handler for the
 * rule, the literals it contains, and the token string (OBJ, "to", OBJ)
 * are stored in here.  Note that it is variable size.
 */
typedef struct verb_node_s {
    struct verb_node_s *next;
    struct object_s *handler;
    short lit[2];
    int token[1];
} verb_node_t;

/* 
 * The entry for a verb.  Links for the verb hash table, and a linked
 * list of rules.
 */
typedef struct verb_s {
    struct verb_s *next;
    char *name;
    verb_node_t *node;
} verb_t;

/* A token definition for the token lookup table */
typedef struct {
    char *name;
    int token;
    int mod_legal;
} token_def_t;

union mu {
    int number;
    char *string;
};

typedef struct {
    int token;
    int first, last;
    union mu val;
} match_t;

typedef struct {
    int tok_index, word_index;
    int match_level;
    int num_matches;
    int num_errors;
} parse_state_t;

typedef struct {
    char *func;
    int num;
    svalue_t *args;
} sub_result_t;

typedef struct {
    object_t *ob;
    sub_result_t res[4];
} parse_result_t;

#endif
