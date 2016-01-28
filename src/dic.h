struct bucket {
	int key;
	char *value;
	struct bucket *next;
};

typedef struct bucket bucket;

struct dict {
	int	size;
	struct bucket **words;
};

typedef struct dict dict;
