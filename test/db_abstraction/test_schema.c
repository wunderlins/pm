#include <stdio.h>
#include <stdlib.h>
#include "test_schema.h"

int add_field(db_table_t *table, db_field_t *field) {

	if (field->primary_key)
		table->_key = table->_num_fields;

	db_field_t *temp;
	temp = (db_field_t *) realloc(table->fields,
			((table->_num_fields+1) * sizeof(db_field_t)));

	if (temp == NULL)
		return 1;
	*table->fields = *temp;

	table->fields[table->_num_fields] = *field;

	table->_num_fields += 1;
	return 0;
}

int main() {
	db_table_t t1 = {.name = "my_table", ._num_fields = 0, ._key = 0};
	printf("num: %d\n", t1._num_fields);

	// field 1
	db_field_t f1 = {
			.name = "id",
			.size = 8,
			.autoincrement = 1,
			.null = 0,
			.primary_key = 1
	};
	add_field(&t1, &f1);

	printf("field 0, %s\n", t1.fields[0].name);

	return 0;
}
