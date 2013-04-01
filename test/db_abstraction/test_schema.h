// database structure definition

typedef enum {INTEGER, VARCHAR, TEXT} db_type_t;
typedef struct {
	db_type_t type;
	char *name;

	// following data could be packed into a union?
	int size;
	int autoincrement;
	int null;
	int primary_key;
} db_field_t;

typedef struct {
	int _fields_allocated = ((void *)0);
	db_field_t fields[];
	int _key = ((void *)0);
} db_tablename_t;

db_tablename_t create_table(int num_fields) {
	db_tablename_t t;

	// FIXME: this should allocate enough mem for all the db_fields.
	//        i have no fuckin clue if this is going to work nor how it
	//        should work in C.
	db_field_t fields[num_fields] = (db_field_t *)
			                        malloc(sizeof(db_field_t)*num_fields);

	return t;
}
